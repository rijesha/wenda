#include "msgbase.h"
#include <typeinfo>


BaseMessage::BaseMessage(long int i)
{
  time = i;
}

stringstream BaseMessage::getFileString(){
  stringstream ss;
  ss << "phi= "<< std::to_string(this->roll) << endl;
  ss << "theta= "<< std::to_string(this->pitch) << endl;
  ss << "psi= "<< std::to_string(this->yaw) << endl;
  ss << "fix= " << std::to_string(this->fix) << endl;
  ss << "utm_east= " << std::to_string(this->utm_east) << endl;
  ss << "utm_north= " << std::to_string(this->utm_north) << endl;
  ss << "utm_zone= " << std::to_string(this->utm_zone) << endl;
  ss << "speed= " << std::to_string(this->speed) << endl;
  ss << "week= " << std::to_string(this->week) << endl;
  ss << "itow= " << std::to_string(this->itow) << endl;
  ss << "z= " << std::to_string(this->est_height) << endl;
  ss << "z_rate= " << std::to_string(this->est_height_rate) << endl;
  ss << "gps_alt= " << std::to_string(this->alt) << endl;
  ss << "gps_alt_rate= " << std::to_string(this->alt_rate) << endl;
  ss << "roll_rate= " << std::to_string(this->roll_rate) << endl;
  ss << "yaw_rate= " << std::to_string(this->yaw_rate) << endl;
  ss << "pitch_rate= " << std::to_string(this->pitch_rate) << endl;
  return ss;
}

stringstream BaseMessage::getLogBaseString(){
  stringstream ss;
  ss << std::to_string(this->roll) << " ";
  ss << std::to_string(this->pitch) << " ";
  ss << std::to_string(this->yaw) << " ";
  ss << std::to_string(this->fix) << " ";
  ss << std::to_string(this->utm_east) << " ";
  ss << std::to_string(this->utm_north) << " ";
  ss << std::to_string(this->utm_zone) << " ";
  ss << std::to_string(this->speed) << " ";
  ss << std::to_string(this->week) << " ";
  ss << std::to_string(this->itow) << " ";
  ss << std::to_string(this->est_height) << " ";
  ss << std::to_string(this->est_height_rate) << " ";
  ss << std::to_string(this->alt)  << " ";
  ss << std::to_string(this->alt_rate) << " ";
  ss << std::to_string(this->roll_rate) << " ";
  ss << std::to_string(this->yaw_rate) << " ";
  ss << std::to_string(this->pitch_rate) << " ";
  return ss;
}

template <class D>
D BaseMessage::interpolateVar(long int time, long int t1, long int t2, D y1, D y2) {
  return (D)((time-t1)*(y2-y1))/(t2-t1) + y1;
}

template <class D>
D BaseMessage::getRate(long int t1, long int t2, D y1, D y2) {
  return (D) (y2-y1)/(t2-t1);
}
/*
template <class D>
D BaseMessage::interpolateVar(long int time, long int t1, long int t2, D y1, D y2, bool print) {
  std::cout << typeid(D).name() << '\n';
  cout << "y1 " << y1 << " y2 " << y2 << "t1 " << t1 << " t2 " << y2 <<  "time " << time << endl;  
  long int temp = ((time-t1)/(t2-t1));
  D temp1 = (D) temp;
  cout << temp1 * (y2-y1) + y1 << endl; 
  return (D)((time-t1)*(y2-y1))/(t2-t1) + y1;
}*/

bool BaseMessage::operator>(const BaseMessage& bm) {
         return this->time > bm.time;
}
bool BaseMessage::operator<(const BaseMessage& bm) {
         return this->time < bm.time;
}
bool BaseMessage::operator==(const BaseMessage& bm) {
         return this->time == bm.time;
}





GpsMessage::GpsMessage(long int time) : BaseMessage(time){
}

GpsMessage GpsMessage::clone(){
  return GpsMessage(this->time);
}

GpsMessage GpsMessage::interpolate(GpsMessage gm, long int time){
  GpsMessage gm_i = GpsMessage(time);
  gm_i.utm_east = interpolateVar(time, this->time, gm.time, this->utm_east, gm.utm_east);
  gm_i.utm_north = interpolateVar(time, this->time, gm.time, this->utm_north, gm.utm_north);
  gm_i.utm_zone = interpolateVar(time, this->time, gm.time, this->utm_zone, gm.utm_zone);
  gm_i.speed = interpolateVar(time, this->time, gm.time, this->speed, gm.speed);
  gm_i.week = interpolateVar(time, this->time, gm.time, this->week, gm.week);
  gm_i.itow = interpolateVar(time, this->time, gm.time, this->itow, gm.itow);
  gm_i.alt = interpolateVar(time, this->time, gm.time, this->alt, gm.alt);
  gm_i.alt_rate = getRate(this->time, gm.time, this->alt, gm.alt);
  return gm_i;
}

stringstream GpsMessage::getLogString(){
  stringstream ss;
  ss << this->time << " GPS " << getLogBaseString().rdbuf() << endl;
  return ss;
}




WaldoMessage::WaldoMessage(long int time) : BaseMessage(time){
  
}

WaldoMessage WaldoMessage::clone(){
  return WaldoMessage(this->time);
}

WaldoMessage WaldoMessage::interpolate(WaldoMessage wm, long int time){
  WaldoMessage wm_i = WaldoMessage(time);
  wm_i.utm_east = interpolateVar(time, this->time, wm.time, this->utm_east, wm.utm_east);
  wm_i.utm_north = interpolateVar(time, this->time, wm.time, this->utm_north, wm.utm_north);
  wm_i.utm_zone = interpolateVar(time, this->time, wm.time, this->utm_zone, wm.utm_zone);
  wm_i.speed = interpolateVar(time, this->time, wm.time, this->speed, wm.speed);
  wm_i.week = interpolateVar(time, this->time, wm.time, this->week, wm.week);
  wm_i.itow = interpolateVar(time, this->time, wm.time, this->itow, wm.itow);
  wm_i.roll = interpolateVar(time, this->time, wm.time, this->roll, wm.roll);
  wm_i.yaw = interpolateVar(time, this->time, wm.time, this->yaw, wm.yaw);
  wm_i.pitch = interpolateVar(time, this->time, wm.time, this->pitch, wm.pitch);
  wm_i.est_height = interpolateVar(time, this->time, wm.time, this->est_height, wm.est_height);
  wm_i.roll_rate = getRate(this->time, wm.time, this->roll, wm.roll);
  wm_i.yaw_rate = getRate(this->time, wm.time, this->yaw, wm.yaw);
  wm_i.pitch_rate = getRate(this->time, wm.time, this->pitch, wm.pitch);
  wm_i.est_height_rate = getRate(this->time, wm.time, this->est_height, wm.est_height);
  return wm_i;
}

stringstream WaldoMessage::getLogString(){
  stringstream ss;
  ss << this->time <<" WALDO " << getLogBaseString().rdbuf() << endl;
  return ss;
}