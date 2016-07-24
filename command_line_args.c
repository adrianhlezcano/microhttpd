#include "command_line_args.h"

/* The name of the program */
const char *program_name;
const char* const short_options = "hp:d:";
const struct option long_options[] = { 
	{"help",  0, NULL, 'h'},
	{"port",  1, NULL, 'p'},
	{"dir",   1, NULL, 'd'},
	{NULL,    0, NULL,  0 },
};

void 
print_usage() {
	 fprintf(stdout, "Usage: %s -p [PORT] -d [DIR] \n", program_name);
	 fprintf(stdout, "Start http web server in specific port and www-dir.\n\n");
	 fprintf(stdout, 
		"  -h   --help        Display this usage information.\n"
		"  -p   --port PORT   Specify port of webserver.\n"
		"  -d   --dir DIR     Specify root dir of web pages in webserver.\n");   
}

int get_cmd_opts(CMD_OPTS *cmd_opts, const int argc, char *argv[]){	
	int no_opts = 2;
	int next_option;
	program_name = argv[0];
	
	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);
		switch (next_option) {
			case 'p':
				cmd_opts->port = atoi(optarg);
				no_opts -= 1;
				break;
			case 'd':
				cmd_opts->dir = optarg;
				no_opts -= 1;
				break;			
			case 'h':
			case '?':
				print_usage();
			case -1:
				break;
			default:
				abort();
		}
	} while (next_option != -1);

	if ( 1 <= no_opts) {
		print_usage();
	}	
	return no_opts;
}
