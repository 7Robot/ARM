#include "Mission.h"

#include "Map.h"

#include <thread>

#include <gtkmm/entry.h>

class Gui: public Mission
{
	public:
		void start();
		bool odoEvent(int x, int y, int theta);

	private:
		void gui();
		std::thread * thr;
		Map * map;
		Gtk::Entry * posx;
		Gtk::Entry * posy;
		Gtk::Entry * angle;
	
};
