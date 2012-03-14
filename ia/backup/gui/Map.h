#ifndef _MAP_H_
#define _MAP_H_

#include <gtkmm/drawingarea.h>

class Map: public Gtk::DrawingArea
{
	public :
		Map();
		~Map();
		void setPosPetit(int x, int y, int theta);

	protected:
		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
		Glib::RefPtr<Gdk::Pixbuf> map;
		Glib::RefPtr<Gdk::Pixbuf> petit;
		int petit_x, petit_y, petit_theta;
		int h, w;
		int pw, ph;
};

#endif
