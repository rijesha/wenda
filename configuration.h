// Configuration file.

#ifndef STANDALONE
#include "../configuration.h"

#else
#ifndef PPRZLINK_CONFIG_H
#define PPRZLINK_CONFIG_H

#define MAX_PATH_LENGTH 256
#define MESSAGE_BUFFER_LENGTH 256
#define N_REQUEST_READ_BYTES 1
#define TEXT_BUFFER_LENGTH 500

#define DOWNLINK 1

#define LOG_PARSED_MESSAGES
//#define PRINT_REQUESTED_INTERPOLATED
//#define DEBUG_INTERPOLATION
//#define PRINT_RAW_DATA
#define AUTOPILOT_TESTING 1
//#define AUTOPILOT_INPUT_TERMINAL "waldo_msg.log"
#define AUTOPILOT_INPUT_TERMINAL "/dev/ttyS1"

#define BAUD_RATE 57600

#define BUFFER_SIZE 2 //in seconds
#define WALDO_MSG_FREQ 10 //in Hertz

#define USE_GPS
#define GPS_MSG_FREQ 4

#endif
#endif
