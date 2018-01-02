//#include "stdafx.h"
#include "Environment.h"
#include "Scheduler.h"
#include "Events.h"
#include <string>

void Environment::InitOnEntry()
{

	const ActorId* SchedulerId = CreateMachine<Scheduler>("Scheduler");
	Send(*(SchedulerId),std::make_unique<ConfigEvent>(3,SchedulerId));
	getchar();

}
