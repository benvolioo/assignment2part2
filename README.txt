Owen Stadlwieser and Martin Rudolf
Assignment 2 Part 2
CMPUT275 W2020
Driving Route Finder

Subdirectories: client  server

Files contained in client:
client.cpp draw_route.cpp lcd_image.cpp map_drawing.cpp serial_handling.cpp 
consts_and_types.h draw_route.h lcd_image.h map_drawing.h serial_handling.h Makefile

Files contained in server:
digraph.cpp dijkstra.cpp server.cpp serialport.cpp wdigraph.cpp Makefile
digraph.h dijkstra.h heap.h serialport.h wdigraph.h edmonton-roads-2.0.1.txt

Arduino WIRING: (Taken from instructions.txt file on eClass)
Mount the TFT display on the Arduino just like with Assignment 1

Joystick (same as with assignment 1)
VRx <--> A8
VRy <--> A9
SW  <--> Digital Pin 53
GND <--> GND
+5V <--> 5V

Two pushbuttons for zoom. They are initialized in the code with internal pullup
resistors. So for each, one side is connected to GND and the other to a digital
pin.

Zoom Out Button <--> Digital Pin 47
Zoom In Button  <--> Digital Pin 45

Running Instructions:
After downloading the a2part2.zip file and wiring up your Arduino, enter "arduino-port-select" then enter "1" and "1" again. Unzip and navigate to the client directory and enter "make upload". Navigate to the server directory and enter "make" followed by "./server edmontonroads.2.0.1.txt". 

The program will upload. Using the joystick to navigate around the map and by pushing down on the joystick the user can first select a "From" location and then a "To" location. After selecting the "To" location, if a path is found, a driving route will be drawn onto the map. 

Only modified server.cpp, serial_handling.cpp and draw_route.cpp files, other files are from eClass. 