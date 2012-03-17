#ifndef _MODULE_H_
#define _MODULE_H_

#include <vector>

#include "Can.h"

#define BIND0(command, callback) bind(recv::command(std::bind(&callback, this))) 
#define BIND1(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1))) 
#define BIND2(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2))) 
#define BIND3(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))) 
#define BIND4(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))) 
#define BIND5(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))) 
#define BIND6(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6))) 
#define BIND7(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7))) 
#define BIND8(command, callback) bind(recv::command(std::bind(&callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8)))

class Module
{
	public:
		void setup(Can * can, const char * name);
		virtual void init();
		virtual void erase();
		int getState() const;
		const char * getName() const;

	protected:
		int state;
		Task * send(can_packet);
		void bind(can_callback);
		void load(const char * name);
		void abord();

	private:
		Can * m_can;
		int m_id;
		const char * m_name;
};

#endif
