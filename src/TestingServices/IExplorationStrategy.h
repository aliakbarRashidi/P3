//-----------------------------------------------------------------------
// <copyright file="IExplorationStrategy.h">
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

#ifndef MICROSOFT_P3_TESTINGSERVICES_IEXPLORATIONSTRATEGY_H
#define MICROSOFT_P3_TESTINGSERVICES_IEXPLORATIONSTRATEGY_H

#include "Scheduling/ActorInfo.h"
#include <memory>
#include <vector>

namespace Microsoft { namespace P3 { namespace TestingServices
{
    // Interface of a generic exploration strategy.
    class IExplorationStrategy
    {
    public:
        IExplorationStrategy() { }
        virtual ~IExplorationStrategy() = 0 { }

        // Returns the next process to schedule.
        virtual bool TryGetNext(ActorInfo*& next, std::vector<ActorInfo*> choices, ActorInfo& current) = 0;

        // Returns the next boolean choice.
        virtual bool GetNextBooleanChoice(int maxValue, bool& next) = 0;

    private:
        // Copy is disabled.
        IExplorationStrategy(const IExplorationStrategy& that) = delete;
        IExplorationStrategy &operator=(IExplorationStrategy const &) = delete;
    };
} } }

#endif // MICROSOFT_P3_TESTINGSERVICES_IEXPLORATIONSTRATEGY_H
