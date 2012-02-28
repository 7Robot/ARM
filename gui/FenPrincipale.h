#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtGui>
#include <QGraphicsScene>

#include <iostream>
#include <QTimer>

#include <libcan.h>

#define mapLargeur 648
#define mapHauteur 406

#define mapLargeur_int 620
#define mapHauteur_int 420

#define x0_def 70
#define	y0_def 70

class FenPrincipale : public QWidget
{
	Q_OBJECT
	
    public:
        FenPrincipale();
        void modifPosition(QGraphicsItem *robot,int x , int y , int theta );//placer robot
        void canRecv(struct libcan::can_t packet);//recup message
        void testPosObs(QGraphicsItem *obst,int d,int angle);//placer obstacle
        
    private slots:
      void timerOut();//timer utile pour debug
        
    private: 
		QGraphicsItem *map;
		QGraphicsPixmapItem *robot;
		QGraphicsItem *obstacle,*obstacle2;
		
		QTimer * timer ;
		QGraphicsItem *origine ;
		
};

#endif


