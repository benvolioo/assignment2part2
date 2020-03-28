#include "draw_route.h"
#include "map_drawing.h"

extern shared_vars shared;

/*
	Description: Draws the driving route onto the Arduino map. 
	Arguments:  
	Returns: void
*/
void draw_route() 
{
    shared.tft->setCursor(0, 0);
    shared.tft->setTextColor(0xFFFF);
    shared.tft->println(shared.num_waypoints);
	for (int i =0; i < shared.num_waypoints - 1; i++)
	{
		int x1 = longitude_to_x(shared.map_number, shared.waypoints[i].lon) - shared.map_coords.x;
		int y1 = latitude_to_y(shared.map_number, shared.waypoints[i].lat) - shared.map_coords.y;
		int x2 = longitude_to_x(shared.map_number, shared.waypoints[i+1].lon)- shared.map_coords.x;
		int y2 = latitude_to_y(shared.map_number, shared.waypoints[i+1].lat)- shared.map_coords.y;
		shared.tft->drawLine(x1, y1, x2, y2, 0x001F);
	}
}
