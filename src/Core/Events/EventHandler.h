//-----------------------------------------------------------------------
// <copyright file="EventHandler.h">
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

#ifndef MICROSOFT_P3_EVENTHANDLER_H
#define MICROSOFT_P3_EVENTHANDLER_H

namespace Microsoft { namespace P3
{
    // An abstract event action handler.
    class EventHandler
    {
        friend class Machine;

    public:
        EventHandler() { }
        ~EventHandler() { }

    private:
        enum class Type
        {
            Action = 0,
            Ignore,
            Defer
        };

        Type m_type;
        Action m_action;
    };
} }

#endif // MICROSOFT_P3_EVENTHANDLER_H
