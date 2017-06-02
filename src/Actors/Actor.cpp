//-----------------------------------------------------------------------
// <copyright file="Actor.cpp">
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

#include "P3/Actors/Actor.h"
#include "P3/Actors/ActorId.h"
#include "P3/Runtime.h"
#include <iostream>
#include <memory>

using namespace Microsoft::P3;

Actor::Actor()
{
    m_isRunning = true;
    m_isHalted = false;
}

void Actor::Send(const ActorId& target, std::unique_ptr<Event> event)
{
    // If the event is null, then report an error.
    Runtime->Assert(event != nullptr, "Cannot send a null event.");
    Runtime->SendEvent(target, std::move(event), m_id.get());
}

void Actor::InvokeMonitor(std::string name, std::unique_ptr<Event> event)
{
    Runtime->InvokeMonitor(name, move(event));
}

void Actor::Log(const std::string& message)
{
    Runtime->Log(message);
}

void Actor::Log(std::ostringstream& stream)
{
    Runtime->Log(stream);
}

void Actor::Assert(bool predicate)
{
    if (!predicate)
    {
        Runtime->Assert(predicate);
    }
}

void Actor::Assert(bool predicate, const std::string& message)
{
    Runtime->Assert(predicate, message);
}

void Actor::Assert(bool predicate, std::ostringstream& stream)
{
    Runtime->Assert(predicate, stream);
}

const ActorId* Actor::GetId()
{
    return m_id.get();
}

// Enqueues an asynchronous event to an actor.
void Actor::Enqueue(std::unique_ptr<Event> event, bool& runNewHandler)
{
    // Lock the queue to avoid data races.
    std::lock_guard<std::mutex> lock(m_inboxLock);

    if (m_isHalted)
    {
        // If the actor has halted, do nothing.
        return;
    }
    
    Runtime->Log("<EnqueueLog> '" + m_id->m_name + "' enqueued event '" + event->m_name + "'.");

    // Inserts the event into the inbox queue.
    m_inbox.push_back(std::move(event));

    if (!m_isRunning)
    {
        // If the actor is not running, then ask to run a new event handler.
        m_isRunning = true;
        runNewHandler = true;
    }
}

// Gets the next available event. It returns a null event if no event is available.
std::unique_ptr<Event> Actor::GetNextEvent()
{
    std::unique_ptr<Event> nextEvent = nullptr;
    if (m_inbox.size() > 0)
    {
        nextEvent = std::move(m_inbox.front());
        m_inbox.pop_front();
    }

    return nextEvent;
}

// Starts the actor with the specified event.
void Actor::Start(std::unique_ptr<Event> event)
{
    if (event)
    {
        bool ignore;
        Enqueue(move(event), ignore);
    }
}

// Runs the event handler. The handler terminates if there is
// no next event to process or if the actor is halted.
void Actor::RunEventHandler()
{
    if (m_isHalted)
    {
        // If the actor has halted, do nothing.
        return;
    }

    std::unique_ptr<Event> nextEvent = nullptr;
    while (!m_isHalted)
    {
        {
            // Lock the queue to avoid data races.
            std::lock_guard<std::mutex> lock(m_inboxLock);
            nextEvent = GetNextEvent();
        }

        // Check if next event to process is null.
        if (nextEvent == nullptr)
        {
            m_isRunning = false;
            break;
        }

        // Handle the next event.
        HandleEvent(std::move(nextEvent));
    }
}

void Actor::SetActorId(std::unique_ptr<const ActorId> id)
{
    Runtime = id->m_runtime;
    m_id = std::move(id);
}

Actor::~Actor() { }
