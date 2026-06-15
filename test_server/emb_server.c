/*

compile - gcc emb_server.c civetweb.c -I. -DNO_SSL -lws2_32 -o emb_server.exe

 * Copyright (c) 2013-2021 the CivetWeb developers
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

/* Note: This example omits some error checking and input validation for a
 * better clarity/readability of the code. Example codes undergo less quality
 * management than the main source files of this project. */


/* Simple example program on how to use CivetWeb embedded into a C program. */
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "civetweb.h"


#define DOCUMENT_ROOT "."
#define PORT "8888,8884"
#define EXIT_URI "/exit"

volatile int exitNow = 0;



int
ExitHandler(struct mg_connection *conn, void *cbdata)
{
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: "
	          "text/plain\r\nConnection: close\r\n\r\n");
	mg_printf(conn, "Server will shut down.\n");
	mg_printf(conn, "Bye!\n");
	exitNow = 1;
	return 1;
}


int
BXHandler(struct mg_connection *conn, void *cbdata)
{
	/* Handler may access the request info using mg_get_request_info */
	const struct mg_request_info *req_info = mg_get_request_info(conn);
	const char *text = (const char *)cbdata;

	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: "
	          "close\r\n\r\n");
	mg_printf(conn, "<html><body>");
	mg_printf(conn, "<h2>This is the BX handler with argument %s.</h2>", text);
	mg_printf(conn, "<p>The actual uri is %s</p>", req_info->local_uri);
	mg_printf(conn, "</body></html>\n");
	return 1;
}

int
FileHandler(struct mg_connection *conn, void *cbdata)
{
	/* In this handler, we ignore the req_info and send the file "fileName". */
	const char *fileName = (const char *)cbdata;

	mg_send_file(conn, fileName);
	return 1;
}

int
PostResponser(struct mg_connection *conn, void *cbdata)
{
	long long r_total = 0;
	int r, s;

	char buf[2048];
  memset(buf, 0, 2048);

	const struct mg_request_info *ri = mg_get_request_info(conn);

	if (0 != strcmp(ri->request_method, "POST")) {
		/* Not a POST request */
		int ret = mg_get_request_link(conn, buf, sizeof(buf));

		mg_printf(conn,
		          "HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\n");
		mg_printf(conn, "Content-Type: text/plain\r\n\r\n");
		mg_printf(conn,
		          "%s method not allowed in the POST handler\n",
		          ri->request_method);
		if (ret >= 0) {
			mg_printf(conn,
			          "use a web tool to send a POST request to %s\n",
			          buf);
		}
		return 1;
	}

	if (ri->content_length >= 0) {
		/* We know the content length in advance */
	} else {
		/* We must read until we find the end (chunked encoding
		 * or connection close), indicated my mg_read returning 0 */
	}

	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "Content-Type: Content-Type: application/json\r\n\r\n");
	mg_printf(conn, "[1.0,");
	mg_printf(conn, "2.2]\r\n");
  
	r = mg_read(conn, buf, sizeof(buf));
	while (r > 0) {
		r_total += r;
		//s = mg_send_chunk(conn, buf, r);
		//if (s <= 0) {
			/* Send error */
		//	break;
		//}
    printf("%s\n", buf);
		r = mg_read(conn, buf, sizeof(buf));
	}
	//mg_printf(conn, "0\r\n");

	return 1;
}


int
log_message(const struct mg_connection *conn, const char *message)
{
	puts(message);
	return 1;
}


int
main(int argc, char *argv[])
{
	const char *options[] = {
		"document_root",
		DOCUMENT_ROOT,
		"listening_ports",
		PORT,
		"request_timeout_ms",
		"10000",
		"error_log_file",
		"error.log",

		"enable_auth_domain_check",
		"no",
		0
	};
	struct mg_callbacks callbacks;
	struct mg_context *ctx;
	struct mg_server_port ports[32];
	int port_cnt, n;
	

	/* Start CivetWeb web server */
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = log_message;
	ctx = mg_start(&callbacks, 0, options);

	/* Check return value: */
	if (ctx == NULL) {
		fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
		return EXIT_FAILURE;
	}

	mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);


	/* Add handler for /B, /B/A, /B/B but not for /B* */
	mg_set_request_handler(ctx, "/B$", BXHandler, (void *)"ze");
	mg_set_request_handler(ctx, "/B/A$", BXHandler, (void *)"cruel");
	mg_set_request_handler(ctx, "/B/B$", BXHandler, (void *)"gamma");


	mg_set_request_handler(ctx,
	                       "/plot",
	                       FileHandler,
	                       (void *)"./teste_plot.html");


	/* Add handler for /postresponse example */
	mg_set_request_handler(ctx, "/postresponse", PostResponser, 0);

	
	/* List all listening ports */
	memset(ports, 0, sizeof(ports));
	port_cnt = mg_get_server_ports(ctx, 32, ports);
	printf("\n%i listening ports:\n\n", port_cnt);

	for (n = 0; n < port_cnt && n < 32; n++) {
		const char *proto = ports[n].is_ssl ? "https" : "http";
		const char *host;

		if ((ports[n].protocol & 1) == 1) {
			/* IPv4 */
			host = "127.0.0.1";
			printf("Browse files at %s://%s:%i/\n", proto, host, ports[n].port);
			
			printf(
			    "Exit at %s://%s:%i%s\n", proto, host, ports[n].port, EXIT_URI);
			printf("\n");
		}

	}

	/* Wait until the server should be closed */
	while (!exitNow) {
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	/* Stop the server */
	mg_stop(ctx);
	printf("Server stopped.\n");
	printf("Bye!\n");

	return EXIT_SUCCESS;
}
