//-----------------------------------------------------------------------
// <copyright file="BugFindingRuntime.h">
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

#ifndef MICROSOFT_P3_RUNTIME_BUGFINDINGRUNTIME_H
#define MICROSOFT_P3_RUNTIME_BUGFINDINGRUNTIME_H

#include "../TestingServices/Scheduling/BugFindingScheduler.h"
#include "../TestingServices/IExplorationStrategy.h"
#include "P3/Configuration.h"
#include "P3/Runtime.h"
#include <future>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace Microsoft { namespace P3
{
    class Event;

    // Runtime for executing actors for testing.
    class BugFindingRuntime : public Runtime
    {
    public:
        BugFindingRuntime(std::unique_ptr<Configuration> configuration, TestingServices::IExplorationStrategy* strategy);
        ~BugFindingRuntime();
        
        // Invokes the monitor with the specified name.
        void InvokeMonitor(std::string name, std::unique_ptr<Event> event);
        
        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, const std::string& message);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, std::ostringstream& stream);

        // Waits for the runtime to terminate execution.
        void Wait();

        // Returns the bug-finding scheduler
        TestingServices::BugFindingScheduler* GetScheduler();
        
    protected:
        // Initializes the specified actor.
        void InitializeActor(Actor* actor, std::string name);

        // Initializes the specified machine.
        void InitializeMachine(Machine* machine, std::string name);

        // Initializes the specified monitor.
        void InitializeMonitor(Monitor* monitor, std::string name);

        // Sends an asynchronous event to the specified machine.
        void SendEvent(const ActorId& target, std::unique_ptr<Event> event, const ActorId* sender);

        // Runs a new asynchronous event handler for the specified actor.
        void RunEventHandler(Actor& actor, std::unique_ptr<Event> event, bool isFresh);

        // Notifies that a machine entered a state.
        void NotifyEnteredState(Machine& machine);

        // Notifies that a monitor entered a state.
        void NotifyEnteredState(Monitor& monitor);

        // Notifies that a machine exited a state.
        void NotifyExitedState(Machine& machine);

        // Notifies that a monitor exited a state.
        void NotifyExitedState(Monitor& monitor);

        // Notifies that a machine invoked an action.
        void NotifyInvokedAction(Machine& machine);

        // Notifies that a monitor invoked an action.
        void NotifyInvokedAction(Monitor& monitor);

        // Notifies that a machine raised an event.
        void NotifyRaisedEvent(Machine& machine, Event& event);

        // Notifies that a monitor raised an event.
        void NotifyRaisedEvent(Monitor& monitor, Event& event);

        // Notifies that a machine popped its state.
        void NotifyPoppedState(Machine& machine);

    private:
        // Map from unique ids to actors.
        std::unordered_map<long, std::unique_ptr<Actor>> m_actorMap;

        // Set of registered monitors.
        std::set<std::unique_ptr<Monitor>> m_monitors;

        // Spawned actor tasks.
        std::vector<std::future<void>> m_actorTasks;

        // Bug-finding scheduler.
        std::unique_ptr<TestingServices::BugFindingScheduler> m_scheduler;

        // Enqueues an asynchronous event to the target actor.
        void EnqueueEvent(Actor& target, std::unique_ptr<Event> event, bool& runNewHandler);

        // Copy is disabled.
        BugFindingRuntime(const BugFindingRuntime& that) = delete;
        BugFindingRuntime &operator=(BugFindingRuntime const &) = delete;
    };
} }

#endif // MICROSOFT_P3_RUNTIME_BUGFINDINGRUNTIME_H
