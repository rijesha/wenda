# wenda
A Paparazzi message parser to interpolate data required for target localization. Originally designed to integrate with Waldo from the University of Alberta Aerial Robotics Group.

Goals:
* Capable of reading serial messages in the Paparazzi Format
* Interpolate messages so as to produce data accurate to a specific timestamp.
* Provide a buffer to allow for delayed interpolation results. 

configuration.h contains commonly accessed parameters. The #STANDALONE flag is used to build a standalone monitoring program.

autopilot.h has the functions exposed for interacting with wenda.

autopilot.cpp has an example program that monitors a folder and generates a text file with interpolated data whenever a new image is added to that folder.
