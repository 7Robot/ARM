#include "Task.h"
#include <thread>

int Task::id = 0;

Task::Task(std::function<void (void)> callback):
	m_blocking(true), m_callback(callback),
	m_priority(0), m_delay(0), m_id(id++) {}

void Task::operator()() {
	if (this->m_blocking) {
		this->exec();
	} else {
		std::thread thr(&Task::exec, this);
		thr.detach();
	}
}

void Task::exec()
{
	if (m_delay > 0) {
		usleep(m_delay * 1000);
	}
	m_callback();
}

void Task::setPriority(int priority) {
	m_priority = priority;
}

int Task::getPriority() const {
	return m_priority;
}

void Task::setBlocking(bool blocking) {
	m_blocking = blocking;
}

void Task::setDelay(int delay) {
	m_delay = delay;
}

int Task::getId() const {
	return m_id;
}
