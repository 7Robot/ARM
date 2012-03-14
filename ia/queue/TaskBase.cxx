#include "TaskBase.h"
#include <thread>

int TaskBase::id = 0;

TaskBase::TaskBase(): m_blocking(true),
	m_priority(0), m_delay(0), m_id(id++) {}

void TaskBase::operator()() {
	if (this->m_blocking) {
		this->exec();
	} else {
		std::thread thr(&TaskBase::threadedExec, this);
		thr.detach();
	}
}

void TaskBase::threadedExec()
{
	if (m_delay > 0) {
		usleep(m_delay * 1000);
	}
	this->exec();
}

void TaskBase::setPriority(int priority) {
	m_priority = priority;
}

int TaskBase::getPriority() const {
	return m_priority;
}

void TaskBase::setBlocking(bool blocking) {
	m_blocking = blocking;
}

void TaskBase::setDelay(int delay) {
	m_delay = delay;
}

int TaskBase::getId() const {
	return m_id;
}
