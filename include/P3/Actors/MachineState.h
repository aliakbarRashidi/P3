//-----------------------------------------------------------------------
// <copyright file="MachineState.h">
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

#ifndef MICROSOFT_P3_ACTORS_MACHINESTATE_H
#define MICROSOFT_P3_ACTORS_MACHINESTATE_H

#include "Action.h"
#include <map>
#include <set>
#include <string>

namespace Microsoft { namespace P3
{
    // The state of a state-machine.
    class MachineState final
    {
        friend class Machine;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        // Sets the on-entry action for this state.
        void SetOnEntryAction(Action onEntry);

        // Sets the on-exit action for this state.
        void SetOnExitAction(Action onExit);

        // Sets the goto state transition when the machine receives the specified event in this state.
        void SetOnEventGotoState(std::string event, std::string destination);

        // Sets the push state transition when the machine receives the specified event in this state.
        void SetOnEventPushState(std::string event, std::string destination);

        // Sets the action handler that is invoked when the machine receives the specified event in this state.
        void SetOnEventDoAction(std::string event, Action action);

        // Sets an ignored event, that is dropped if dequeued while the machine is in this state.
        void SetIgnoredEvent(std::string event);

        // Sets a deferred event, that is skipped if dequeued while the machine is in this state.
        void SetDeferredEvent(std::string event);

    private:
        // Handler to the machine that owns this state.
        Machine* _machine;

#pragma warning(push)
#pragma warning(disable: 4251)
        // The name of this state.
        std::string m_name;

        // The on-entry action of the state.
        Action m_onEntryAction;

        // The on-exit action of the state.
        Action m_onExitAction;

        // Map containing events to goto state transitions.
        std::map<std::string, std::string> m_gotoTransitions;

        // Map containing events to push state transitions.
        std::map<std::string, std::string> _pushTransitions;

        // Map containing events to action bindings.
        std::map<std::string, Action> m_actionBindings;

        // Set of ignored events.
        std::set<std::string> m_ignoredEvents;

        // Set of deferred events.
        std::set<std::string> m_deferredEvents;
#pragma warning(pop)

        MachineState(std::string name, Machine& machine);

        // Copy is disabled.
        MachineState(const MachineState& that) = delete;
        MachineState &operator=(MachineState const &) = delete;

        void CheckPreviousDeclaration(std::string event);
    };
} }

#endif // MICROSOFT_P3_ACTORS_MACHINESTATE_H
