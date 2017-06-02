//-----------------------------------------------------------------------
// <copyright file="Runtime.cpp">
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
#include "P3/Runtime.h"
#include "P3/Machine.h"
#include "P3/ActorId.h"
#include "P3/Runtime\AssertionFailureException.h"
#include <iostream>
#include <memory>

using namespace Microsoft::P3;

Runtime* Runtime::Create()
{
    // Create a new runtime configuration.
    std::unique_ptr<Configuration> configuration(Configuration::Create());
    return new ActorRuntime(move(configuration));
}

Runtime* Runtime::Create(std::unique_ptr<Configuration> configuration)
{
    return new ActorRuntime(move(configuration));
}

// Creates a new runtime.
Runtime::Runtime(std::unique_ptr<Configuration> configuration)
{
    Config = move(configuration);
    m_idCounter = 0;
}

void Runtime::SendEvent(const ActorId& target, std::unique_ptr<Event> event)
{
    // If the event is null, then report an error.
    Assert(event != nullptr, "Cannot send a null event.");
    SendEvent(target, std::move(event), nullptr);
}

// Checks if the assertion holds, and if not it throws an exception.
void Runtime::Assert(bool predicate)
{
    if (!predicate)
    {
        Assert(predicate, "Detected an assertion failure.");
    }
}

inline
void Runtime::NotifyEnteredState(Machine& machine)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyEnteredState(Monitor& monitor)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyExitedState(Machine& machine)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyExitedState(Monitor& monitor)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyInvokedAction(Machine& machine)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyInvokedAction(Monitor& monitor)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyRaisedEvent(Machine& machine, Event& event)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyRaisedEvent(Monitor& monitor, Event& event)
{
    // Override to implement the notification.
}

inline
void Runtime::NotifyPoppedState(Machine& machine)
{
    // Override to implement the notification.
}

void Runtime::Log(const std::string& message)
{
    if (Config->Verbosity)
    {
        std::cout << message << std::endl;
    }
}

void Runtime::Log(std::ostringstream& stream)
{
    if (Config->Verbosity)
    {
        std::cout << stream.str() << std::endl;
    }
}

long Runtime::GetNextId()
{
    long id = _InterlockedIncrement(&m_idCounter);
    return id - 1;
}

Runtime::~Runtime() { }
