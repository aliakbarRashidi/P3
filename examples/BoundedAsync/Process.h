#ifndef MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_PROCESS
#define MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_PROCESS

#include "P3/Machine.h"
#include "P3/MachineState.h"
#include "P3/Actor.h"
#include <iostream>

using namespace Microsoft::P3;
class Process: public Machine
{
protected:
	void Initialize()
	{
		auto initState=AddState("Init",true);
		initState->SetOnEntryAction(std::bind(&Process::InitOnEntry,this,std::placeholders::_1));
		initState->SetOnEventDoAction("Initialise",std::bind(&Process::InitialiseAction,this,std::placeholders::_1));
		
		auto syncingState=AddState("Syncing");
		syncingState->SetOnEventDoAction("Resp",std::bind(&Process::Sync,this));
		syncingState->SetOnEventDoAction("MyCount",std::bind(&Process::ConfirmInSync,this,std::placeholders::_1));
		std::cout << "Initialised Process" << std::endl;
	}
protected:
	const ActorId* Scheduler;
	const ActorId* left;
	const ActorId* right;
	int Count;
	void InitOnEntry(std::unique_ptr<Event> e);
	void InitialiseAction(std::unique_ptr<Event> e);
	void Sync();
	void ConfirmInSync(std::unique_ptr<Event> e);

};
#endif ///MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_PROCESS
