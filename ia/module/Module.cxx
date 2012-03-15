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

Module::~Module()
{
	for_each(binding.begin(), binding.end(), [this](int bindid) {
		m_can->unbind(bindid);
	});
}

void Module::bind(can_callback cb)
{
	binding.push_back(m_can->bind(cb));
}
