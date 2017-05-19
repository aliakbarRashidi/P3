//-----------------------------------------------------------------------
// <copyright file="ActorRuntime.h">
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

#ifndef MICROSOFT_P3_RUNTIME_ACTORRUNTIME_H
#define MICROSOFT_P3_RUNTIME_ACTORRUNTIME_H

#include "P3/Runtime.h"
#include <future>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace Microsoft { namespace P3
{
    class Event;

    // Runtime for executing actors in production.
    class ActorRuntime : public Runtime
    {
    public:
        ActorRuntime(std::unique_ptr<Configuration> configuration);
        ~ActorRuntime();

        // Invokes the monitor with the specified name.
        void InvokeMonitor(std::string name, std::unique_ptr<Event> event);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, const std::string& message);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, std::ostringstream& stream);

        // Waits for the runtime to terminate execution.
        void Wait();
        
    protected:
        // Initializes the specified actor.
        void InitializeActor(Actor* actor, std::string name);

        // Initializes the specified machine.
        void InitializeMachine(Machine* machine, std::string name);

        // Initializes the specified monitor.
        void InitializeMonitor(Monitor* monitor, std::string name);

        // Sends an asynchronous event to the specified actor.
        void SendEvent(const ActorId& target, std::unique_ptr<Event> event, const ActorId* sender);

        // Runs a new asynchronous event handler for the specified actor.
        void RunEventHandler(Actor& actor, std::unique_ptr<Event> event, bool isFresh);

        // Notifies that a machine entered a state.
        void NotifyEnteredState(Machine& machine);

        // Notifies that a machine exited a state.
        void NotifyExitedState(Machine& machine);

        // Notifies that a machine invoked an action.
        void NotifyInvokedAction(Machine& machine);

        // Notifies that a machine raised an event.
        void NotifyRaisedEvent(Machine& machine, Event& event);

    private:
        // Map from unique ids to actor.
        std::unordered_map<long, std::unique_ptr<Actor>> m_actorMap;

        // Spawned actor tasks.
        std::vector<std::future<void>> m_actorTasks;

        // Enqueues an asynchronous event to the target actor.
        void EnqueueEvent(Actor& target, std::unique_ptr<Event> event, bool& runNewHandler);

        // Copy is disabled.
        ActorRuntime(const ActorRuntime& that) = delete;
        ActorRuntime &operator=(ActorRuntime const &) = delete;
    };
} }

#endif // MICROSOFT_P3_RUNTIME_ACTORRUNTIME_H
