// main.c
// Proof of concept for using PPRZLink's generated C messages interface
// to read autopilot data.

#ifdef __cplusplus
  extern "C" {
#endif
#include "parser.h"

uint8_t parsed_message_buffer[MESSAGE_BUFFER_LENGTH];
bool msg_available;
struct pprz_transport *reader = (pprz_transport*) malloc(sizeof(struct pprz_transport));
struct link_device serial_port_dev;


int initParser() {

  
  serial_port_dev.char_available = (char_available_t) &uartBytesAvailable;
  serial_port_dev.get_byte = (get_byte_t) &uartGetByte;

  if (uartOpen()) {
    return 1;
  }

  uartInit();

  pprz_transport_init(reader);
  return 0;

}

void runner(){
  pprz_check_and_parse(&serial_port_dev, reader, parsed_message_buffer, &msg_available);
    if (msg_available) {
      dl_parse_msg();
      msg_available = false;
    }
}

void closeParser(){
  free(reader);
}

#ifdef __cplusplus
}
#endif

#include "autopilot_cpp.h"
#include <chrono>


void dl_parse_msg() {
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
  long int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_ms.time_since_epoch()).count();

  WaldoMessage wm(ms);
  GpsMessage gm(ms);


  int msg_id = parsed_message_buffer[1];
  switch (msg_id) {
    case DL_WALDO_MSG:
      wm.roll = DL_WALDO_MSG_roll(parsed_message_buffer);
      wm.yaw = DL_WALDO_MSG_yaw(parsed_message_buffer);
      wm.pitch = DL_WALDO_MSG_pitch(parsed_message_buffer);
      wm.fix = DL_WALDO_MSG_fix(parsed_message_buffer);
      wm.utm_east = DL_WALDO_MSG_utm_east(parsed_message_buffer);
      wm.utm_north = DL_WALDO_MSG_utm_north(parsed_message_buffer);
      wm.utm_zone = DL_WALDO_MSG_utm_zone(parsed_message_buffer);
      wm.speed = DL_WALDO_MSG_speed(parsed_message_buffer);
      wm.week = DL_WALDO_MSG_week(parsed_message_buffer);
      wm.itow = DL_WALDO_MSG_itow(parsed_message_buffer);
      wm.est_height= DL_WALDO_MSG_est_height(parsed_message_buffer);
      wmBuffer.push(wm);
      logFile << wm.getLogString().rdbuf();
      break;

    case DL_GPS:
      gm.utm_east = DL_GPS_utm_east(parsed_message_buffer);
      gm.utm_north = DL_GPS_utm_north(parsed_message_buffer);
      gm.utm_zone = DL_GPS_utm_zone(parsed_message_buffer);
      gm.speed = DL_GPS_speed(parsed_message_buffer);
      gm.week = DL_GPS_week(parsed_message_buffer);
      gm.itow = DL_GPS_itow(parsed_message_buffer);
      gm.alt = DL_GPS_alt(parsed_message_buffer);
      gmBuffer.push(gm);
      logFile << gm.getLogString().rdbuf();
      break;

    default:
      break;
  }
    
}
