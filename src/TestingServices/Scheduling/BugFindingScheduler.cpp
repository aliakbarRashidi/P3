//-----------------------------------------------------------------------
// <copyright file="BugFindingScheduler.cpp">
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

#include "BugFindingScheduler.h"
#include "ActorInfo.h"
#include "../../Exceptions/ExecutionCanceledException.h"
#include <exception>
#include <future>
#include <iostream>

using namespace Microsoft::P3;
using namespace TestingServices;

// Creates a new runtime.
TestingServices::BugFindingScheduler::BugFindingScheduler(Configuration* config, IExplorationStrategy* strategy)
{
    m_config = config;
    m_strategy = strategy;
    IsSchedulerRunning = true;
    HasFullyExploredSchedule = false;
    BugFound = false;
}

void TestingServices::BugFindingScheduler::Schedule()
{
    // std::cout << "================" << std::endl;
    // std::cout << "scheduling point" << std::endl;
    // std::cout << "================" << std::endl;
    if (m_actorMap.empty())
    {
        // If this is the first scheduling point, then return.
        return;
    }

    if (!IsSchedulerRunning)
    {
        Stop();
        return;
    }

    auto current = m_scheduledProcessInfo;
    ActorInfo* next = nullptr;
    if (!m_strategy->TryGetNext(next, GetProcessInfos(), *current))
    {
        if (m_config->Verbosity)
        {
            std::cout << "<ScheduleLog> Schedule explored." << std::endl;
        }
        
        HasFullyExploredSchedule = true;
        Stop();
        return;
    }
    
    m_scheduledProcessInfo = next;

    // std::cout << "previous: " << current->Id << " :: " << std::this_thread::get_id() << std::endl;
    // std::cout << "scheduling: " << next->Id << " :: " << std::this_thread::get_id() << std::endl;

    // Only a single process should be running at a time during
    // testing. For this reason we serialize the execution.
    if (current->Id != next->Id)
    {
        current->IsActive = false;

        {
            // Wakes up the next scheduled process.
            std::lock_guard<std::mutex> nextLock(next->m_lock);
            next->IsActive = true;
            // std::cout << "unlocking: " << next->Id << " :: " << std::this_thread::get_id() << std::endl;
            next->m_cv.notify_all();
            // std::cout << "unlocked: " << next->Id << " :: " << std::this_thread::get_id() << std::endl;
        }

        {
            // Waits to be scheduled again.
            std::unique_lock<std::mutex> currentLock(current->m_lock);
            // std::cout << "sleeping: " << current->Id << " :: " << std::this_thread::get_id() << std::endl;
            current->m_cv.wait(currentLock, [current]{ return current->IsActive; });
            // std::cout << "woke up: " << current->Id << " :: " << std::this_thread::get_id() << std::endl;
        }

        if (!current->IsEnabled)
        {
            throw ExecutionCanceledException();
        }
    }
}

bool TestingServices::BugFindingScheduler::GetNextNondeterministicBooleanChoice(int maxValue)
{
    bool choice = false;
    if (!m_strategy->GetNextBooleanChoice(maxValue, choice))
    {
        std::cout << "<ScheduleDebug> Schedule explored." << std::endl;
        HasFullyExploredSchedule = true;
        Stop();
    }

    return choice;
}

void TestingServices::BugFindingScheduler::NotifyAssertionFailure(std::string text)
{
    if (!BugFound)
    {
        BugFound = true;
    }
    
    Stop();
}

void TestingServices::BugFindingScheduler::NotifyProcessCreated(long id)
{
    // Check if process has already been created.
    if (m_actorMap.find(id) != m_actorMap.end())
    {
        auto process = m_actorMap[id].get();

        // std::cout << "=======================" << std::endl;
        // std::cout << "process found: " << id  << " :: " << std::this_thread::get_id() << std::endl;
        // std::cout << "=======================" << std::endl;
        
        process->IsEnabled = true;
        process->IsHalted = false;
    }
    else
    {
        // Create a new process and insert it in the map.
        auto info = std::make_unique<ActorInfo>(id);

        // std::cout << "=======================" << std::endl;
        // std::cout << "process created: " << id  << " :: " << std::this_thread::get_id() << std::endl;
        // std::cout << "=======================" << std::endl;

        if (m_actorMap.empty())
        {
            // If this is the first process, then schedule it.
            m_scheduledProcessInfo = info.get();
        }

        m_actorMap.insert(std::make_pair(id, move(info)));
    }
}

