#ifndef MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_SCHEDULER_H
#define MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_SCHEDULER_H

#include "P3/Machine.h"
#include "P3/MachineState.h"
#include "P3/Actor.h"


#define P_MAX 10
using namespace Microsoft::P3;
class Scheduler : public Machine
{
protected:
    void Initialize()
    {
        
        auto initState=AddState("Init",true);
        initState->SetOnEventDoAction("ConfigEvent",std::bind(&Scheduler::initMembers,this,std::placeholders::_1));
		//initState->SetDeferredEvent("Req");
		//initState->SetDeferredEvent("Unit");
	
		
		auto syncState=AddState("Sync");
        syncState->SetOnExitAction(std::bind(&Scheduler::OnExit,this));
        syncState->SetOnEventDoAction("Req",std::bind(&Scheduler::CountReq,this));
        syncState->SetOnEventGotoState("Unit","Sync");
		//syncState->SetIgnoredEvent("ConfigEvent");

    }
protected:

    const ActorId* Processes[P_MAX];
	
    int Plen;
    int Count;
	
	void initMembers(std::unique_ptr<Event> e);
    void OnExit();
    void CountReq();
};
#endif //MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_SCHEDULER_H

