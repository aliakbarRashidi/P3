//-----------------------------------------------------------------------
// <copyright file="RandomStrategy.h">
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

#ifndef MICROSOFT_P3_TESTINGSERVICES_EXPLORATIONSTRATEGIES_RANDOMSTRATEGY_H
#define MICROSOFT_P3_TESTINGSERVICES_EXPLORATIONSTRATEGIES_RANDOMSTRATEGY_H

#include "../IExplorationStrategy.h"
#include <memory>
#include <random>
#include <vector>

namespace Microsoft { namespace P3 { namespace TestingServices
{
    // Random-walk scheduling strategy.
    class RandomStrategy : public IExplorationStrategy
    {
    public:
        RandomStrategy();
        ~RandomStrategy();

        // Returns the next process to schedule.
        bool TryGetNext(ActorInfo*& next, std::vector<ActorInfo*> choices, ActorInfo& current);

        // Returns the next boolean choice.
        bool GetNextBooleanChoice(int maxValue, bool& next);
            
    private:
        // Seed used during this iteration.
        size_t m_seed;

        // Random integer generator.
        std::mt19937 m_generator;

        // Copy is disabled.
        RandomStrategy(const RandomStrategy& that) = delete;
        RandomStrategy &operator=(RandomStrategy const &) = delete;
    };
} } }

#endif // MICROSOFT_P3_TESTINGSERVICES_EXPLORATIONSTRATEGIES_RANDOMSTRATEGY_H
