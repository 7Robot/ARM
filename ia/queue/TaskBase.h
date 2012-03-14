#ifndef _TASK_BASE_H_
#define _TASK_BASE_H_

class TaskBase
{
	public:
		TaskBase();

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
		static int id;

		void threadedExec();
		virtual void exec() = 0;
};

#endif
