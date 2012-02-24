#include <QApplication>
#include <QtGui>
#include "FenPrincipale.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include <libcan.h>

using namespace libcan;

FenPrincipale * fenetre;

void canRecv(struct can_t packet)
{
	if (fenetre != NULL) {
		fenetre->canRecv(packet);
	}
}

int main(int argc, char *argv[])
{
	can_ctx * ctx;
	if (can_init(&ctx) < 0) {
		perror("can_init");
		exit(1);
	}

	//can_on_error(ctx, exit);


	if (can_register_callback(ctx, 0, 0, canRecv) < 0) {
		perror("can_register_callback");
		exit(1);
	}
	if (can_listen_on(ctx, STDIN_FILENO, libcan::dec) < 0) {
		perror("can_listen_on");
		exit(1);
	}
	
	QApplication app(argc, argv);

	fenetre = new FenPrincipale();
	fenetre->show();

	return app.exec();
}

