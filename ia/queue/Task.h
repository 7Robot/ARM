#ifndef _TASK_H_
#define _TASK_H_

#include "TaskBase.h"

template <typename T>
class Task: public TaskBase
{
	public:
		Task(T callback): m_callback(callback) {}

	private:
		T m_callback;

		void exec() {
			m_callback();
		}
};

#endif
