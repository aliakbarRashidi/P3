//-----------------------------------------------------------------------
// <copyright file="Configuration.cpp">
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

#include "P3/Configuration.h"

using namespace Microsoft::P3;
using namespace TestingServices;

Configuration* Configuration::Create()
{
    return new Configuration();
}

Configuration* Configuration::CopyFrom(const Configuration& that)
{
    auto copy = new Configuration();
    copy->SchedulingIterations = that.SchedulingIterations;
    copy->Strategy = that.Strategy;
    return copy;
}

Configuration::Configuration()
{
    SchedulingIterations = 1;
    Strategy = ExplorationStrategy::Random;
}

Configuration::~Configuration() { }
