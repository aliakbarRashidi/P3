#include "stdafx.h"
#include "Scheduler.h"
#include "Events.h"
#include "Process.h"



void Scheduler::initMembers(std::unique_ptr<Event> e)
{
	
    ConfigEvent* payload = dynamic_cast<ConfigEvent*>(e.get());
    Assert(payload!=nullptr,"dymanic casting ConfigEvent");
	
    int processNumber=payload->pnum;

    Assert(processNumber>=3 && processNumber<=P_MAX,"3 processes");

    this->Plen=processNumber;
    
    const ActorId* Schedulerid=payload->id;
    for(int i=0;i<processNumber;i++)
    {
    	this->Processes[i]=CreateMachine<Process>("Process",std::make_unique<ProcessConfigEvent>(Schedulerid));
    }

    for(int i=0;i<processNumber;i++)
    {
    	int left=i-1,right=i+1;
    	if(i==0)
    	{
    		left=processNumber-1;
    	}
    	if(i==processNumber-1)
    	{
    		right=0;
    	}
    	Send(*(this->Processes[i]),std::make_unique<Initialise>(this->Processes[left],this->Processes[right]));
    }
    this->Count=0;

    Jump("Sync");
    
}



void Scheduler::OnExit()
{
	for(int i=0;i<this->Plen;i++)
	{
		Send(*(this->Processes[i]),std::make_unique<Resp>());
	}
}



void Scheduler::CountReq()
{
	this->Count++;
	if(this->Count==this->Plen)
	{
		this->Count=0;
		Raise(std::make_unique<Unit>());
	}
}