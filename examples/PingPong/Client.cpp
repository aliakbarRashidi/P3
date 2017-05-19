//-----------------------------------------------------------------------
// <copyright file="Client.cpp">
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

#include "Client.h"
#include "Events.h"
#include <iostream>

using namespace std;

void Client::InitOnEntry(unique_ptr<Event> e)
{
    ConfigEvent* payload = dynamic_cast<ConfigEvent*>(e.get());
    _serverId = payload->Id;
    _counter = 0;
    Raise(make_unique<LocalEvent>());
}

void Client::ActiveOnEntry()
{
    SendPing();
}

void Client::SendPing()
{
    // InvokeMonitor("Safety", make_unique<NotificationEvent>());
    // Assert(false, "test test");
    if (_counter < 5)
    {
        cout << "=======================" << "." << endl;
        cout << "=== sending ping #" << _counter << " ===." << endl;
        cout << "=======================" << endl;
        Send(*(_serverId), make_unique<PingEvent>(GetId()));
        _counter++;
    }
}
