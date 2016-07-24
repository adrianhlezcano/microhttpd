#include<sys/types.h>
#include<sys/select.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<microhttpd.h>

#ifndef __STDIO_H__
#define __STDIO_H__ 1
#include<stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__ 1
#include<stdlib.h>
#endif

#ifndef __STRING_H__
#define __STRING_H__ 1
#include<string.h>
#endif

#include "custom_func.h"
#include "command_line_args.h"

/*
 * Status response
 * code - http status response code
 * msg  - http status response message
 */ 
typedef struct {
	int code;
	char *msg;
} StatusResponse;

/*
 * Initialize web server.
 * Start up MHD daemon and setup listeners
 */ 
void start_webserver();

/*
 * Shutdown web server.
 * Kill MHD daemon
 */ 
void stop_webserver();
