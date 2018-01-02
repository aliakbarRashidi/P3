//-----------------------------------------------------------------------
// <copyright file="Machine.h">
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

#ifndef MICROSOFT_P3_MACHINE_H
#define MICROSOFT_P3_MACHINE_H

#include "Actor.h"
#include "Action.h"
#include "MachineState.h"
#include "P3/Event.h"
#include "P3/Runtime.h"
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>

namespace Microsoft { namespace P3
{
    class ActorId;
    class EventHandler;

#pragma warning(push)
#pragma warning(disable: 4275)
    // Abstract class representing a state-machine.
    class Machine : public Actor
    {
        friend class MachineState;
        friend class Runtime;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        virtual ~Machine() = 0;

    protected:
        Machine();

        // Initializes the state machine.
        virtual void Initialize() = 0;

        // Raises an event internally at the end of the current action.
        void Raise(std::unique_ptr<Event> event);

        // Transitions the machine to the state with the specified
        // name at the end of the current action.
        void Jump(std::string stateName);

        // Pops the current state from the machine state stack
        // at the end of the current action.
        void Pop();

        // Returns the current machine state.
        std::string GetCurrentState();

        // Adds a state with the specified name to the machine.
        MachineState* AddState(std::string name, bool isStart = false);

    private:
#pragma warning(push)
#pragma warning(disable: 4251)
        // Available states of this machine.
        std::map<std::string, std::unique_ptr<MachineState>> m_states;

        // start state of the machine
        std::string startState;

        // Stack of currently installed machine states.
        std::stack<MachineState*> m_stateStack;

        // A stack of maps that determine event handling action for each event type.
        // These maps do not keep transition handlers. This stack has always the
        // same height as m_stateStack.
        std::stack<std::map<std::string, std::shared_ptr<EventHandler>>> m_actionHandlerStack;

        // Map containing events to goto state transitions for the currently installed state.
        std::map<std::string, std::string> m_gotoTransitions;

        // Map containing events to push state transitions for the currently installed state.
        std::map<std::string, std::string> m_pushTransitions;
#pragma warning(pop)

        // Gets the raised event. If no event has been raised this will return null.
        std::unique_ptr<Event> m_raisedEvent;

        bool m_isPopInvoked;

        std::unique_ptr<Event> GetNextEvent(bool& isDequeued);
        
        void Start(std::unique_ptr<Event> event);
        void RunEventHandler();
        
        void HandleEvent(std::unique_ptr<Event> event);
        void GotoState(std::string state, std::unique_ptr<Event> event);

        void ExecuteCurrentStateOnEntry(std::unique_ptr<Event> event);
        void ExecuteCurrentStateOnExit();

        void Do(Action action, std::unique_ptr<Event> event);
        void PopState();

        void DoStatePush(MachineState* state);
        void DoStatePop();

        bool IsIgnored(std::string event);
        bool IsDeferred(std::string event);

        // Copy is disabled.
        Machine(const Machine& that) = delete;
        Machine &operator=(Machine const &) = delete;
    };
#pragma warning(pop)
} }

#endif // MICROSOFT_P3_MACHINE_H
