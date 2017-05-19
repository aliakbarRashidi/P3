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

#ifndef MICROSOFT_P3_ACTORS_MACHINE_H
#define MICROSOFT_P3_ACTORS_MACHINE_H

#include "Actor.h"
#include "Action.h"
#include "MachineState.h"
#include "P3/Event.h"
#include "P3/Runtime.h"
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace Microsoft { namespace P3
{
    class ActorId;

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

        // Creates a new machine of the specified type.
        template<typename T>
        const ActorId* CreateMachine(std::string name, std::unique_ptr<Event> event = nullptr)
        {
            return Runtime->CreateMachine<T>(name, move(event));
        }

        // Sends an asynchronous event to the target machine.
        void Send(const ActorId& target, std::unique_ptr<Event> event);

        // Raises an event internally at the end of the current action.
        void Raise(std::unique_ptr<Event> event);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, const std::string& message);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, std::ostringstream& stream);

        // Returns the unique id of this machine.
        const ActorId* GetId();

        // Returns the current machine state.
        std::string GetCurrentState();

        // Adds a state with the specified name to the machine.
        MachineState* AddState(std::string name, bool isStart = false);

    private:
#pragma warning(push)
#pragma warning(disable: 4251)
        // Available states of this machine.
        std::map<std::string, std::unique_ptr<MachineState>> m_states;

        // The currently installed machine state.
        MachineState* m_currentState;

        // Map containing events to goto state transitions for the currently installed state.
        std::map<std::string, std::string> m_gotoTransitions;

        // Map containing events to push state transitions for the currently installed state.
        std::map<std::string, std::string> _pushTransitions;

        // Map containing events to action bindings for the currently installed state.
        std::map<std::string, Action> m_actionBindings;

        // Gets the raised event. If no event has been raised this will return null.
        std::unique_ptr<Event> m_raisedEvent;
#pragma warning(pop)

        std::unique_ptr<Event> GetNextEvent(bool& isDequeued);
        
        void Start(std::unique_ptr<Event> event);
        void RunEventHandler();
        
        void HandleEvent(std::unique_ptr<Event> event);
        void GotoState(std::string state, std::unique_ptr<Event> event);

        void ExecuteCurrentStateOnEntry(std::unique_ptr<Event> event);
        void ExecuteCurrentStateOnExit();

        void Do(Action action, std::unique_ptr<Event> event);

        void DoStatePush(MachineState& state);
        void DoStatePop();

        // Copy is disabled.
        Machine(const Machine& that) = delete;
        Machine &operator=(Machine const &) = delete;
    };
#pragma warning(pop)
} }

#endif // MICROSOFT_P3_ACTORS_MACHINE_H
