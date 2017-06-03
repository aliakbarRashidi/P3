//-----------------------------------------------------------------------
// <copyright file="BugFindingEngine.h">
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

#ifndef MICROSOFT_P3_TESTINGSERVICES_BUGFINDINGENGINE_H
#define MICROSOFT_P3_TESTINGSERVICES_BUGFINDINGENGINE_H

#include "Statistics/TestReport.h"
#include "P3/Configuration.h"
#include "P3/Runtime.h"
#include <functional>
#include <memory>

namespace Microsoft { namespace P3 { namespace TestingServices
{
    class IExplorationStrategy;

    // Type of a machine action.
    typedef std::function<void(Runtime&)> TestAction;

    // The P3 bug-finding engine.
    class BugFindingEngine
    {
    public:
        // Creates a new bug-finding engine.
        static BugFindingEngine* Create(std::unique_ptr<Configuration> configuration, TestAction);

        // Runs the engine.
        void Run();

        // Returns the generated test report.
        TestReport* GetReport();

        ~BugFindingEngine();

    private:
        // The runtime and testing configuration.
        std::unique_ptr<Configuration> m_configuration;

        // The exploration strategy used during testing.
        std::unique_ptr<IExplorationStrategy> m_strategy;

        // The test report.
        std::unique_ptr<TestReport> m_report;
        
        // The entry point to the test.
        TestAction m_testAction;

        BugFindingEngine(std::unique_ptr<Configuration> configuration, TestAction);

        void Initialize();        
        void RunNextIteration(int iteration);

        void Log(const std::string& message);

        // Copy is disabled.
        BugFindingEngine(const BugFindingEngine& that) = delete;
        BugFindingEngine &operator=(BugFindingEngine const &) = delete;
    };
} } }

#endif // MICROSOFT_P3_TESTINGSERVICES_BUGFINDINGENGINE_H
