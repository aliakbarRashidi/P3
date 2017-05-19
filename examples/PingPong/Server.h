//-----------------------------------------------------------------------
// <copyright file="Server.h">
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

#ifndef MICROSOFT_P3_EXAMPLES_PINGPONG_SERVER_H
#define MICROSOFT_P3_EXAMPLES_PINGPONG_SERVER_H

#include "P3/Event.h"
#include "P3/Actors/Machine.h"

using namespace Microsoft::P3;

class Server : public Machine
{
protected:
    void Initialize()
    {
        auto initState = AddState("Init", true);
        initState->SetOnEntryAction(std::bind(&Server::InitOnEntry, this));
        initState->SetOnEventGotoState("LocalEvent", "Active");

        auto activeState = AddState("Active");
        activeState->SetOnEventDoAction("PingEvent", std::bind(&Server::SendPong, this, std::placeholders::_1));
    }

private:
    void InitOnEntry();
    void SendPong(std::unique_ptr<Event> e);
};

#endif // MICROSOFT_P3_EXAMPLES_PINGPONG_SERVER_H
