#include "Task.h"

#include <stdio.h>
#include <thread>

int Task::id = 0;

Task::Task():
	m_priority(50), m_autofree(true), m_blocking(true),
	m_id(id++), m_delay(0) {}

void Task::operator()()
{
	if (this->m_blocking) {
		this->exec();
	} else {
		std::thread thr(&Task::threadedExec, this);
		thr.detach(); // Detach thread otherwise processus quit when thread terminate
	}
}

void Task::threadedExec()
{
	if (m_delay > 0) {
		usleep(m_delay * 1000);
	}
	this->exec();
}

void Task::setDelay(int delay)
{
	m_blocking = false;
	m_delay = delay;
}

void Task::setPriority(int priority)
{
	m_priority = priority;
}

int Task::getPriority() const
{
	return m_priority;
}

void Task::setAutofree(bool autofree)
{
	m_autofree = autofree;
}

void Task::setBlocking(bool blocking)
{
	m_blocking = blocking;
}

int Task::getId() const
{
	return m_id;
}
