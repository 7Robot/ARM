#ifndef HEADER_FENAFF
#define HEADER_FENAFF

#include <QtGui>
#include <iostream>
class FenAff : public QWidget
{
    Q_OBJECT


    public:
       FenAff(); 
  private slots:
      void send_ordre();
    private:
		QPushButton *b_send;
		QSpinBox *param1;
		QSpinBox *param2;
		QComboBox *liste;
		
		QLCDNumber *lcd_odoX;
       QLCDNumber *lcd_odoY;
       QLCDNumber *lcd_odoTheta;

};

#endif


