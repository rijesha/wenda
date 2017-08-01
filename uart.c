// uart.c
// Functions for reading and writing to the device's serial port.

#include "uart.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "configuration.h"

uint8_t raw_message_buffer;
int fd; // Global serial file descriptor
int stole_byte_from_buffer = 0;
uint8_t stolen_byte;

int uartOpen() {
  // Handle serial port opening
  fd = open(AUTOPILOT_INPUT_TERMINAL, O_RDWR); // Attempt to connect to port
  if (fd == -1) { // A file open error occur
      printf("Failed to open autopilot serial port (%s) for reading.\n", AUTOPILOT_INPUT_TERMINAL);
      return 1;
  }
  return 0;
}

int uartInit() {
  /*
  Apply some basic serial port settings to the provided file
  descriptor. In particular, baud rate, parity, stop bits, etc...

  For details of what does what, see:
  http://man7.org/linux/man-pages/man3/termios.3.html
  */

  struct termios settings;
  tcgetattr(fd, &settings);

  /*
  Settings copied from the open source Paparazzi project:
  https://github.com/paparazzi/paparazzi/blob/master/sw/lib/ocaml/cserial.c
  */

  /* input modes */
  settings.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IXON|IXANY|IXOFF|IMAXBEL);
  settings.c_iflag |= BRKINT;

  /* output_flags */
  settings.c_oflag  &=~(OPOST|ONLCR|OCRNL|ONOCR|ONLRET);

  /* control modes */
  settings.c_cflag &= ~(CSIZE|CSTOPB|CREAD|PARENB|PARODD|HUPCL|CLOCAL|CRTSCTS);

  settings.c_cflag |= CREAD|CS8|CLOCAL;

  /* local modes */
  settings.c_lflag &= ~(ISIG|ICANON|IEXTEN|ECHO|FLUSHO|PENDIN);
  settings.c_lflag |= NOFLSH;

  cfsetspeed(&settings, BAUD_RATE);

  // Set up non-blocking serial port read
  fcntl(fd, F_SETFL, O_NONBLOCK);

  return tcsetattr(fd, TCSADRAIN, &settings); /* apply the settings */


}

uint8_t uartGetByte() {
    /*
    This function simply calls the posix read() function on the
    provided serial port file descriptor. Unless AUTOPILOT_TESTING
    is defined in which case this function will slowly call read()
    on the normal file file descriptor to simulate a real serial
    port.
    */

    read(fd, &raw_message_buffer, 1);

    #ifdef AUTOPILOT_TESTING
    usleep(1000000/BAUD_RATE);
    #endif
    #ifdef PRINT_RAW_DATA
    printf("%02X ", raw_message_buffer);
    #endif
    return raw_message_buffer;

}

// Checks if a byte is available in the serial port
int uartBytesAvailable() {
  int bytesAvailable;
  ioctl(fd, FIONREAD, &bytesAvailable);
  #ifdef AUTOPILOT_TESTING
  if (bytesAvailable == 0){
      //Detected end of file. Rewinding to start.
      lseek(fd, 0, SEEK_SET);
      ioctl(fd, FIONREAD, &bytesAvailable);
  }
  #endif
  return bytesAvailable;
}
