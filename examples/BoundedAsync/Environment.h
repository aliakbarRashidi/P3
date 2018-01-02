#pragma once
#ifndef MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_ENVIRONMENT_H
#define MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_ENVIRONMENT_H

#include "P3/Machine.h"

using namespace Microsoft::P3;

class Environment : public Machine
{
protected:
	void Initialize()
	{
		auto initState = AddState("Init", true);
		initState->SetOnEntryAction(std::bind(&Environment::InitOnEntry, this));
	}

protected:

	void InitOnEntry();
};
#endif //MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_ENVIRONMENT_H