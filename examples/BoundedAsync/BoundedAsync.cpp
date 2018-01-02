// BoundedAsync.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"


#include "P3\Runtime.h"
#include "P3\Machine.h"
#include "Scheduler.h"
#include "Events.h"
#include "Environment.h"
#include <iostream>

using namespace Microsoft::P3;

int main()
{


	// Create a new runtime configuration.
	std::unique_ptr<Configuration> configuration(Configuration::Create());
	configuration->Verbosity = true;

	// Create a new instance of the P3 runtime.
	std::unique_ptr<Runtime> runtime(Runtime::Create(std::move(configuration)));

	const ActorId* EnvironmentId = runtime->CreateMachine<Environment>("Environment");
	

	// Wait for user-input as the P3 runtime runs asynchronously.
	getchar();

	return 0;
}
