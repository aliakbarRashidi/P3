//-----------------------------------------------------------------------
// <copyright file="ActorInfo.h">
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

#ifndef MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_PROCESSINFO_H
#define MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_PROCESSINFO_H

#include <condition_variable>
#include <mutex>

namespace Microsoft { namespace P3 { namespace TestingServices
{
    // Contains process related information that is used for scheduling purposes.
    // In this context, a process is an abstract representation of a concurrent
    // entity (e.g. a machine or an actor).
    class ActorInfo
    {
        friend class BugFindingScheduler;

    public:
        // Unique id.
        long Id;
        bool IsEnabled;
        bool IsActive;
        bool HasStarted;
        bool IsHalted;

        ActorInfo(long id);
        ~ActorInfo();

    private:
        std::mutex m_lock;
        std::condition_variable m_cv;

        // Copy is disabled.
        ActorInfo(const ActorInfo& that) = delete;
        ActorInfo &operator=(ActorInfo const &) = delete;
    };
} } }

#endif // MICROSOFT_P3_TESTINGSERVICES_SCHEDULING_PROCESSINFO_H
