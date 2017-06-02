//-----------------------------------------------------------------------
// <copyright file="MonitorState.cpp">
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

#include "P3/MonitorState.h"
#include "P3/Monitor.h"
#include "P3/ActorId.h"
#include <string>

using namespace Microsoft::P3;

MonitorState::MonitorState(std::string name, Monitor& monitor)
{
    m_name = name;
    m_monitor = &monitor;
    m_onEntryAction = nullptr;
    m_onExitAction = nullptr;
}

void MonitorState::SetOnEntryAction(Action onEntry)
{
    m_monitor->Assert(m_onEntryAction == nullptr,
        "The on-entry action for state '" + m_name + "' in monitor '" + m_monitor->m_name + "' has already been set.");
    m_onEntryAction = onEntry;
}

void MonitorState::SetOnExitAction(Action onExit)
{
    m_monitor->Assert(m_onExitAction == nullptr,
        "The on-exit action for state '" + m_name + "' in monitor '" + m_monitor->m_name + "' has already been set.");
    m_onExitAction = onExit;
}

void MonitorState::SetOnEventGotoState(std::string event, std::string destination)
{
    CheckPreviousDeclaration(event);
    m_gotoTransitions[event] = destination;
}

void MonitorState::SetOnEventDoAction(std::string event, Action action)
{
    CheckPreviousDeclaration(event);
    m_actionBindings[event] = action;
}

// Checks if the event has been already declared in a handler for this state.
void MonitorState::CheckPreviousDeclaration(std::string event)
{
    m_monitor->Assert(m_gotoTransitions.find(event) == m_gotoTransitions.end(),
        "The '" + event + "' is already declared in a goto transition in state '" + m_name + "' of monitor '" + m_monitor->m_name + "'.");
    m_monitor->Assert(m_actionBindings.find(event) == m_actionBindings.end(),
        "The '" + event + "' is already declared in an action binding in state '" + m_name + "' of monitor '" + m_monitor->m_name + "'.");
}