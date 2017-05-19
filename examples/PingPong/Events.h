//-----------------------------------------------------------------------
// <copyright file="Events.h">
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

#ifndef MICROSOFT_P3_EXAMPLES_PINGPONG_EVENTS_H
#define MICROSOFT_P3_EXAMPLES_PINGPONG_EVENTS_H

#include "P3/Event.h"

using namespace Microsoft::P3;

class ConfigEvent : public Event
{
public:
    const ActorId* Id;

    ConfigEvent(const ActorId* id) :
        Event("ConfigEvent"),
        Id(id)
    { }

    ~ConfigEvent() { }
};

class PingEvent : public Event
{
public:
    const ActorId* Id;

    PingEvent(const ActorId* id) :
        Event("PingEvent"),
        Id(id)
    { }

    ~PingEvent() { }
};

class PongEvent : public Event
{
public:
    PongEvent() : Event("PongEvent") { }
    ~PongEvent() { }
};

class LocalEvent : public Event
{
public:
    LocalEvent() : Event("LocalEvent") { }
    ~LocalEvent() { }
};

class NotificationEvent : public Event
{
public:
    NotificationEvent() : Event("NotificationEvent") { }
    ~NotificationEvent() { }
};

#endif // MICROSOFT_P3_EXAMPLES_PINGPONG_EVENTS_H
