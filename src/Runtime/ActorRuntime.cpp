//-----------------------------------------------------------------------
// <copyright file="ActorRuntime.cpp">
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

#include "ActorRuntime.h"
#include "P3/Actor.h"
#include "P3/Machine.h"
#include "P3/ActorId.h"
#include "P3/Runtime/AssertionFailureException.h"
#include "P3/Event.h"
#include <iostream>

using namespace Microsoft::P3;

// Creates a new runtime.
ActorRuntime::ActorRuntime(std::unique_ptr<Configuration> configuration)
    : Runtime(move(configuration))
{ }

void ActorRuntime::InvokeMonitor(std::string name, std::unique_ptr<Event> event)
{
    // No-op for production.
}

void ActorRuntime::Wait()
{
    // No-op for production.
    // Maybe not expose it to the user?
}

void ActorRuntime::InitializeActor(Actor* actor, std::string name)
{
    std::unique_ptr<const ActorId> id(new ActorId(name, *this));
    Log("<CreateLog> Actor '" + id->m_name + "' is created.");
    m_actorMap[id->m_value] = std::unique_ptr<Actor>(actor);
    actor->SetActorId(move(id));
}

void ActorRuntime::InitializeMachine(Machine* machine, std::string name)
{
    std::unique_ptr<const ActorId> id(new ActorId(name, *this));
    Log("<CreateLog> Machine '" + id->m_name + "' is created.");
    m_actorMap[id->m_value] = std::unique_ptr<Actor>(machine);
    machine->SetActorId(move(id));
    machine->Initialize();
}

void ActorRuntime::InitializeMonitor(Monitor* monitor, std::string name)
{
    // No-op for production.
    delete monitor;
}

void ActorRuntime::SendEvent(const ActorId& target, std::unique_ptr<Event> event, const ActorId* sender)
{
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

inline
void ActorRuntime::EnqueueEvent(Actor& target, std::unique_ptr<Event> event, bool& runNewHandler)
{
    target.Enqueue(std::move(event), runNewHandler);
}

void ActorRuntime::RunEventHandler(Actor& actor, std::unique_ptr<Event> event, bool isFresh)
{
    auto task = std::async(std::launch::async, [](Actor& actor, std::unique_ptr<Event> event, bool isFresh)
    {
        if (isFresh)
        {
            actor.Start(move(event));
        }

        actor.RunEventHandler();
    }, std::ref(actor), std::move(event), isFresh);

    m_actorTasks.emplace_back(std::move(task));
}

void ActorRuntime::Assert(bool predicate, const std::string& message)
{
    if (!predicate)
    {
        Log("<ErrorLog> " + message);
        throw AssertionFailureException(message);
    }
}

void ActorRuntime::Assert(bool predicate, std::ostringstream& stream)
{
    if (!predicate)
    {
        auto message = stream.str();
        Log("<ErrorLog> " + message);
        throw AssertionFailureException(message);
    }
}

inline
void ActorRuntime::NotifyEnteredState(Machine& machine)
{
    Log("<StateLog> '" + machine.m_id->m_name + "' enters state '" + machine.GetCurrentState() + "'.");
}

inline
void ActorRuntime::NotifyExitedState(Machine& machine)
{
    Log("<StateLog> '" + machine.m_id->m_name + "' exits state '" + machine.GetCurrentState() + "'.");
}

inline
void ActorRuntime::NotifyInvokedAction(Machine& machine)
{
    Log("<ActionLog> '" + machine.m_id->m_name + "' invoked an action.");
}

inline
void ActorRuntime::NotifyRaisedEvent(Machine& machine, Event& event)
{
    Log("<RaiseLog> '" + machine.m_id->m_name + "' raised event '" + event.m_name + "'.");
}

inline
void ActorRuntime::NotifyPoppedState(Machine& machine)
{
    Log("<PopLog> '" + machine.m_id->m_name + "' popped state '" + machine.GetCurrentState() + "'.");
}

ActorRuntime::~ActorRuntime() { }
