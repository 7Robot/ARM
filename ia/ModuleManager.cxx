#include "ModuleManager.h"

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

ModuleManager::ModuleManager(Can * can, const char * bd):
	can(can), basedir(bd) {}

Module * ModuleManager::load(const char * name)
{
	if (name == NULL)
		return NULL;

	void * hndl = NULL;
	char path[256];

	if (hndl == NULL) { // try "libname.so"
		sprintf(path, "lib%s.so", name);
		hndl = dlopen(path, RTLD_LAZY);
	}
	if (hndl == NULL) { // try "basedir/libname.so"
		sprintf(path, "%s/lib%s.so", basedir, name);
		hndl = dlopen(path, RTLD_LAZY);
	}
	if (hndl == NULL) {
		fprintf(stderr, "dlopen failed(%s)\n", dlerror());
		return NULL;
	}

	Module * (*create)() = (Module*(*)())dlsym(hndl, "create");
	if (create == NULL) {
		fprintf(stderr, "dlsym failed (%s)\n", dlerror());
		return NULL;
	}

	Module * module = (*create)();
	if (module == NULL) {
		fprintf(stderr, "Create module failed.\n");
		return NULL;
	}

	module->setup(can, name);
	module->init();
	modules.insert(module);

	printf("LOADED MODULES: %3d [+ %s]\n", modules.size(), module->getName());
	
	return module;
}

void ModuleManager::unload(Module * module)
{
	modules.erase(module);
	printf("LOADED MODULES: %3d [— %s]\n", modules.size(), module->getName());
	delete module;
}
