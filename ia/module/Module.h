#ifndef _MODULE_H_
#define _MODULE_H_

class Module
{
	public:
		int getState() const;
		const char * getName() const;

	protected:
		int state;

	private:
		int m_id;
		char * name;

#endif
