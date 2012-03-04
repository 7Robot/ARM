#include "Task.h"

#include <stdio.h>
#include <pthread.h>

int Task::id = 0;

Task::Task(): m_priority(50), m_autofree(true), m_blocking(true), m_id(id++) {}

void Task::operator()()
{
	if (this->m_blocking) {
		this->exec();
	} else {
		pthread_t pth;
		pthread_create(&pth, NULL, Task::thread, (void *) this);
	}
}

void * Task::thread(void * arg)
{
	Task * task = (Task *) arg;
	task->exec();
	return NULL;
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
