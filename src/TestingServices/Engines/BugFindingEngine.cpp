//-----------------------------------------------------------------------
// <copyright file="BugFindingEngine.cpp">
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

#include "P3/TestingServices/BugFindingEngine.h"
#include "P3/TestingServices/ExplorationStrategy.h"
#include "../ExplorationStrategies/RandomStrategy.h"
#include "../../Runtime/BugFindingRuntime.h"
#include <iostream>
#include <string>

using namespace Microsoft::P3;
using namespace TestingServices;

BugFindingEngine* TestingServices::BugFindingEngine::Create(std::unique_ptr<Configuration> configuration,
    TestAction action)
{
    return new BugFindingEngine(move(configuration), action);
}

TestingServices::BugFindingEngine::BugFindingEngine(std::unique_ptr<Configuration> configuration,
    TestAction action)
{
    m_configuration = move(configuration);
    m_report = std::make_unique<TestReport>();
    m_testAction = action;
    Initialize();
}

// Initializes the engine.
void TestingServices::BugFindingEngine::Initialize()
{
    if (m_configuration->Strategy == ExplorationStrategy::Random)
    {
        // Use the random scheduling strategy.
        std::unique_ptr<RandomStrategy> strategy(new RandomStrategy());
        m_strategy = move(strategy);
    }
}

void TestingServices::BugFindingEngine::Run()
{
    Log(". Testing started");

    int maxIterations = m_configuration->SchedulingIterations;
    for (int i = 0; i < maxIterations; i++)
    {
        // Runs a new testing iteration.
        RunNextIteration(i);

        if (m_report->NumOfFoundBugs > 0)
        {
            break;
        }
    }
    
    Log(". Done");
}

void TestingServices::BugFindingEngine::RunNextIteration(int iteration)
{
    Log("... Iteration #" + std::to_string(iteration + 1));

    // Copy the configuration to pass it to the runtime.
    std::unique_ptr<Configuration> configuration(Configuration::CopyFrom(*(m_configuration.get())));
    
    // Creates a new instance of the bug-finding runtime.
    std::unique_ptr<BugFindingRuntime> runtime(new BugFindingRuntime(move(configuration), m_strategy.get()));

    // Run the test.
    m_testAction(*(runtime.get()));

    // Wait for the runtime to terminate execution.
    runtime->Wait();

    if (runtime->GetScheduler()->BugFound)
    {
        m_report->NumOfFoundBugs++;
        Log("..... Iteration #" + std::to_string(iteration + 1) + " triggered bug #" +
            std::to_string(m_report->NumOfFoundBugs));
    }
}

TestReport* TestingServices::BugFindingEngine::GetReport()
{
    return m_report.get();
}

void TestingServices::BugFindingEngine::Log(const std::string& message)
{
    if (m_configuration->ToolVerbosity)
    {
        std::cout << message << std::endl;
    }
}

TestingServices::BugFindingEngine::~BugFindingEngine() { }
