#include "webserver.h"

// Set constant cmd_opt 
const CMD_OPTS cmd_opts;
// Set MHD daemon
struct MHD_Daemon *mhd_daemon;
// Set Log file
FILE *LOG;
// Set Log file name
const char *LOG_NAME = "/info.log";
// Set number of connections
static int num_of_connections = 0;
// Set client ip
static char *ip_addr;
// Set Status Responses
const StatusResponse Ok = { 
	200, "Ok"
};
const StatusResponse NotFound = {
	404, "Bad Request - Invalid client request for web resource"
};
const StatusResponse InvalidMethod = {
	405, "Method Not Allowed - HTTP verb used to access this page is not allowed"
};
const StatusResponse InternalError = {
	500, "Server error - An internal error has happened in server"
};
const StatusResponse ServiceUnavailable = {
	503, "Server error - Too many concurrent request"
};


static int print_out_key(void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
    fprintf(LOG, "%s: %s\n", key, value);
    return MHD_YES;
}

static int on_client_connect(void *cls, const struct sockaddr *addr, socklen_t addrlen){
    struct sockaddr_in *addr_in = (struct sockaddr_in *) addr;
    // fprintf(LOG, "====================================================\n");
    // fprintf(LOG, "Connection Number: %d\n", ++num_of_connections);
    // fprintf(LOG, "Uptime: %ld\n", get_uptime());
    // fprintf(LOG, "IP Client: %s\n", );
    
    ip_addr = inet_ntoa(addr_in->sin_addr);

    return MHD_YES;
}
  
static int answer_to_connection(void *cls, struct MHD_Connection *connection, const char *url, 
  const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls){
	static int aptr;
	if (&aptr != *con_cls){
		*con_cls = &aptr;
		return MHD_YES;
	}
	
    if ( 0 == strcmp(url, "/shutdown")) { // check for shutdown
		stop_webserver();
	}
	  	
	int fd = 0;
    int ret = 0;
    struct stat sbuff;
    struct MHD_Response *response;
    StatusResponse statusResponse = Ok;    
	
    const char *path = url;
    if (1 == strlen(path)){
		path = "/index.html";
	}        
    char full_path[strlen(cmd_opts.dir) + strlen(path) + 1];
    strcpy(full_path, cmd_opts.dir);
    strcat(full_path, path);    
    
    if ( !( 0 == strcmp(method, "GET") || 0 == strcmp(method, "POST")) ) {
		statusResponse = InvalidMethod;
		response = MHD_create_response_from_buffer(strlen(statusResponse.msg), (void *) statusResponse.msg, MHD_RESPMEM_PERSISTENT);	
    } else if ( -1 == (fd = open(full_path, O_RDONLY))) {
		statusResponse = NotFound;
		response = MHD_create_response_from_buffer(strlen(statusResponse.msg), (void *) statusResponse.msg, MHD_RESPMEM_PERSISTENT);
	} else if ( 0 != fstat(fd, &sbuff)) {
		statusResponse = InternalError;
		response = MHD_create_response_from_buffer(strlen(statusResponse.msg), (void *) statusResponse.msg, MHD_RESPMEM_PERSISTENT);
		if (fd != -1) {
            close(fd);
        }
	} else {
		fprintf(LOG, "===================== REQUEST =====================\n");
		fprintf(LOG, "Connection Number: %d\n", ++num_of_connections);
		fprintf(LOG, "Uptime: %ld\n", get_uptime());
		fprintf(LOG, "IP Client: %s\n", ip_addr);
		
		fprintf(LOG, "Current time: %s\n", (char *) cls);
		fprintf(LOG, "Path: %s\n", url);
		fprintf(LOG, "HTTP Method: %s\n", method);   
		fprintf(LOG, "HTTP Version: %s\n", version);
		
		fprintf(LOG, "<Headers>\n");
		MHD_get_connection_values(connection, MHD_HEADER_KIND, &print_out_key, NULL);
		fprintf(LOG, "<Cookies>\n");
		MHD_get_connection_values(connection, MHD_COOKIE_KIND, &print_out_key, NULL);
		fprintf(LOG, "<Query Params>\n");
		MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, &print_out_key, NULL);
				
		response = MHD_create_response_from_fd_at_offset64(sbuff.st_size, fd, 0);
	}		
	ret = MHD_queue_response(connection, statusResponse.code, response);
	MHD_destroy_response(response);
	
	fprintf(LOG, "===================== RESPONSE =====================\n");
	fprintf(LOG, "%d %s\n", statusResponse.code, statusResponse.msg);
    fflush(LOG);
    
	return ret;
}

void start_webserver() {
	mhd_daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, cmd_opts.port, &on_client_connect, NULL,
		&answer_to_connection, (void *) curr_str_time(), NULL, MHD_OPTION_END);
    
    if (NULL == mhd_daemon){
		fprintf(stdout, "Cannot start up webserver.\n");
		exit(1);
	}
	
	char log_full_path[strlen(cmd_opts.dir) + strlen(LOG_NAME) + 1];
	strcpy(log_full_path, cmd_opts.dir);
	strcat(log_full_path, LOG_NAME);
	LOG = fopen(log_full_path, "w");
	
	if (NULL == LOG){
		fprintf(stdout, "Cannot open log file.\n");
		exit(1);
	}
		
	getchar();
	MHD_stop_daemon(mhd_daemon);
}

void stop_webserver() {
	fclose(LOG);
	MHD_stop_daemon(mhd_daemon);
	
	fprintf(stdout, "Shutdown webserver.\n");
	exit(0);
}

/*
 * Main entry of program.
 */ 
int main(int argc, char *argv[]){
  int cmd_result = get_cmd_opts(&cmd_opts, argc, argv);
  
  if (0 != cmd_result) { 
	  exit(1);
  }
  
  init_uptime();
  start_webserver();    
  return 0;
}
