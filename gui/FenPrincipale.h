#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtGui>
#include <QGraphicsScene>

#include <iostream>
#include <QTimer>

#include <libcan.h>

#define mapLargeur 648
#define mapHauteur 406

#define mapLargeur_int 606
#define mapHauteur_int 406

#define x0_def 70
#define	y0_def 70

class FenPrincipale : public QWidget
{
	Q_OBJECT
	
    public:
        FenPrincipale();
        void modifPosition(QGraphicsItem *robot,int x , int y , int theta );
        void canRecv(struct libcan::can_t packet);
    private slots:
      void timerOut();
        
    private: 
		QGraphicsItem *map;
		QGraphicsPixmapItem *robot;
		QTimer * timer ;
		QGraphicsItem *origine ;
		
};

#endif


