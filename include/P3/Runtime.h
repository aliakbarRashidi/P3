//-----------------------------------------------------------------------
// <copyright file="Runtime.h">
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

#ifndef MICROSOFT_P3_RUNTIME_H
#define MICROSOFT_P3_RUNTIME_H

#include "Configuration.h"
#include "Event.h"
#include "Actors/Actor.h"
#include "Actors/Machine.h"
#include "Actors/Monitor.h"
#include <memory>
#include <sstream>
#include <string>

namespace Microsoft { namespace P3
{
    class Event;

    // Runtime for executing actors.
    class Runtime
    {
        friend class Actor;
        friend class Machine;
        friend class Monitor;
        friend class ActorId;

    public:
        // Creates a new actor runtime.
        static Runtime* Create();
        
        // Creates a new actor runtime with the specified configuration.
        static Runtime* Create(std::unique_ptr<Configuration> configuration);

        // Creates a new actor of the specified type.
        template<typename T>
        const ActorId* CreateActor(std::string name, std::unique_ptr<Event> event = nullptr)
        {
            Assert(std::is_base_of<Actor, T>::value && !std::is_base_of<Machine, T>::value, "Type is not an actor.");
            auto actor = new T();
            Assert(actor != nullptr, "Failed to create actor '" + name + "'.");
            InitializeActor(actor, name);
            RunEventHandler(*actor, move(event), true);
            return actor->m_id.get();
        }

        // Creates a new machine of the specified type.
        template<typename T>
        const ActorId* CreateMachine(std::string name, std::unique_ptr<Event> event = nullptr)
        {
            Assert(std::is_base_of<Machine, T>::value, "Type is not a machine.");
            auto machine = new T();
            Assert(machine != nullptr, "Failed to create machine '" + name + "'.");
            InitializeMachine(machine, name);
            RunEventHandler(*machine, move(event), true);
            return machine->m_id.get();
        }

        // Registers a monitor of the specified type.
        template<typename T>
        void RegisterMonitor(std::string name)
        {
            Assert(std::is_base_of<Monitor, T>::value, "Type is not a monitor.");
            auto monitor = new T();
            Assert(monitor != nullptr, "Failed to create monitor '" + name + "'.");
            InitializeMonitor(monitor, name);
        }

        // Sends an asynchronous event to the target.
        void SendEvent(const ActorId& target, std::unique_ptr<Event> event);

        // Invokes the monitor with the specified name.
        virtual void InvokeMonitor(std::string name, std::unique_ptr<Event> event) = 0;

        // Logs the specified message.
        virtual void Log(const std::string& message);

        // Logs the specified message.
        virtual void Log(std::ostringstream& stream);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate);

        // Checks if the assertion holds, and if not it throws an exception.
        virtual void Assert(bool predicate, const std::string& message) = 0;

        // Checks if the assertion holds, and if not it throws an exception.
        virtual void Assert(bool predicate, std::ostringstream& stream) = 0;

        // Waits for the runtime to terminate execution.
        virtual void Wait() = 0;

        virtual ~Runtime() = 0;

    protected:
        // The installed configuration.
        std::unique_ptr<Configuration> Config;

        Runtime();
        Runtime(std::unique_ptr<Configuration> configuration);

        // Initializes the specified actor.
        virtual void InitializeActor(Actor* actor, std::string name) = 0;

        // Initializes the specified machine.
        virtual void InitializeMachine(Machine* machine, std::string name) = 0;

        // Initializes the specified monitor.
        virtual void InitializeMonitor(Monitor* monitor, std::string name) = 0;

        // Runs a new asynchronous event handler for the specified actor.
        virtual void RunEventHandler(Actor& actor, std::unique_ptr<Event> event, bool isFresh) = 0;

        // Sends an asynchronous event to the specified actor.
        virtual void SendEvent(const ActorId& target, std::unique_ptr<Event> event, const ActorId* sender) = 0;

        // Notifies that a machine entered a state.
        virtual void NotifyEnteredState(Machine& machine);

        // Notifies that a monitor entered a state.
        virtual void NotifyEnteredState(Monitor& monitor);

        // Notifies that a machine exited a state.
        virtual void NotifyExitedState(Machine& machine);

        // Notifies that a monitor exited a state.
        virtual void NotifyExitedState(Monitor& monitor);

        // Notifies that a machine invoked an action.
        virtual void NotifyInvokedAction(Machine& machine);

        // Notifies that a monitor invoked an action.
        virtual void NotifyInvokedAction(Monitor& monitor);

        // Notifies that a machine raised an event.
        virtual void NotifyRaisedEvent(Machine& machine, Event& event);

        // Notifies that a monitor raised an event.
        virtual void NotifyRaisedEvent(Monitor& monitor, Event& event);

        // Notifies that a machine popped its state.
        virtual void NotifyPoppedState(Machine& machine);

    private:
        // Monotonically increasing id counter.
        volatile long m_idCounter;

        // Returns the next unique id.
        long GetNextId();

        // Copy is disabled.
        Runtime(const Runtime& that) = delete;
        Runtime &operator=(Runtime const &) = delete;
    };
} }

#endif // MICROSOFT_P3_RUNTIME_H
