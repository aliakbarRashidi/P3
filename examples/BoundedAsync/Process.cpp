#include "stdafx.h"
#include "Process.h"
#include "Events.h"

void Process::InitOnEntry(std::unique_ptr<Event> e)
{
	ProcessConfigEvent* payload=dynamic_cast<ProcessConfigEvent*>(e.get());
	Assert(payload!=nullptr,"dymanic casting ProcessConfigEvent");
	this->Scheduler=payload->Id;	
	this->Count=0;
}

void Process::InitialiseAction(std::unique_ptr<Event> e)
{
	Initialise* payload=dynamic_cast<Initialise*>(e.get());
	Assert(payload!=nullptr,"dymanic casting Initialise");
	this->left=payload->left;
	this->right=payload->right;
	Send(*(this->Scheduler),std::make_unique<Req>());
	Jump("Syncing");
}

void Process::Sync()
{
	this->Count++;
	Send(*(this->left),std::make_unique<MyCount>(this->Count));
	Send(*(this->right),std::make_unique<MyCount>(this->Count));
	Send(*(this->Scheduler),std::make_unique<Req>());
	if(this->Count==10)
	{
		
		//halt machine here.. TO-DO How to set m_halted=true ?
		for (; true;); //workaround
	}

}

void Process::ConfirmInSync(std::unique_ptr<Event> e)
{
	MyCount* payload=dynamic_cast<MyCount*>(e.get());
	Assert(payload!=nullptr,"dymanic casting MyCount");
	int cnt=payload->Count;
	Assert((this->Count == cnt || this->Count == cnt - 1),"Received wrong count! ");
}