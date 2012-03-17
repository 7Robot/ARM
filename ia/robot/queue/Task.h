#ifndef _TASK_H_
#define _TASK_H_

#include <algorithm>

class Task
{
	public:
		Task(std::function<void (void)> callback);

		void operator()();

		void setPriority(int priority);
		int getPriority() const;
		void setBlocking(bool blocking);
		void setDelay(int delay);
		int getId() const;
	
	private:
		bool m_blocking;
		int m_priority;
		int m_delay;
		int m_id;
		std::function<void (void)> m_callback;
		static int id;

		void exec();
};

#endif
