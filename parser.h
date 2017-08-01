// main.c
// Proof of concept for using PPRZLink's generated C messages interface
// to read autopilot data.

#ifndef CRUNNER_H
#define CRUNNER_H

#include <string.h>

#ifdef __cplusplus
  extern "C" {
#endif
#include "configuration.h"
#include "messages.h"
#include "pprzlink/pprz_transport.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

void dl_parse_msg();
int initParser();
void runner();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif