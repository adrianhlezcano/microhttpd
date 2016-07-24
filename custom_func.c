#include "custom_func.h"

time_t uptime;

char * curr_str_time(){
  time_t now;
  time(&now);

  char *time = asctime(gmtime(&now));
  time[strlen(time) - 1] = '\0';

  return time;
}

void init_uptime(){
	time(&uptime);
}

unsigned long get_uptime(){
	time_t now;
	time(&now);
	
	return (now - uptime);	
}
