#ifndef HEADER_FENAFF
#define HEADER_FENAFF

#include <libcan.h>
#include <QtGui>
#include <iostream>
#include <math.h>

class FenAff : public QWidget
{
    Q_OBJECT


    public:
       FenAff();
       bool send(int id, int length, ...);
       
       void ordre_AsservLigne();
       void ordre_AsservRot();
       void ordre_AsservON();
       void ordre_AsservOff();
       void ordre_OdoSet();
        
  private slots:
      void send_ordre();
      
    private:
		QPushButton *b_send;
		QSpinBox *param1;
		QSpinBox *param2;
		QSpinBox *param3;
		int16_t a;
		int16_t b;
		int16_t c;
		QComboBox *liste;
		
		QLCDNumber *lcd_odoX;
       QLCDNumber *lcd_odoY;
       QLCDNumber *lcd_odoTheta;
       
       

};

#endif


