//-----------------------------------------------------------------------
// <copyright file="Configuration.h">
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

#ifndef MICROSOFT_P3_CONFIGURATION_H
#define MICROSOFT_P3_CONFIGURATION_H

#include "TestingServices/ExplorationStrategy.h"

namespace Microsoft { namespace P3
{
    // Configurations for the P3 runtime and tools.
    class Configuration final
    {
        friend class Machine;
        friend class ActorRuntime;
        friend class BugFindingRuntime;

    public:
        // Enables verbose output.
        int Verbosity;

        // Number of scheduling iterations.
        int SchedulingIterations;

        // Exploration strategy to be used during testing.
        TestingServices::ExplorationStrategy Strategy;

        static Configuration* Create();
        static Configuration* CopyFrom(const Configuration& that);
        
        ~Configuration();

    private:
        Configuration();

        Configuration(const Configuration& that) = delete;
        Configuration &operator=(Configuration const &) = delete;
    };
} }

#endif // MICROSOFT_P3_CONFIGURATION_H
