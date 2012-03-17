#include "Module.h"

#include "queue/Queue.h"
#include "ModuleManager.h"

void Module::setup(Queue * queue, Can * can, ModuleManager * mh, const char * name)
{
	m_queue = queue;
	m_can = can;
	m_mh = mh;
	m_name = name;
}

void Module::init() {}

void Module::erase() {}

int Module::getState() const
{
	return state;
}

const char * Module::getName() const
{
	return m_name;
}

void Module::load(const char * name)
{
	printf("Module::load(%s): Not implemented yet !\n", name);
}

void Module::abord()
{
	//printf("Module::abord: Not implemented yet !\n");
	Task * t = new Task([this, m_mh]() {
			m_mh->unload(this);
	});
	m_queue->push(t);
}
	

Module::~Module()
{
	m_can->unbind(this);
}

Task * Module::send(can_packet packet)
{
	return m_can->push(packet);
}

void Module::bind(can_callback cb)
{
	m_can->bind(this, cb);
}
