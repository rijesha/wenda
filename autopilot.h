
#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "configuration.h"


#ifdef __cplusplus
#include <string>
using namespace std;
  extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


void getInterpolatedData(long int time);
int initAutopilotDataReading();
void autopilotThreadRunner();
int waitForDataReady();
bool openLogFile();
long int getCurrentTime();

float getSpeed();
float getHeight();
float getPitch();
float getRoll();

void cleanupAutopilotDataReading();

const char *strAutopilotError(int error_number);
char text_file_string_buffer[TEXT_BUFFER_LENGTH];

#ifdef __cplusplus
string getInterpolatedDataString(long int time);
}
#endif

#endif
