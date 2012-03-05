#include "Map.h"

#include <iostream>

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <glibmm/fileutils.h>
#include <math.h>

#define PI 3.14159265358979323846264

Map::Map(): petit_x(0), petit_y(0), petit_theta(0)
{
	try {
		map = Gdk::Pixbuf::create_from_file("/media/data/vcs/git/7Robot/ARM/ia/gui/map.png");
	} catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
	;} catch(const Gdk::PixbufError& ex) {
		std::cerr << "PixbufError: " << ex.what() << std::endl;
	}
	if (map) {
		w = map->get_width();
		h = map->get_height();
		set_size_request(w, h);
	}
	try {
		petit = Gdk::Pixbuf::create_from_file("/media/data/vcs/git/7Robot/ARM/ia/gui/petit.png");
	} catch(const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
	;} catch(const Gdk::PixbufError& ex) {
		std::cerr << "PixbufError: " << ex.what() << std::endl;
	}
	if (petit) {
		pw = petit->get_width();
		ph = petit->get_height();
	}
}

Map::~Map()
{
}

void Map::setPosPetit(int x, int y, int theta)
{
	if (x > 1500) {
		petit_x = 1500;
	} else if (x < -1500) {
		petit_x = -1500;
	} else {
		petit_x = x;
	}
	if (y > 1000) {
		petit_y = 1000;
	} else if (y < -1000) {
		petit_y = -1000;
	} else {
		petit_y = y;
	}
	printf("Petit(%d, %d, %d)\n", petit_x, petit_y, petit_theta);
	petit_theta = theta;
	queue_draw();
}

bool Map::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!map || !petit)
		return false;

	Gdk::Cairo::set_source_pixbuf(cr, map, 0, 0);
	cr->paint();

	double t = (petit_theta * PI) / 18000.0;
	double dx = (pw*(cos(t)-1)-ph*sin(t))/2;
	double dy = (pw*sin(t)+ph*(cos(t)-1))/2;

	double petit_x = this->petit_x*w/3000.0 - dx - pw/2.0 + w/2.0;
	double petit_y = this->petit_y*h/2000.0 - dy - ph/2.0 + h/2.0;
	double petit_theta = this->petit_theta/100.0;

	cr->save();
	cr->translate(petit_x, petit_y);
	cr->rotate_degrees(petit_theta);
	Gdk::Cairo::set_source_pixbuf(cr, petit, 0, 0);
	cr->paint();
	cr->restore();
}
