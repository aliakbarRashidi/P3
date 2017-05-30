//-----------------------------------------------------------------------
// <copyright file="RandomStrategy.cpp">
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

#include "RandomStrategy.h"

using namespace Microsoft::P3;
using namespace TestingServices;

TestingServices::RandomStrategy::RandomStrategy()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::mt19937::min(), std::mt19937::max());
    m_seed = dis(gen);
    m_generator.seed(m_seed);
}

bool TestingServices::RandomStrategy::TryGetNext(ActorInfo*& next,
    std::vector<ActorInfo*> choices, ActorInfo& current)
{
    std::vector<ActorInfo*> enabled;
    for (auto& p : choices)
    {
        if (p->IsEnabled)
        {
            enabled.push_back(p);
        }
    }

    if (enabled.empty())
    {
        return false;
    }

    std::uniform_int_distribution<> dis(0, enabled.size());
    next = enabled[dis(m_generator)];

    return true;
}

bool TestingServices::RandomStrategy::GetNextBooleanChoice(int maxValue, bool& next)
{
    return false;
}

TestingServices::RandomStrategy::~RandomStrategy() { }
