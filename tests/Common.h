//-----------------------------------------------------------------------
// <copyright file="Common.h">
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

#ifndef MICROSOFT_P3_TESTS_COMMON_H
#define MICROSOFT_P3_TESTS_COMMON_H

#include "P3/Runtime.h"
#include "P3/TestingServices.h"
#include <memory>

struct Test
{
    static void Run(Microsoft::P3::TestingServices::TestAction testAction)
    {
        // Create a new testing configuration.
        std::unique_ptr<Microsoft::P3::Configuration> configuration(Microsoft::P3::Configuration::Create());
        configuration->SchedulingIterations = 1;

        Run(std::move(configuration), testAction);
    }

    static void Run(std::unique_ptr<Microsoft::P3::Configuration> configuration,
        Microsoft::P3::TestingServices::TestAction testAction)
    {
        // Create a new instance of the P3 bug-finding engine.
        std::unique_ptr<Microsoft::P3::TestingServices::BugFindingEngine> engine(
            Microsoft::P3::TestingServices::BugFindingEngine::Create(
            std::move(configuration), testAction));
        
        // Run the engine.
        engine->Run();
    }
};

#endif // MICROSOFT_P3_TESTS_COMMON_H
