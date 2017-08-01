
#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include <iostream>
#include <sstream>
#include <stdint.h>
using namespace std;


class BaseMessage
{
public:
  BaseMessage(long int i);
  long int time;
  float roll;
  float yaw;
  float pitch;
  uint8_t fix;
  int32_t utm_east;
  int32_t utm_north;
  uint8_t utm_zone;
  uint16_t speed;
  uint16_t week;
  uint32_t itow;
  float est_height;
  int32_t alt;
  float roll_rate;
  float yaw_rate;
  float pitch_rate;
  int32_t alt_rate;
  float est_height_rate;


  stringstream getFileString();
  stringstream getLogBaseString();

  template <class D>
  D interpolateVar(long int time, long int t1, long int t2, D y1, D y2);

  template <class D>
  D getRate(long int t1, long int t2, D y1, D y2);

  bool operator==(const BaseMessage& bm1);
  bool operator>(const BaseMessage& bm1);
  bool operator<(const BaseMessage& bm1);
};

class GpsMessage : public BaseMessage
{
public:
  GpsMessage(long int time);

  GpsMessage clone();
  GpsMessage interpolate(GpsMessage gm, long int time);
  stringstream getLogString();
};

class WaldoMessage : public BaseMessage
{
public:
  WaldoMessage(long int time);

  WaldoMessage clone();
  WaldoMessage interpolate(WaldoMessage wm,long int time);
  stringstream getLogString();
};

#endif /* BASEMESSAGE_H */
