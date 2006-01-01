#include <stdio.h>
#include <unistd.h>

#include "Demo.h"

void Demo::init()
{
	printf("demo init\n");
}

void Demo::cb_asserv(int erreur)
{
	printf("asserv, erreur = %d\n", erreur);
}

void Demo::run()
{
	printf("run");
	can->rotate(180);
	pause();
}

extern "C" {
	Mission * create() {
		return new Demo();
	}
}
