#ifndef MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_EVENTS_H
#define MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_EVENTS_H

#include "P3/Event.h"



using namespace Microsoft::P3;

class ConfigEvent : public Event
{
public:
    int pnum;
    const ActorId* id;
    ConfigEvent(int pn, const ActorId* ID) :
        Event("ConfigEvent"),
        pnum(pn),
        id(ID)
    { }

    ~ConfigEvent() { }
};

class ProcessConfigEvent: public Event
{
public:
	const ActorId* Id;
    ProcessConfigEvent(const ActorId* id):
        Event("ProcessConfigEvent"),
        Id(id)
    { }
    
    ~ProcessConfigEvent() {}
};

class Initialise: public Event
{
public:
	const ActorId* left;
	const ActorId* right;
    Initialise(const ActorId* l, const ActorId* r):
		Event("Initialise"),
        left(l),
        right(r)
    { }
    ~Initialise() { }
};

class Resp : public Event
{
public:
	Resp() :
		Event("Resp")
	{ }
	~Resp() { }
};


class Unit: public Event
{
public:
    Unit() :
		Event("Unit")
    { }
	
	~Unit() { }
};

class Req: public Event
{
public:
    Req():
		Event("Req")
    { }
	~Req() { }
};

class MyCount: public Event
{
public:
	int Count;
    MyCount(int c): 
		Event("MyCount"),
		Count(c)
    { }
	~MyCount() { }
};

#endif // MICROSOFT_P3_EXAMPLES_BOUNDEDASYNC_EVENTS_H