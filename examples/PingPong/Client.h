//-----------------------------------------------------------------------
// <copyright file="Client.h">
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

#ifndef MICROSOFT_P3_EXAMPLES_PINGPONG_CLIENT_H
#define MICROSOFT_P3_EXAMPLES_PINGPONG_CLIENT_H

#include "P3/Event.h"
#include "P3/Machine.h"

using namespace Microsoft::P3;

class Client : public Machine
{
protected:
    void Initialize()
    {
        auto initState = AddState("Init", true);
        initState->SetOnEntryAction(std::bind(&Client::InitOnEntry, this, std::placeholders::_1));
        initState->SetOnEventGotoState("LocalEvent", "Active");

        auto activeState = AddState("Active");
        activeState->SetOnEntryAction(std::bind(&Client::ActiveOnEntry, this));
        activeState->SetOnEventDoAction("PongEvent", std::bind(&Client::SendPing, this));
    }

private:
    const ActorId* _serverId;
    int _counter;

    void InitOnEntry(std::unique_ptr<Event> e);
    void ActiveOnEntry();
    void SendPing();
};

#endif // MICROSOFT_P3_EXAMPLES_PINGPONG_CLIENT_H
