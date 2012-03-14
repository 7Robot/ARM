#ifndef _TASK_H_
#define _TASK_H_

class Task
{
	public:
		Task();
		void operator()();
		virtual void exec() = 0;

		void setDelay(int delay);
		void setPriority(int priority);
		int getPriority() const;
		void setAutofree(bool autofree);
		void setBlocking(bool blocking);
		int getId() const;

	private:
		void threadedExec();

		int m_priority;
		bool m_autofree;
		bool m_blocking;
		int m_id;
		int m_delay;
		static int id;
};

#endif