void TestingServices::BugFindingScheduler::NotifyProcessStarted(long id)
{
    auto process = m_actorMap[id].get();

    {
        // Wakes up the creator process (which is the currently scheduled process).
        std::lock_guard<std::mutex> lock(process->m_lock);

        // std::cout << "====================" << std::endl;
        // std::cout << "process started: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        // std::cout << "====================" << std::endl;

        // std::cout << "NotifyProcessStarted: unlocking: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        process->HasStarted = true;
        process->m_cv.notify_all();
        // std::cout << "NotifyProcessStarted: unlocked: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    }

    {
        // Waits to be scheduled again.
        std::unique_lock<std::mutex> lock(process->m_lock);

        // std::cout << "NotifyProcessStarted: wait: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        process->m_cv.wait(lock, [process] { return process->IsActive; });
        // std::cout << "NotifyProcessStarted: done: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    }

    if (!process->IsEnabled)
    {
        throw ExecutionCanceledException();
    }
}

void TestingServices::BugFindingScheduler::NotifyProcessPaused(long id)
{
    auto process = m_scheduledProcessInfo;
}

void TestingServices::BugFindingScheduler::NotifyProcessHalted(long id)
{
    auto process = m_scheduledProcessInfo;

    // std::cout << "=================================" << std::endl;
    // std::cout << "process halted: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    // std::cout << "=================================" << std::endl;

    process->IsEnabled = false;
    process->IsHalted = true;
    Schedule();
}

void TestingServices::BugFindingScheduler::WaitForProcessToStart(long id)
{
    auto process = m_actorMap[id].get();
    if (m_actorMap.size() == 1)
    {
        // Wakes up the recently created process.
        std::lock_guard<std::mutex> lock(process->m_lock);

        // std::cout << "=================================" << std::endl;
        // std::cout << "waiting for process to start: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        // std::cout << "=================================" << std::endl;

        // std::cout << "WaitForProcessToStart: unlocking: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        process->IsActive = true;
        process->m_cv.notify_all();
        // std::cout << "WaitForProcessToStart: unlocked: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    }
    else
    {
        // Waits until the recently created process has started.
        std::unique_lock<std::mutex> lock(process->m_lock);

        // std::cout << "=================================" << std::endl;
        // std::cout << "waiting for process to start: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        // std::cout << "=================================" << std::endl;

        // std::cout << "WaitForProcessToStart: wait: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        process->m_cv.wait(lock, [process] { return process->HasStarted; });
        // std::cout << "WaitForProcessToStart: done: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    }
}

void TestingServices::BugFindingScheduler::Wait()
{
    m_completionSource.get_future().wait();
    // std::cout << "DONE WAITING"  << " :: " << std::this_thread::get_id() << std::endl;
}

void TestingServices::BugFindingScheduler::Stop()
{
    IsSchedulerRunning = false;
    KillRemainingProcesses();
    m_completionSource.set_value();
    throw ExecutionCanceledException();
}

std::vector<ActorInfo*> TestingServices::BugFindingScheduler::GetProcessInfos()
{
    // TODO: Make this ordered.
    std::vector<ActorInfo*> processInfos;
    for (auto& p : m_actorMap)
    {
        processInfos.push_back(p.second.get());
    }

    return processInfos;
}

/// <summary>
/// Kills any remaining machines at the end of the schedule.
/// </summary>
void TestingServices::BugFindingScheduler::KillRemainingProcesses()
{
    for (auto& kvp : m_actorMap)
    {
        auto process = kvp.second.get();
        // std::cout << "checking: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;

        process->IsActive = true;
        process->IsEnabled = false;
        process->IsHalted = true;
        
        // Wakes up the process.
        std::lock_guard<std::mutex> lock(process->m_lock);
        // std::cout << "unlocking: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
        process->m_cv.notify_all();
        // std::cout << "unlocked: " << process->Id  << " :: " << std::this_thread::get_id() << std::endl;
    }
}

TestingServices::BugFindingScheduler::~BugFindingScheduler()
{
    // std::cout << "BugFindingScheduler: cleanup" << std::endl;
    KillRemainingProcesses();
}
