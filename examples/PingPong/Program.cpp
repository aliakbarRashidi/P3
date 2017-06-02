//-----------------------------------------------------------------------
// <copyright file="Program.cpp">
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

#include "Environment.h"
#include "Safety.h"
#include "P3/Runtime.h"
#include "P3/TestingServices.h"
#include <iostream>

using namespace Microsoft::P3;
using namespace TestingServices;

struct Test
{
    static void Run()
    {
        // Create a new testing configuration.
        std::unique_ptr<Configuration> configuration(Configuration::Create());
        configuration->SchedulingIterations = 2;
        configuration->Verbosity = true;

        // Create a new instance of the P3 bug-finding engine.
        std::unique_ptr<BugFindingEngine> engine(BugFindingEngine::Create(
            move(configuration),
            [](Runtime& runtime)
        {
            // Create the environment (test harness) machine.
            runtime.RegisterMonitor<Safety>("Safety");
            runtime.CreateMachine<Environment>("Environment");
        }));
        
        // Run the engine.
        engine->Run();
    }
};

int main(int argc, char* argv[])
{
    // Check if the program should be tested.
    if (argc == 2)
    {
        std::string mode(argv[1]);
        if (mode == "test")
        {
            Test::Run();
            return 0;
        }
    }

    // Create a new runtime configuration.
    std::unique_ptr<Configuration> configuration(Configuration::Create());
    configuration->Verbosity = true;

    // Create a new instance of the P3 runtime.
    std::unique_ptr<Runtime> runtime(Runtime::Create(move(configuration)));

    // Create the environment (test harness) machine.
    runtime->CreateMachine<Environment>("Environment");

    // Wait for user-input as the P3 runtime runs asynchronously.
    getchar();

    return 0;
}
