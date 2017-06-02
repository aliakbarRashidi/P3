//-----------------------------------------------------------------------
// <copyright file="Monitor.cpp">
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

#include "P3/Monitor.h"
#include "P3/MonitorState.h"
#include "P3/Runtime.h"
#include <iostream>
#include <memory>

using namespace Microsoft::P3;

Monitor::Monitor()
{
    m_currentState = nullptr;
    m_raisedEvent = nullptr;
}

void Monitor::Setup(std::string name, Runtime& runtime)
{
    m_runtime = &runtime;
    m_name = name;
}

void Monitor::Raise(std::unique_ptr<Event> event)
{
    // If the event is null, then report an error.
    m_runtime->Assert(event != nullptr, "Cannot send a null event.");
    m_raisedEvent = move(event);
    m_runtime->NotifyRaisedEvent(*this, *(m_raisedEvent.get()));
}

void Monitor::Assert(bool predicate)
{
    if (!predicate)
    {
        m_runtime->Assert(predicate);
    }
}

void Monitor::Assert(bool predicate, const std::string& message)
{
    m_runtime->Assert(predicate, message);
}

void Monitor::Assert(bool predicate, std::ostringstream& stream)
{
    m_runtime->Assert(predicate, stream);
}

// Handles the specified event.
void Monitor::HandleEvent(std::unique_ptr<Event> event)
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
}

// Transitions to the start state, and executes the entry action, if there is any.
void Monitor::GotoStartState(std::unique_ptr<Event> event)
{
    Assert(m_currentState != nullptr,
        "The start state for monitor '" + m_name + "' has not been declared.");
    DoStatePush(*(m_currentState));
    ExecuteCurrentStateOnEntry(std::move(event));
}

// Performs a goto transition to the specified state.
void Monitor::GotoState(std::string state, std::unique_ptr<Event> event)
{
    // The monitor performs the on-exit action of the current state.
    ExecuteCurrentStateOnExit();
    DoStatePop();
    m_currentState = m_states[state].get();
    DoStatePush(*(m_currentState));
    ExecuteCurrentStateOnEntry(std::move(event));
}

// Executes the on-entry action of the current state.
void Monitor::ExecuteCurrentStateOnEntry(std::unique_ptr<Event> event)
{
    // Notifies the runtime that the monitor transitions to a new state.
    m_runtime->NotifyEnteredState(*this);

    Action entryAction = m_currentState->m_onEntryAction;

    // Invokes the on-entry action of the new state, if there is one available.
    if (entryAction)
    {
        m_runtime->NotifyInvokedAction(*this);
        entryAction(std::move(event));
    }
}

// Executes the on-exit action of the current state.
void Monitor::ExecuteCurrentStateOnExit()
{
    // Notifies the runtime that the monitor exits the current state.
    m_runtime->NotifyExitedState(*this);

    Action exitAction = m_currentState->m_onExitAction;

    // Invokes the on-exit action of the current state, if there is one available.
    if (exitAction)
    {
        m_runtime->NotifyInvokedAction(*this);
        exitAction(nullptr);
    }
}

// Invokes the specified action.
void Monitor::Do(Action action, std::unique_ptr<Event> event)
{
    m_runtime->NotifyInvokedAction(*this);
    action(std::move(event));
}

MonitorState* Monitor::AddState(std::string name, bool isStart)
{
    if (m_states.find(name) == m_states.end())
    {
        std::unique_ptr<MonitorState> state(new MonitorState(name, *this));
        m_states[name] = std::move(state);
    }

    if (isStart)
    {
        Assert(m_currentState == nullptr,
            "The start state for monitor '" + m_name + "' has already been set.");
        m_currentState = m_states[name].get();
    }

    return m_states[name].get();
}

// Configures the state transitions of the monitor when a state is pushed on to the stack.
void Monitor::DoStatePush(MonitorState& state)
{
    m_gotoTransitions = state.m_gotoTransitions;
    m_actionBindings = state.m_actionBindings;
}

// Configures the state transitions of the monitor when a state is popped.
void Monitor::DoStatePop()
{

}

std::string Monitor::GetCurrentState()
{
    return m_currentState->m_name;
}

Monitor::~Monitor() { }
