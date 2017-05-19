//-----------------------------------------------------------------------
// <copyright file="Server.cpp">
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

#include "Server.h"
#include "Events.h"

void Server::InitOnEntry()
{
    Raise(std::make_unique<LocalEvent>());
}

void Server::SendPong(std::unique_ptr<Event> e)
{
    PingEvent* payload = dynamic_cast<PingEvent*>(e.get());
    auto clientId = payload->Id;
    Send(*clientId, std::make_unique<PongEvent>());
}
