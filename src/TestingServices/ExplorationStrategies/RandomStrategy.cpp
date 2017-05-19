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
    
}

bool TestingServices::RandomStrategy::TryGetNext(ActorInfo*& next,
    std::vector<ActorInfo*> choices, ActorInfo& current)
{
    //TODO: use strategy.
    for (auto& p : choices)
    {
        if (p->IsEnabled)
        {
            next = p;
            break;
        }
    }

    if (next == nullptr)
    {
        return false;
    }

    return true;
}

bool TestingServices::RandomStrategy::GetNextBooleanChoice(int maxValue, bool& next)
{
    return false;
}

TestingServices::RandomStrategy::~RandomStrategy() { }
