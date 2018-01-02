//-----------------------------------------------------------------------
// <copyright file="BugFindingRuntime.cpp">
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

#include "BugFindingRuntime.h"
#include "../Exceptions/ExecutionCanceledException.h"
#include "../TestingServices/Scheduling/BugFindingScheduler.h"
#include "P3/Actor.h"
#include "P3/Machine.h"
#include "P3/ActorId.h"
#include "P3/Runtime/AssertionFailureException.h"
#include "P3/Event.h"
#include <iostream>
#include <memory>
#include <string>

using namespace Microsoft::P3;
using namespace TestingServices;

// Creates a new runtime.
BugFindingRuntime::BugFindingRuntime(std::unique_ptr<Configuration> configuration, IExplorationStrategy* strategy)
    : Runtime(move(configuration))
{
    std::unique_ptr<BugFindingScheduler> scheduler(new BugFindingScheduler(Config.get(), strategy));
    m_scheduler = move(scheduler);
}

void BugFindingRuntime::Wait()
{
    m_scheduler->Wait();
    for (auto& task : m_actorTasks)
    {
        task.wait();
    }
}

void BugFindingRuntime::InitializeActor(Actor* actor, std::string name)
{
    // Insert a scheduling point.
    m_scheduler->Schedule();

    // Create a new unique id.
    std::unique_ptr<const ActorId> id(new ActorId(name, *this));
    Log("<CreateLog> Actor '" + id->m_name + "' is created.");
    m_actorMap[id->m_value] = std::unique_ptr<Actor>(actor);
    actor->SetActorId(std::move(id));
}

void BugFindingRuntime::InitializeMachine(Machine* machine, std::string name)
{
    // Insert a scheduling point.
    m_scheduler->Schedule();

    // Create a new unique id.
    std::unique_ptr<const ActorId> id(new ActorId(name, *this));
    Log("<CreateLog> Machine '" + id->m_name + "' is created.");
    m_actorMap[id->m_value] = std::unique_ptr<Actor>(machine);
    machine->SetActorId(std::move(id));
    machine->Initialize();
    machine->DoStatePush(machine->m_states[machine->startState].get());
}

void BugFindingRuntime::InitializeMonitor(Monitor* monitor, std::string name)
{
    Log("<MonitorLog> Monitor '" + name + "' is registered.");
    std::unique_ptr<Monitor> mptr(monitor);
    m_monitors.insert(move(mptr));
    monitor->Setup(name, *this);
    monitor->Initialize();
    monitor->GotoStartState(nullptr);
}

void BugFindingRuntime::SendEvent(const ActorId& target, std::unique_ptr<Event> event, const ActorId* sender)
{
    // Insert a scheduling point.
    m_scheduler->Schedule();

    auto actor = m_actorMap[target.m_value].get();

    if (sender != nullptr)
    {
        Log("<SendLog> '" + sender->m_name + "' sent event '" + event->m_name + "' to '" + target.m_name + "'.");
    }
    else
    {
        Log("<SendLog> Event '" + event->m_name + "' was sent to '" + target.m_name + "'.");
    }

    bool runNewHandler = false;
    EnqueueEvent(*actor, std::move(event), runNewHandler);
    if (runNewHandler)
    {
        RunEventHandler(*actor, nullptr, false);
    }
}

void BugFindingRuntime::InvokeMonitor(std::string name, std::unique_ptr<Event> event)
{
    Log("<MonitorLog> Monitor '" + name + "' invoked with event '" + event->m_name + "'.");

    for (auto& monitor : m_monitors)
    {
        if (monitor->m_name == name)
        {
            monitor->HandleEvent(std::move(event));
            return;
        }
    }

    Assert(false, "<MonitorLog> Invoking unregistered monitor '" + name + "'.");
}

inline
void BugFindingRuntime::EnqueueEvent(Actor& target, std::unique_ptr<Event> event, bool& runNewHandler)
{
    target.Enqueue(std::move(event), runNewHandler);
}

void BugFindingRuntime::RunEventHandler(Actor& actor, std::unique_ptr<Event> event, bool isFresh)
{
    m_scheduler->NotifyProcessCreated(actor.m_id->m_value);

    auto task = async([this](Actor& actor, std::unique_ptr<Event> event, bool isFresh)
    {
        try
        {
            m_scheduler->NotifyProcessStarted(actor.m_id->m_value);

            if (isFresh)
            {
                actor.Start(std::move(event));
            }

            actor.RunEventHandler();

            m_scheduler->NotifyProcessHalted(actor.m_id->m_value);
        }
        catch (const ExecutionCanceledException&)
        {
            // Ignore this exception, as it is bening.
        }
    }, std::ref(actor), std::move(event), isFresh);

    m_actorTasks.emplace_back(std::move(task));

    m_scheduler->WaitForProcessToStart(actor.m_id->m_value);
}

void BugFindingRuntime::Assert(bool predicate, const std::string& message)
{
    if (!predicate)
    {
        Log("<ErrorLog> " + message);
        m_scheduler->NotifyAssertionFailure(message);
    }
}

void BugFindingRuntime::Assert(bool predicate, std::ostringstream& stream)
{
    if (!predicate)
    {
        auto message = stream.str();
        Log("<ErrorLog> " + message);
        m_scheduler->NotifyAssertionFailure(message);
    }
}

inline
void BugFindingRuntime::NotifyEnteredState(Machine& machine)
{
    Log("<StateLog> '" + machine.m_id->m_name + "' enters state '" + machine.GetCurrentState() + "'.");
}

inline
void BugFindingRuntime::NotifyEnteredState(Monitor& monitor)
{
    Log("<MonitorLog> '" + monitor.m_name + "' enters state '" + monitor.GetCurrentState() + "'.");
}

inline
void BugFindingRuntime::NotifyExitedState(Machine& machine)
{
    Log("<StateLog> '" + machine.m_id->m_name + "' exits state '" + machine.GetCurrentState() + "'.");
}

inline
void BugFindingRuntime::NotifyExitedState(Monitor& monitor)
{
    Log("<MonitorLog> '" + monitor.m_name + "' exits state '" + monitor.GetCurrentState() + "'.");
}

inline
void BugFindingRuntime::NotifyInvokedAction(Machine& machine)
{
    Log("<ActionLog> '" + machine.m_id->m_name + "' invoked an action.");
}

inline
void BugFindingRuntime::NotifyInvokedAction(Monitor& monitor)
{
    Log("<MonitorLog> '" + monitor.m_name + "' invoked an action.");
}

inline
void BugFindingRuntime::NotifyRaisedEvent(Machine& machine, Event& event)
{
    Log("<RaiseLog> '" + machine.m_id->m_name + "' raised event '" + event.m_name + "'.");
}

inline
void BugFindingRuntime::NotifyRaisedEvent(Monitor& monitor, Event& event)
{
    Log("<MonitorLog> '" + monitor.m_name + "' raised event '" + event.m_name + "'.");
}

inline
void BugFindingRuntime::NotifyPoppedState(Machine& machine)
{
    Log("<PopLog> '" + machine.m_id->m_name + "' popped state '" + machine.GetCurrentState() + "'.");
}

BugFindingScheduler* BugFindingRuntime::GetScheduler()
{
    return m_scheduler.get();
}

BugFindingRuntime::~BugFindingRuntime() { }
