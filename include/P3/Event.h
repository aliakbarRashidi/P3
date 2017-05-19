//-----------------------------------------------------------------------
// <copyright file="Event.h">
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

#ifndef MICROSOFT_P3_EVENT_H
#define MICROSOFT_P3_EVENT_H

#include <string>

namespace Microsoft { namespace P3
{
    // Abstract class representing an event.
    class Event
    {
        friend class Actor;
        friend class Machine;
        friend class Monitor;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        virtual ~Event() = 0;

    protected:
        Event(std::string name);

    private:
#pragma warning(push)
#pragma warning(disable: 4251)
        // The event name.
        std::string m_name;
#pragma warning(pop)
    };
} }

#endif // MICROSOFT_P3_EVENT_H
