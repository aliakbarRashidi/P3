//-----------------------------------------------------------------------
// <copyright file="ActorId.h">
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

#ifndef MICROSOFT_P3_ACTORS_UNIQUEID_H
#define MICROSOFT_P3_ACTORS_UNIQUEID_H

#include <string>

namespace Microsoft { namespace P3
{
    // Unique actor id.
    class ActorId
    {
        friend class Actor;
        friend class Machine;
        friend class MachineState;
        friend class Runtime;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        // Returns the name of the actor with this id.
        std::string GetName() const;

        // Returns the runtime that executes the actor with this id.
        const Runtime* GetRuntime();

    private:
        // Unique id value.
        const long m_value;

#pragma warning(push)
#pragma warning(disable: 4251)
        // Name of the actor with this id.
        const std::string m_name;
#pragma warning(pop)

        // The runtime that executes the actor with this id.
        Runtime* m_runtime;

        ActorId(std::string friendlyName, Runtime& runtime);

        // Copy is disabled.
        ActorId(const ActorId& that) = delete;
        ActorId &operator=(ActorId const &) = delete;
    };
} }

#endif // MICROSOFT_P3_ACTORS_UNIQUEID_H
