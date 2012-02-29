
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
	b_violet = new QPushButton("violet");
	b_rouge = new QPushButton("rouge");
	
	liste = new QComboBox(this);
    liste->addItem("Asserv_Line");
    liste->addItem("Asserv_Rot");
    liste->addItem("Asserv_On");
    liste->addItem("Asserv_Off");
    liste->addItem("Odo_Set");
    //liste->move(30, 20);
    
    param1=new QSpinBox();
    param1->setRange(-3000,3000);
    param2=new QSpinBox();
     param2->setRange(-3000,3000);
    param3=new QSpinBox();
     param3->setRange(-3000,3000);
    
    layout1->addWidget(liste, 0,0);
    layout1->addWidget(param1, 0,1);
    layout1->addWidget(param2, 0,2);
    layout1->addWidget(param3, 0,3);
    layout1->addWidget(b_send, 0,4);
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
	
	QGridLayout *layout3 = new QGridLayout;
//	 QVBoxLayout layout3 ;
	layout3->addWidget(b_violet, 0,0);
	layout3->addWidget(b_rouge, 0,1);
	
	//config du layout 0
	layout0->addWidget(frame1,0,0);
	layout0->addWidget(frame2,1,0);
	layout0->addLayout(layout3,2,0);

	
	
	// On configure la fenetre princip ale
	setLayout(layout0);
	//resize(350, 400);
    setWindowTitle("Fenetre Debug Affichage");
    
    //Conection des solts
    connect(b_send, SIGNAL(clicked()), this, SLOT(send_ordre()));
    connect(b_violet, SIGNAL(clicked()), this, SLOT(send_violet()));
    connect(b_rouge, SIGNAL(clicked()), this, SLOT(send_rouge()));
}


void FenAff::send_ordre()
{
	//cout <<"Ordre n°: "<<liste->currentIndex()<<" param1: "<<param1->value()<<" param2: "<<param2->value()<<endl ;
	a = param1->value();
	b = param2->value();
	c= param3->value();
	switch (liste->currentIndex()) {

		case 0 :
		ordre_AsservLigne();
			break;

		case 1 :
		ordre_AsservRot();
			break;

		case 2 :
		ordre_AsservON();
			break;
			
		case 3 :
		ordre_AsservOff();
			break;
			
		case 4 :
		ordre_OdoSet();
			break;
			
			

		default: 
		  break;
	}	
}

bool FenAff::send(int id, int length, ...)
{
	//printf("Can::send(%d, %d, ...)\n", id, length);
	va_list ap;

    va_start(ap, length);
	can_vwrite(1,libcan::dec,id, length, ap); // a lodifier
    va_end(ap);

	return false;
}

void FenAff::ordre_AsservLigne(){
	
	a=round(a*13.6);
	send(1025,4,((char*)&a)[0],((char*)&a)[1]);
}

void FenAff::ordre_AsservRot(){
	
	a=round(a*23.4);
	send(1026,4,((char*)&a)[0],((char*)&a)[1]);
}

void FenAff::ordre_AsservON(){
	send(1030,0);
}

void FenAff::ordre_AsservOff(){
	send(1031,0);
}

void FenAff::ordre_OdoSet(){
	uint16_t d = c * 100;
	send(517,6,((char*)&a)[0],((char*)&a)[1],((char*)&b)[0],((char*)&b)[1],((char*)&d)[0],((char*)&d)[1]);
}

void FenAff::send_violet(){
	int16_t a=-750;
	int16_t b=-1250;
	int16_t d=-90;
	send(517,6,((char*)&a)[0],((char*)&a)[1],((char*)&b)[0],((char*)&b)[1],((char*)&d)[0],((char*)&d)[1]);
}

void FenAff::send_rouge(){
	int16_t a=750;
	int16_t b=1250;
	int16_t d=90;
	send(517,6,((char*)&a)[0],((char*)&a)[1],((char*)&b)[0],((char*)&b)[1],((char*)&d)[0],((char*)&d)[1]);
}
