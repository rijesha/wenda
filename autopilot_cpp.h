
#ifndef AUTOPILOT_CPP_H
#define AUTOPILOT_CPP_H

#include "parser.h"
#include "cBuffer.h"
#include "msgbase.h"
#include <fstream>
#include <iostream>

extern CircularBuffer<WaldoMessage> wmBuffer;
extern CircularBuffer<GpsMessage> gmBuffer;

extern std::ofstream logFile;


#endif
