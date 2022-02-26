#include "../api.h"

void	response_code_400(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 400, NULL, "Bad request");
	dprintf (logfd, "RESPONSE CODE : 400\nRESPONSE HEADERS :\nRESPONSE BODY : Bad request\n\n\n\n");
}

void	response_code_404(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 404, NULL, "Not found");
	dprintf (logfd, "Response : HTTP Status 404 Not found\n\n\n\n");
}

void	response_code_405(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 405, NULL, "Method not allowed");
	dprintf (logfd, "Response : HTTP Status 405 Method not allowed\n\n\n\n");
}

void	response_code_500(int logfd, struct mg_connection *c)
{
	mg_http_reply(c, 500, NULL, "Internal server error");
	dprintf (logfd, "RESPONSE CODE : 500\nRESPONSE HEADERS :\nRESPONSE BODY : Internal server error\n\n\n\n");
}
