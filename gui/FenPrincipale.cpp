#include "FenPrincipale.h"

extern FenPrincipale * fenetre;

using namespace std;
FenPrincipale::FenPrincipale()
{				
       // setFixedSize(1200,800); // Dimensions fixées à 850x480px
		
		QGridLayout *layout0 = new QGridLayout; // c le layout de base de la page1
		QGraphicsScene *scene =  new QGraphicsScene; 
		QGraphicsView *vue = new QGraphicsView(scene,this);
	
		scene->setSceneRect(0,0,mapLargeur+50,mapHauteur+50); // taille de la scene
		vue->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);    
		vue->setFixedSize(mapLargeur+100,mapHauteur+100); //taille de la vue
    
		//Mise en place de la map 
		QPixmap map_im ; 
		map_im.load("map.png");
		map=scene->addPixmap(map_im);
		map->setPos(0,0);
		map->setZValue(-1);
		
		//Resctangle test
		QRect rectangle1(15,15,620,420);
		scene->addRect(rectangle1);
		//rectangle1.moveCenter(QPoint(0,0)); // met le centre au centre
		
		
		//Création de l'origine
		
		QRect rectangle(0,0,10,10);
		rectangle.moveCenter(QPoint(0,0)); // met le centre au centre
		origine=scene->addRect(rectangle);
		origine->setPos(map_im.width()/2,map_im.height()/2);
		
        //Mise en place de du robot
		QPixmap robot_im ; 
		robot_im.load("petit_robot.png");
		robot=scene->addPixmap(robot_im);
		robot->setZValue(2);
		
		robot->setOffset(-robot_im.width()/2,-robot_im.height()/2);
		//robot->setTransformOriginPoint(200,200);
		robot->setPos(origine->x(),origine->y());
        //Mise en place des pions
        //Mise en place de l'obstacle
        
        //Ajout au layout principale puis à la fenetre
        layout0->addWidget(vue, 0,0);
        setLayout(layout0);	
         
         //Configuration du Timer
        // timer = new QTimer(this);
         //timer->start(1000);   		
         
         //Connection des Signaux/Solts
         //connect( timer, SIGNAL(timeout()), this, SLOT(timerOut()) );
       //  modifPosition(robot,-750,-1250,0);
         
}

void FenPrincipale::canRecv(struct libcan::can_t packet)
{
	cout << "Reçu paket :" << endl;
	cout << "\tID : " << packet.id << endl;
	cout << "\tLongueur : " << packet.length << endl;
	cout << "\tPremier octet : " << (int)packet.b[0] << endl;
	
	if (packet.id==516){ // id du message calcul Odometrie
	
	int x=((int16_t*)packet.b)[0];
	int y=((int16_t*)packet.b)[1];
	int theta=((uint16_t*)packet.b)[2]/100; // recoit des centi deg
	
	cout<<"x:"<<x<<"mm "<<"y:"<<y<<"mm "<<"theta:"<<theta<<"dg"<<endl;
	modifPosition(robot,x,y,theta);
	}
}

void FenPrincipale::timerOut()
{
	//Simule des data ...
	//cout << "ok_timer"<<robot->rotation()<<endl;
	//modifPosition(robot,400,400,90);
	
}

void FenPrincipale::modifPosition(QGraphicsItem *robot1,int x , int y , int theta ){
	int x_px, y_px;
	//conversion de x1,y1 en px 
	x_px=origine->x()+(y*mapLargeur_int)/3000;//prod en croix
	y_px=origine->y()+(x*mapHauteur_int)/2000;
	// modif x <-> y
	robot1->setPos(x_px,y_px);
	robot1->setRotation(90-theta);
}
