#include "Gui.h"

#include <stdio.h>
#include <gtkmm.h>

void Gui::start()
{
	thr = new std::thread(&Gui::gui,this);
}

void Gui::gui()
{
	Gtk::Main gui(0, NULL);
	
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

	builder->add_from_file("/media/data/vcs/git/7Robot/ARM/ia/gui/gui.glade");

	Gtk::Window * win = 0;
	builder->get_widget("window", win);

	Gtk::SpinButton * distance = 0;
	builder->get_widget("spinbuttondistance", distance);
	Gtk::Button * buttondistance = 0;
	builder->get_widget("buttondistance", buttondistance);
	if (distance && buttondistance) {
		auto action = [this, &distance]() {
			can->fwd(distance->get_adjustment()->get_value());
		};
		(buttondistance->signal_clicked()).connect(action);
		(distance->signal_activate()).connect(action);
	}

	Gtk::SpinButton * speedleft = 0;
	builder->get_widget("spinbuttonspeedleft", speedleft);
	Gtk::SpinButton * speedright = 0;
	builder->get_widget("spinbuttonspeedright", speedright);
	Gtk::CheckButton * ramp = 0;
	builder->get_widget("checkbuttonrampe", ramp);
	Gtk::Button * speed = 0;
	builder->get_widget("buttonspeed", speed);
	if (speed && ramp && speedleft && speedright) {
		auto action = [this, &speedleft, &speedright, &ramp]() {
			can->fwd(speedleft->get_adjustment()->get_value(),
				speedright->get_adjustment()->get_value(),
				ramp->get_active());
		};
		(speed->signal_clicked()).connect(action);
		(speedleft->signal_activate()).connect(action);
		(speedright->signal_activate()).connect(action);
	}

	Gtk::SpinButton * spinangle = 0;
	builder->get_widget("spinbuttonangle", spinangle);
	Gtk::Button * buttonangle = 0;
	builder->get_widget("buttonangle", buttonangle);
	if (spinangle && buttonangle) {
		auto action = [this, &spinangle]() {
			can->rotate(spinangle->get_adjustment()->get_value());
		};
		(buttonangle->signal_clicked()).connect(action);
		(spinangle->signal_activate()).connect(action);
	}

	builder->get_widget("posx", posx);
	builder->get_widget("posy", posy);
	builder->get_widget("angle", angle);

	Gtk::Button * posupdate = 0;
	builder->get_widget("posupdate", posupdate);
	if (posupdate) {
		(posupdate->signal_clicked()).connect([this]() {
			can->odoRequest();
		});
	}

	Gtk::Button * stop = 0;
	builder->get_widget("stop", stop);
	if (stop) {
		(stop->signal_clicked()).connect([this]() {
			can->send(1051, 0);
		});
	}

	Gtk::Box * mainbox = 0;
	builder->get_widget("mainbox", mainbox);
	if (mainbox) {
		map = new Map();
		mainbox->pack_start(*map);
		map->show();
	}

	if (win) {
		gui.run(*win);
	}
	delete win;
}

bool Gui::odoEvent(int x, int y, int theta)
{
	static char buffer[32];
	if (map) {
		map->setPosPetit(x, y, theta);
	}
	if (posx) {
		sprintf(buffer, "%.1f cm", x/10.0);
		posx->set_text(buffer);
	}
	if (posy) {
		sprintf(buffer, "%.1f cm", y/10.0);
		posy->set_text(buffer);
	}
	if (angle) {
		sprintf(buffer, "%.2f°", theta/100.0);
		angle->set_text(buffer);
	}

	return true;
}

extern "C" {
	Mission * create() {
		return new Gui();
	}
}

