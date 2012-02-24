
#include "FenAff.h"

using namespace std;

FenAff::FenAff()
{
	//def des layout
	QGridLayout *layout0 = new QGridLayout;
	
	QGridLayout *layout1 = new QGridLayout;
	QFrame *frame1 = new QFrame();
	frame1->setFrameShape(QFrame::StyledPanel);
	
	b_send = new QPushButton("send");
	
	liste = new QComboBox(this);
    liste->addItem("Asserv_Line");
    liste->addItem("Rotation");
    liste->addItem("Test_balise");
    liste->addItem("Capteur");
    //liste->move(30, 20);
    
    param1=new QSpinBox();
    param2=new QSpinBox();
    
    layout1->addWidget(liste, 0,0);
    layout1->addWidget(param1, 0,1);
    layout1->addWidget(param2, 0,2);
    layout1->addWidget(b_send, 0,3);
	frame1->setLayout(layout1);
	
	//parti aff de data 
	lcd_odoX = new QLCDNumber();
	lcd_odoX->setSegmentStyle(QLCDNumber::Flat);//option d'affichage
	QLabel *odo_titreX = new QLabel("X:");
	
	lcd_odoY = new QLCDNumber();
	lcd_odoY->setSegmentStyle(QLCDNumber::Flat);//option d'affichage
	QLabel *odo_titreY = new QLabel("Y:");
	
	lcd_odoTheta = new QLCDNumber();
	lcd_odoTheta->setSegmentStyle(QLCDNumber::Flat);//option d'affichage
	QLabel *odo_titreTheta = new QLabel("Theta:");
	
    QHBoxLayout *layout2 = new QHBoxLayout;
    QFrame *frame2 = new QFrame();
	frame2->setFrameShape(QFrame::StyledPanel);
    
	layout2->addWidget(odo_titreX);
	layout2->addWidget(lcd_odoX);
	layout2->addWidget(odo_titreY);
	layout2->addWidget(lcd_odoY);
	layout2->addWidget(odo_titreTheta);
	layout2->addWidget(lcd_odoTheta);
	
	frame2->setLayout(layout2);
	
	//config du layout 0
	layout0->addWidget(frame1,0,0);
	layout0->addWidget(frame2,1,0);
	
	// On configure la fenetre princip ale
	setLayout(layout0);
	//resize(350, 400);
    setWindowTitle("Fenetre Debug Affichage");
    
    //Conection des solts
    connect(b_send, SIGNAL(clicked()), this, SLOT(send_ordre()));
}


void FenAff::send_ordre()
{
	cout <<"Ordre n°: "<<liste->currentIndex()<<" param1: "<<param1->value()<<" param2: "<<param2->value()<<endl ;
	switch (liste->currentIndex()) {

		case 0 :
			break;

		case 1 :
			break;

		case 2 :
			break;

		default: 
		  break;
	}	
}
