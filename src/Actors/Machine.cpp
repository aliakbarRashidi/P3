//-----------------------------------------------------------------------
// <copyright file="Machine.cpp">
//      Copyright (c) Microsoft Corporation. All rights reserved.
// 
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//      TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// </copyright>
//-----------------------------------------------------------------------

#include "P3/Actors/Machine.h"
#include "P3/Actors/MachineState.h"
#include "P3/Actors/ActorId.h"
#include "P3/Runtime.h"
#include <iostream>
#include <memory>
#include <mutex>

using namespace Microsoft::P3;

Machine::Machine()
{
    m_currentState = nullptr;
    m_raisedEvent = nullptr;
    m_isRunning = true;
    m_isHalted = false;
}

void Machine::Send(const ActorId& target, std::unique_ptr<Event> event)
{
    // If the event is null, then report an error.
    Runtime->Assert(event != nullptr, "Cannot send a null event.");
    Runtime->SendEvent(target, std::move(event), m_id.get());
}

void Machine::Raise(std::unique_ptr<Event> event)
{
    // If the event is null, then report an error.
    Runtime->Assert(event != nullptr, "Cannot send a null event.");
    m_raisedEvent = move(event);
    Runtime->NotifyRaisedEvent(*this, *(m_raisedEvent.get()));
}

void Machine::Assert(bool predicate)
{
    if (!predicate)
    {
        Runtime->Assert(predicate);
    }
}

void Machine::Assert(bool predicate, const std::string& message)
{
    Runtime->Assert(predicate, message);
}

void Machine::Assert(bool predicate, std::ostringstream& stream)
{
    Runtime->Assert(predicate, stream);
}

const ActorId* Machine::GetId()
{
    return m_id.get();
}

// Gets the next available event. It gives priority to raised events, else deqeues
// from the inbox. Returns false if the next event was not dequeued. It returns a
// null event if no event is available.
std::unique_ptr<Event> Machine::GetNextEvent(bool& isDequeued)
{
    std::unique_ptr<Event> nextEvent = nullptr;
    if (m_raisedEvent)
    {
        nextEvent = std::move(m_raisedEvent);
    }

    // If there is no raised event, then dequeue.
    if (nextEvent == nullptr && m_inbox.size() > 0)
    {
        nextEvent = std::move(m_inbox.front());
        m_inbox.pop_front();
        isDequeued = true;
    }

    return nextEvent;
}

// Runs the event handler. The handler terminates if there is
// no next event to process or if the machine is halted.
void Machine::RunEventHandler()
{
    if (m_isHalted)
    {
        // If the machine has halted, do nothing.
        return;
    }

    std::unique_ptr<Event> nextEvent = nullptr;
    while (!m_isHalted)
    {
        bool isDequeued = false;

        {
            // Lock the queue to avoid data races.
            std::lock_guard<std::mutex> lock(m_inboxLock);
            nextEvent = GetNextEvent(isDequeued);
        }

        // Check if next event to process is null.
        if (nextEvent == nullptr)
        {
            m_isRunning = false;
            break;
        }

        // Handle the next event.
        HandleEvent(std::move(nextEvent));
    }
}

// Handles the specified event.
void Machine::HandleEvent(std::unique_ptr<Event> event)
{
    while (true)
    {
        if (m_gotoTransitions.find(event->m_name) != m_gotoTransitions.end())
        {
            auto state = m_gotoTransitions[event->m_name];
            GotoState(state, std::move(event));
        }
        else if (m_actionBindings.find(event->m_name) != m_actionBindings.end())
        {
            auto handler = m_actionBindings[event->m_name];
            Do(handler, std::move(event));
        }

        break;
    }
}

// Starts the machine with the specified event. It transitions to
// the start state, and executes the entry action, if there is any.
void Machine::Start(std::unique_ptr<Event> event)
{
    Assert(m_currentState != nullptr,
        "The start state for '" + m_id->m_name + "' has not been declared.");
    DoStatePush(*(m_currentState));
    ExecuteCurrentStateOnEntry(std::move(event));
}

// Performs a goto transition to the specified state.
void Machine::GotoState(std::string state, std::unique_ptr<Event> event)
{
    // The machine performs the on-exit action of the current state.
    ExecuteCurrentStateOnExit();
    if (m_isHalted)
    {
        return;
    }

    DoStatePop();
    m_currentState = m_states[state].get();
    DoStatePush(*(m_currentState));
    ExecuteCurrentStateOnEntry(std::move(event));
}

// Executes the on-entry action of the current state.
void Machine::ExecuteCurrentStateOnEntry(std::unique_ptr<Event> event)
{
    // Notifies the runtime that the machine transitions to a new state.
    Runtime->NotifyEnteredState(*this);

    Action entryAction = m_currentState->m_onEntryAction;

    // Invokes the on-entry action of the new state, if there is one available.
    if (entryAction)
    {
        Runtime->NotifyInvokedAction(*this);
        entryAction(std::move(event));
    }
}

// Executes the on-exit action of the current state.
void Machine::ExecuteCurrentStateOnExit()
{
    // Notifies the runtime that the machine exits the current state.
    Runtime->NotifyExitedState(*this);

    Action exitAction = m_currentState->m_onExitAction;

    // Invokes the on-exit action of the current state, if there is one available.
    if (exitAction)
    {
        Runtime->NotifyInvokedAction(*this);
        exitAction(nullptr);
    }
}

// Invokes the specified action.
void Machine::Do(Action action, std::unique_ptr<Event> event)
{
    Runtime->NotifyInvokedAction(*this);
    action(std::move(event));
}

MachineState* Machine::AddState(std::string name, bool isStart)
{
    if (m_states.find(name) == m_states.end())
    {
        std::unique_ptr<MachineState> state(new MachineState(name, *this));
        m_states[name] = std::move(state);
    }

    if (isStart)
    {
        Assert(m_currentState == nullptr,
            "The start state for '" + m_id->m_name + "' has already been set.");
        m_currentState = m_states[name].get();
    }

    return m_states[name].get();
}

// Configures the state transitions of the machine when a state is pushed on to the stack.
void Machine::DoStatePush(MachineState& state)
{
    m_gotoTransitions = state.m_gotoTransitions;
    _pushTransitions = state._pushTransitions;
    m_actionBindings = state.m_actionBindings;
}

// Configures the state transitions of the machine when a state is popped.
void Machine::DoStatePop()
{

}

std::string Machine::GetCurrentState()
{
    return m_currentState->m_name;
}

Machine::~Machine() { }
