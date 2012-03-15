#include "Mission.h"

#include "Map.h"

#include <thread>

#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>

class Gui: public Mission
{
	public:
		void start();
		bool odoAnswer(int x, int y, int theta);
		bool microswitchEvent(int id, bool status);

	private:
		void gui();
		std::thread * thr;
		Map * map;
		Gtk::CheckButton * msFrontState;
		Gtk::CheckButton * msBackState;
		Gtk::Entry * posx;
		Gtk::Entry * posy;
		Gtk::Entry * angle;
	
};
