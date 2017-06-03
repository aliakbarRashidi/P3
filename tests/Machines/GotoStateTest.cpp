//-----------------------------------------------------------------------
// <copyright file="GotoStateTest.cpp">
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

#include "../Framework/catch.hpp"
#include "../Common.h"
#include "P3/Machine.h"

using namespace Microsoft::P3;

class M : public Machine
{
protected:
    void Initialize()
    {
        auto initState = AddState("Init", true);
        initState->SetOnEntryAction(std::bind(&M::InitOnEntry, this));

        AddState("Done");
    }

private:
    void InitOnEntry()
    {
        Jump("Done");
    }
};

TEST_CASE("Machine is transitioning to new state", "[GotoStateTest]")
{
    std::unique_ptr<Microsoft::P3::Configuration> configuration(Microsoft::P3::Configuration::Create());
    configuration->SchedulingIterations = 1;
    // configuration->Verbosity = true;
    
    Test::Run(std::move(configuration), [](Runtime& runtime)
        {
            runtime.CreateMachine<M>("M");
        });
    // REQUIRE(1 == 1);
}
