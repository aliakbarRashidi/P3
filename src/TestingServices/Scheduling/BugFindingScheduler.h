//-----------------------------------------------------------------------
// <copyright file="BugFindingScheduler.h">
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

#ifndef MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_BUGFINDINGSCHEDULER_H
#define MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_BUGFINDINGSCHEDULER_H

#include "ActorInfo.h"
#include "../IExplorationStrategy.h"
#include "P3/Configuration.h"
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Microsoft { namespace P3 { namespace TestingServices
{
    // Bug-finding scheduler for the P3 runtime.
    class BugFindingScheduler
    {
    public:
        // Checks if the scheduler is running.
        bool IsSchedulerRunning;

        // Checks if the schedule has been fully explored.
        bool HasFullyExploredSchedule;

        // True if a bug was found.
        bool BugFound;

        BugFindingScheduler(Configuration* config, IExplorationStrategy* strategy);
        ~BugFindingScheduler();

        // Schedules the next machine to execute.
        void Schedule();
        
        // Returns the next nondeterministic boolean choice.
        bool GetNextNondeterministicBooleanChoice(int maxValue);
        
        // Notify that an assertion has failed.
        void NotifyAssertionFailure(std::string text);

        // Notify that a process has been created.
        void NotifyProcessCreated(long id);
        
        // Notify that a process has started.
        void NotifyProcessStarted(long id);

        // Notify that the process has paused.
        void NotifyProcessPaused(long id);
        
        // Notify that the process has halted.
        void NotifyProcessHalted(long id);

        // Wait for the task to start.
        void WaitForProcessToStart(long id);

        // Blocks until the scheduler terminates.
        void Wait();

        // Stops the scheduler and terminates execution.
        void Stop();

    private:
        // The installed configuration.
        Configuration* m_config;

        // The exploration strategy to be used for bug-finding.
        IExplorationStrategy* m_strategy;

        // Map from unique ids to actor infos.
        std::unordered_map<long, std::unique_ptr<ActorInfo>> m_actorMap;

        // The info of the currently scheduled process.
        ActorInfo* m_scheduledProcessInfo;

        // Completes when the scheduler terminates.
        std::promise<void> m_completionSource;

        std::vector<ActorInfo*> GetProcessInfos();
        void KillRemainingProcesses();

        // Copy is disabled.
        BugFindingScheduler(const BugFindingScheduler& that) = delete;
        BugFindingScheduler &operator=(BugFindingScheduler const &) = delete;
    };
} } }

#endif // MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_BUGFINDINGSCHEDULER_H
