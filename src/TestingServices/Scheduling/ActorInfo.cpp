//-----------------------------------------------------------------------
// <copyright file="ActorInfo.cpp">
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

#include "ActorInfo.h"
#include <iostream>

using namespace Microsoft::P3;
using namespace TestingServices;

TestingServices::ActorInfo::ActorInfo(long id)
{
    Id = id;
    IsEnabled = true;
    IsActive = false;
    HasStarted = false;
    IsHalted = false;
}

TestingServices::ActorInfo::~ActorInfo() { }
