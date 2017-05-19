//-----------------------------------------------------------------------
// <copyright file="Monitor.h">
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

#ifndef MICROSOFT_P3_ACTORS_MONITOR_H
#define MICROSOFT_P3_ACTORS_MONITOR_H

#include "Action.h"
#include "MonitorState.h"
#include "P3/Event.h"
#include "P3/Runtime.h"
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace Microsoft { namespace P3
{
#pragma warning(push)
#pragma warning(disable: 4275)
    // Abstract class representing a monitor.
    class Monitor
    {
        friend class MonitorState;
        friend class Runtime;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        virtual ~Monitor() = 0;

    protected:
        Monitor();
        
        // Initializes the monitor.
        virtual void Initialize() = 0;

        // Raises an event internally at the end of the current action.
        void Raise(std::unique_ptr<Event> event);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, const std::string& message);

        // Checks if the assertion holds, and if not it throws an exception.
        void Assert(bool predicate, std::ostringstream& stream);

        // Returns the current monitor state.
        std::string GetCurrentState();

        // Adds a state with the specified name to the monitor.
        MonitorState* AddState(std::string name, bool isStart = false);

    private:
        // The runtime that executes this monitor.
        Runtime* m_runtime;

#pragma warning(push)
#pragma warning(disable: 4251)
        // Name of this monitor.
        std::string m_name;
        
        // Available states of this monitor.
        std::map<std::string, std::unique_ptr<MonitorState>> m_states;

        // The currently installed monitor state.
        MonitorState* m_currentState;

        // Map containing events to goto state transitions for the currently installed state.
        std::map<std::string, std::string> m_gotoTransitions;

        // Map containing events to action bindings for the currently installed state.
        std::map<std::string, Action> m_actionBindings;

        // Gets the raised event. If no event has been raised this will return null.
        std::unique_ptr<Event> m_raisedEvent;
#pragma warning(pop)

        void Setup(std::string name, Runtime& runtime);

        void HandleEvent(std::unique_ptr<Event> event);

        void GotoStartState(std::unique_ptr<Event> event);
        void GotoState(std::string state, std::unique_ptr<Event> event);

        void ExecuteCurrentStateOnEntry(std::unique_ptr<Event> event);
        void ExecuteCurrentStateOnExit();

        void Do(Action action, std::unique_ptr<Event> event);

        void DoStatePush(MonitorState& state);
        void DoStatePop();

        // Copy is disabled.
        Monitor(const Monitor& that) = delete;
        Monitor &operator=(Monitor const &) = delete;
    };
#pragma warning(pop)
} }

#endif // MICROSOFT_P3_ACTORS_MONITOR_H
