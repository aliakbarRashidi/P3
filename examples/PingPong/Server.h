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
#include "P3/Actors/Actor.h"

using namespace Microsoft::P3;

class Server : public Actor
{
protected:
    void HandleEvent(std::unique_ptr<Event> event);
};

#endif // MICROSOFT_P3_EXAMPLES_PINGPONG_SERVER_H
