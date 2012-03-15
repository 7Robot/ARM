#ifndef _MODULE_H_
#define _MODULE_H_

#include <vector>

#include "Can.h"

class Module
{
	public:
		void setup(Can * can, const char * name);
		virtual void init();
		virtual void erase();
		int getState() const;
		const char * getName() const;
		~Module();

	protected:
		int state;
		void bind(can_callback);

	private:
		Can * m_can;
		int m_id;
		const char * m_name;
		std::vector<int> binding;
};

#endif
