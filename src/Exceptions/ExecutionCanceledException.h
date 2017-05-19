//-----------------------------------------------------------------------
// <copyright file="ExecutionCanceledException.h">
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

#ifndef MICROSOFT_P3_EXCEPTIONS_EXECUTIONCANCELEDEXCEPTION_H
#define MICROSOFT_P3_EXCEPTIONS_EXECUTIONCANCELEDEXCEPTION_H

#include <exception>
#include <string>

namespace Microsoft { namespace P3
{
    // Exception that is thrown by the runtime when the execution is canceled.
    class ExecutionCanceledException : public std::exception { };
} }

#endif // MICROSOFT_P3_EXCEPTIONS_EXECUTIONCANCELEDEXCEPTION_H
