#include "autopilot.h"
#include "autopilot_cpp.h"
#include <unistd.h>
#include <ctime>
#include <thread>
#include <iomanip>
#include <string> 
#include <stdbool.h>
#include <errno.h>
#include <chrono>

CircularBuffer<WaldoMessage> wmBuffer((uint) BUFFER_SIZE*WALDO_MSG_FREQ);
CircularBuffer<GpsMessage> gmBuffer((uint) BUFFER_SIZE*GPS_MSG_FREQ);

using namespace std;

std::ofstream logFile;

long int getCurrentTime(){
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
  long int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_ms.time_since_epoch()).count();
  return ms;
}

int initAutopilotDataReading(){
  #ifdef LOG_PARSED_MESSAGES
  if (!openLogFile()){
    cout << "Failed to open parsed messages log file" << endl;
  }
  #endif
  initParser();
  return 0;
}

void autopilotThreadRunner(){
  runner();
}

int waitForDataReady(){
  int count = 10;
  while (!wmBuffer.filled() or !gmBuffer.filled()){
    if (count == 10)
      cout << "Waiting for Data Buffer to fill" << endl;
    count--;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (count < 0) {
      cout << "Failed To fill Data Buffer" << endl;
      return 1;
    }
  }
  
  cout << "Autopilot Data Buffer Ready" << endl;
  return 0;
}

void getInterpolatedData(long int time){
  WaldoMessage wm = wmBuffer.getInterpolated(time); 
  #ifdef USE_GPS
    GpsMessage gm = gmBuffer.getInterpolated(time);
    wm.utm_east = gm.utm_east;
    wm.utm_north = gm.utm_north;
    wm.utm_zone = gm.utm_zone;
    wm.week = gm.week;
    wm.itow = gm.itow;
    wm.speed = gm.speed;
    wm.alt = gm.alt;
    wm.alt_rate = gm.alt_rate;
  #endif

  sprintf(text_file_string_buffer, "%s", wm.getFileString().str().c_str());

 // cout << wm.getFileString().str() << endl;
 // const char * c = wm.getFileString().str().c_str();

  #ifdef PRINT_REQUESTED_INTERPOLATED
    cout << text_file_string_buffer;
  #endif
}

string getInterpolatedDataString(long int time){
  WaldoMessage wm = wmBuffer.getInterpolated(time); 
  #ifdef USE_GPS
    GpsMessage gm = gmBuffer.getInterpolated(time);
    wm.utm_east = gm.utm_east;
    wm.utm_north = gm.utm_north;
    wm.utm_zone = gm.utm_zone;
    wm.week = gm.week;
    wm.itow = gm.itow;
    wm.speed = gm.speed;
    wm.alt = gm.alt;
    wm.alt_rate = gm.alt_rate;
  #endif

  #ifdef PRINT_REQUESTED_INTERPOLATED
    cout << wm.getFileString().str();
  #endif

  return wm.getFileString().str();
}

float getHeight(){
  return wmBuffer.getLastDataPoint().est_height;
}
  

float getSpeed(){
  return wmBuffer.getLastDataPoint().speed;
}
  
float getPitch(){
  return wmBuffer.getLastDataPoint().pitch;
}

float getRoll(){
  return wmBuffer.getLastDataPoint().roll;
} 

void cleanupAutopilotDataReading(void)
{
  #ifdef LOG_PARSED_MESSAGES
   logFile.close();
   #endif
}

const char *strAutopilotError(int error_number){
    /*
    Returns a string containing a user-friendly error message
    corresponding to the autopilot reading error number provided.
    Note that the current implementation doesn't actually use the
    error_number but rather the string stored in error_str. So this
    function will only give the most recent error message.
    */
    switch (error_number){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return "(Autopilot Error not found.)";
        default:
            return "(Autopilot Error not found.)";

    };
}


bool openLogFile(){
  time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << endl;
  logFile.open(std::to_string(getCurrentTime()));
  return logFile.is_open();
}
