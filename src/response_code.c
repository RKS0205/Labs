#include "../api.h"

void	response_code_200(int logfd, struct mg_connection *c, char *response)
{
	mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", response);
	dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : %s\n\n\n\n", response);
	free (response);
}

void	response_code_201(int logfd, struct mg_connection *c, char *response)
{
	mg_http_reply(c, 201, "Content-Type: application/json\r\n", "%s", response);
	dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : %s\n\n\n\n", response);
	free (response);
}

void	response_code_400(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 400, NULL, "Bad request");
	dprintf (logfd, "RESPONSE CODE : 400\nRESPONSE HEADERS :\nRESPONSE BODY : Bad request\n\n\n\n");
}

void	response_code_404(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 404, NULL, "Not found");
	dprintf (logfd, "RESPONSE CODE : 404\nRESPONSE HEADERS :\nRESPONSE BODY : Not found\n\n\n\n");
}

void	response_code_405(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 405, NULL, "Method not allowed");
	dprintf (logfd, "RESPONSE CODE : 405\nRESPONSE HEADERS :\nRESPONSE BODY : Method not allowed\n\n\n\n");
}

void	response_code_500(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 500, NULL, "Internal server error");
	dprintf (logfd, "RESPONSE CODE : 500\nRESPONSE HEADERS :\nRESPONSE BODY : Internal server error\n\n\n\n");
}
