//-----------------------------------------------------------------------
// <copyright file="MachineState.cpp">
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

#include "P3/MachineState.h"
#include "P3/Machine.h"
#include "P3/ActorId.h"
#include <string>

using namespace Microsoft::P3;

MachineState::MachineState(std::string name, Machine& machine)
{
    m_name = name;
    _machine = &machine;
    m_onEntryAction = nullptr;
    m_onExitAction = nullptr;
}

void MachineState::SetOnEntryAction(Action onEntry)
{
    _machine->Assert(m_onEntryAction == nullptr,
        "The on-entry action for state '" + m_name + "' in machine '" + _machine->m_id->m_name + "' has already been set.");
    m_onEntryAction = onEntry;
}

void MachineState::SetOnExitAction(Action onExit)
{
    _machine->Assert(m_onExitAction == nullptr,
        "The on-exit action for state '" + m_name + "' in machine '" + _machine->m_id->m_name + "' has already been set.");
    m_onExitAction = onExit;
}

void MachineState::SetOnEventGotoState(std::string event, std::string destination)
{
    CheckPreviousDeclaration(event);
    m_gotoTransitions[event] = destination;
}

void MachineState::SetOnEventPushState(std::string event, std::string destination)
{
    CheckPreviousDeclaration(event);
    m_pushTransitions[event] = destination;
}

void MachineState::SetOnEventDoAction(std::string event, Action action)
{
    CheckPreviousDeclaration(event);
    m_actionBindings[event] = action;
}

void MachineState::SetIgnoredEvent(std::string event)
{
    CheckPreviousDeclaration(event);
    m_ignoredEvents.insert(event);
}

void MachineState::SetDeferredEvent(std::string event)
{
    CheckPreviousDeclaration(event);
    m_deferredEvents.insert(event);
}

// Checks if the event has been already declared in a handler for this state.
void MachineState::CheckPreviousDeclaration(std::string event)
{
    _machine->Assert(m_gotoTransitions.find(event) == m_gotoTransitions.end(),
        "The '" + event + "' is already declared in a goto transition in state '" + m_name + "' of machine '" + _machine->m_id->m_name + "'.");
    _machine->Assert(m_pushTransitions.find(event) == m_pushTransitions.end(),
        "The '" + event + "' is already declared in a push transition in state '" + m_name + "' of machine '" + _machine->m_id->m_name + "'.");
    _machine->Assert(m_actionBindings.find(event) == m_actionBindings.end(),
        "The '" + event + "' is already declared in an action binding in state '" + m_name + "' of machine '" + _machine->m_id->m_name + "'.");
    _machine->Assert(m_ignoredEvents.find(event) == m_ignoredEvents.end(),
        "The '" + event + "' is already ignored in state '%s' of machine '" + m_name + "'.");
    _machine->Assert(m_deferredEvents.find(event) == m_deferredEvents.end(),
        "The '" + event + "' is already deferred in state '%s' of machine '" + m_name + "'.");
}