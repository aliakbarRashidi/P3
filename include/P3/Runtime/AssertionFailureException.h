//-----------------------------------------------------------------------
// <copyright file="AssertionFailureException.h">
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

#ifndef MICROSOFT_P3_RUNTIME_ASSERTIONFAILUREEXCEPTION_H
#define MICROSOFT_P3_RUNTIME_ASSERTIONFAILUREEXCEPTION_H

#include <exception>
#include <string>

namespace Microsoft { namespace P3
{
#pragma warning(push)
#pragma warning(disable: 4275)
    // Exception that is thrown by the runtime upon assertion failure.
    class AssertionFailureException : public std::logic_error
    {
    public:
        AssertionFailureException(const std::string& message)
            : std::logic_error(message) {
        }
    };
#pragma warning(pop)
} }

#endif // MICROSOFT_P3_RUNTIME_ASSERTIONFAILUREEXCEPTION_H
