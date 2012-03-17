#include "Module.h"

void Module::setup(Can * can, const char * name)
{
	m_can = can;
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
	printf("Module::abord: Not implemented yet !\n");
}
	

Module::~Module()
{
	for_each(binding.begin(), binding.end(), [this](int bindid) {
		m_can->unbind(bindid);
	});
}

Task * Module::send(can_packet packet)
{
	return m_can->push(packet);
}

void Module::bind(can_callback cb)
{
	binding.push_back(m_can->bind(cb));
}
