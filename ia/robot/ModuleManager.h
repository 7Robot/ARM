#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include <set>

#include "Module.h"

class ModuleManager
{
	public:
		ModuleManager(const char * basedir, Can * can, Queue * queue);
		Module * load(const char * name);
		void unload(Module * module);

	private:
		std::set<Module *> modules;
		Queue * queue;
		Can * can;
		const char * basedir;
};

#endif
