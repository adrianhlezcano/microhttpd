#ifndef __STDIO_H__
#define __STDIO_H__ 1
#include<stdio.h>
#endif

#ifndef __TIME_H__
#define __TIME_H__ 1
#include<time.h>
#endif

#ifndef __STRING_H__
#define __STRING_H__ 1
#include<string.h>
#endif

extern time_t uptime;

/*
 * Get Current time
 */ 
char *curr_str_time();

/*
 * Initialize Uptime
 */ 
void init_uptime();

/*
 * Get Uptime
 */ 
unsigned long get_uptime();
