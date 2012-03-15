#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include <set>

#include "Module.h"

class ModuleManager
{
	public:
		ModuleManager(Can * can, const char * basedir);
		Module * load(const char * mission);
		void unload(Module * module);

	private:
		std::set<Module *> modules;
		Can * can;
		const char * basedir;
};

#endif
