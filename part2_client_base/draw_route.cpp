#include "draw_route.h"
#include "map_drawing.h"

extern shared_vars shared;

void draw_route() {
  // implement this!
    shared.tft->setCursor(0, 0);
    shared.tft->setTextColor(0xFFFF);
    shared.tft->drawRect(100, 100, 100, 100, 0x001F);
    shared.tft->println(shared.num_waypoints);
	for (int i =0; i < shared.num_waypoints-1; i++)
	{
		int x1 = longitude_to_x(shared.map_number, shared.waypoints[i].lon);
		int y1 = latitude_to_y(shared.map_number, shared.waypoints[i].lat);
		int x2 = longitude_to_x(shared.map_number, shared.waypoints[i+1].lon);
		int y2 = latitude_to_y(shared.map_number, shared.waypoints[i+1].lat);
		shared.tft->println(shared.waypoints[i].lon);
		shared.tft->println(shared.waypoints[i].lat);
		shared.tft->drawLine(x1, y1, x2, y2, 0x001F);
	}
}
