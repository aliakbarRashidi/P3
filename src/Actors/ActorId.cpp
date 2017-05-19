//-----------------------------------------------------------------------
// <copyright file="ActorId.cpp">
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

#include "P3/Actors/ActorId.h"
#include "P3/Runtime.h"
#include <string>

using namespace Microsoft::P3;

ActorId::ActorId(std::string friendlyName, Runtime& runtime) :
    m_runtime(&runtime),
    m_value(runtime.GetNextId()),
    m_name(friendlyName.empty() ? std::to_string(m_value) : (friendlyName + "(" + std::to_string(m_value) + ")"))
{ }

std::string ActorId::GetName() const
{
    return m_name;
}

const Runtime* ActorId::GetRuntime()
{
    return m_runtime;
}
