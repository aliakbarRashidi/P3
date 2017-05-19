//-----------------------------------------------------------------------
// <copyright file="Actor.h">
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

#ifndef MICROSOFT_P3_ACTORS_ACTOR_H
#define MICROSOFT_P3_ACTORS_ACTOR_H

#include "P3/Event.h"
#include <list>
#include <memory>
#include <mutex>
#include <string>

namespace Microsoft { namespace P3
{
    class ActorId;

    // Abstract class representing an actor.
    class Actor
    {
        friend class Machine;
        friend class MachineState;
        friend class Monitor;
        friend class MonitorState;
        friend class Runtime;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        virtual ~Actor() = 0;

    protected:
        // The runtime that executes the actor with this id.
        Runtime* Runtime;

        Actor();

        // Handles the specified event.
        virtual void HandleEvent(std::unique_ptr<Event> event) = 0;

        // Invokes the monitor with the specified name.
        void InvokeMonitor(std::string name, std::unique_ptr<Event> event);

    private:
        // The unique id.
        std::unique_ptr<const ActorId> m_id;

#pragma warning(push)
#pragma warning(disable: 4251)
        // Inbox of the actor. Incoming events are queued here.
        // Events are dequeued to be processed.
        std::list<std::unique_ptr<Event>> m_inbox;
#pragma warning(pop)

        // Protects the inbox.
        std::mutex m_inboxLock;

        // Is the actor running.
        bool m_isRunning;

        // Is the actor halted.
        bool m_isHalted;
        
        void Enqueue(std::unique_ptr<Event> event, bool& runNewHandler);
        std::unique_ptr<Event> GetNextEvent();
        
        virtual void Start(std::unique_ptr<Event> event);
        virtual void RunEventHandler();
        
        // Sets the unique id of this actor.
        void SetActorId(std::unique_ptr<const ActorId> id);
        
        // Copy is disabled.
        Actor(const Actor& that) = delete;
        Actor &operator=(Actor const &) = delete;
    };
} }

#endif // MICROSOFT_P3_ACTORS_ACTOR_H
