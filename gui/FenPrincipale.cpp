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
		
		//Rectangle test_debug
		QRect rectangle1(15,15,618,418);
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
		robot->setOffset(-robot_im.width()/2,-robot_im.height()/2); // origine du robot au centre
		robot->setPos(origine->x(),origine->y());//placer au centre du palteau
        //Mise en place des pions
        //Mise en place de l'obstacle
        obstacle = scene->addEllipse(0,0,15,15);
        obstacle->setZValue(1);
        obstacle->setPos(origine->x(),origine->y());
        //Ajout au layout principale puis à la fenetre
        layout0->addWidget(vue, 0,0);
        setLayout(layout0);	
         
         //Configuration du Timer
        timer = new QTimer(this);
        // timer->start(1000);   		
         
         //Connection des Signaux/Solts
         connect( timer, SIGNAL(timeout()), this, SLOT(timerOut()) );
         modifPosition(robot,-750,-1250,90);
         testPosObs(100,90);
         
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
	
	if (packet.id==133){ // id du message calcul Odometrie
	int d1=packet.b[0];
	int ang1=packet.b[1];
	int d2=packet.b[2];
	int ang2=packet.b[3];
	int d3=packet.b[4];
	int ang3=packet.b[5];
	int d4=packet.b[6];
	int ang4=packet.b[7];
	
	cout<<"d1:"<<d1<<"mm "<<"ang1:"<<ang1<<"deg "<<endl;
	
	
	}
	
}

void FenPrincipale::timerOut()
{
	//Simule des data ...
	cout << "angle robot ="<<robot->rotation()<<endl;
	
	
}

void FenPrincipale::modifPosition(QGraphicsItem *robot1,int x , int y , int theta ){
	int x_px, y_px;
	//conversion de x1,y1 en px 
	x_px=origine->x()+(y*mapLargeur_int)/3000;//convention oriantation terrain
	y_px=origine->y()+(x*mapHauteur_int)/2000;
	// modif x <-> y
	robot1->setPos(x_px,y_px);
	robot1->setRotation(90-theta);// convention avec martin
}

void FenPrincipale::testPosObs(int d,int angle){
	int angle_modifRomain;
	int angle_total;
	int x1, y1;
	int dx, dy;
	
	angle_modifRomain=-(90-angle);
	
	x1=d*cos((angle_modifRomain*3.14)/180);
	y1=d*sin((angle_modifRomain*3.14)/180);
	
	dx=x1*cos((robot->rotation()*3.14)/180)-y1*sin((robot->rotation()*3.14)/180);
	dy=x1*sin((robot->rotation()*3.14)/180)+y1*cos((robot->rotation()*3.14)/180);
	
	cout<<"dx="<<x1<<"dy="<<y1<<endl;
	cout<<"angle robot"<<robot->rotation()<<endl;
	
	obstacle->setPos(robot->x()+dx,robot->y()+dy); // ici en px 
	//modifPosition(obstacle,robot->x()+dx,robot->y()+dy,0);// en mmm
}
