#ifndef __STDIO_H__
#define __STDIO_H__ 1
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__ 1
#include <stdlib.h>
#endif

#ifndef __GETOPT_H__
#define __GETOPT_H__ 1
#include <getopt.h>
#endif

// ---- OPTIONS ---
// short form   |   long form    |   Purpose
//     -h       |  --help        | Display usage summary and exit
//     -p       |  --port        | Specify port of webserver
//     -d       |  --dir         | Specify root dir of webpages in webserver
extern const char* const short_options;
extern const struct option long_options[];

/*
 * This struct store 
 * port - web server port
 * dir  - web server root dir of web pages 
 */
typedef struct{
	int port;
	char *dir;
} CMD_OPTS;

/*
 * Prints usage information
 */ 
void print_usage();

/*
 * Get command line options
 * 
 * cmd_opts CMD_OPTS struct 
 * argc number of argument list
 * argv array of pointers to argument list
 * returns 0 if success or -1 if fail
 */
int get_cmd_opts(CMD_OPTS *cmd_opts, const int argc, char *argv[]);
