#include "../api.h"

int loop = 1;

void	log_request(struct mg_http_message *hm, int logfd)
{
	int header_size;
	time_t current_time = time(NULL);
	char *time_string = ctime(&current_time);

	header_size = hm->message.len - (hm->body.len + hm->method.len + hm->uri.len + hm->query.len + hm->proto.len + 7);
	dprintf(logfd, "REQUEST TIME : %s", time_string);
	dprintf(logfd, "REQUEST METHOD : %.*s\n", (int)hm->method.len, hm->method.ptr);
	dprintf(logfd, "REQUEST URI : %.*s\n", (int)hm->uri.len, hm->uri.ptr);
	dprintf(logfd, "REQUEST HEADERS :\n%.*s\n", header_size, hm->headers->name.ptr);
	dprintf(logfd, "REQUEST BODY :\n%.*s\n\n", (int)hm->body.len, hm->body.ptr);
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
	if (ev == MG_EV_HTTP_MSG)
	{
		int logfd = open ("server.log", O_WRONLY | O_APPEND);
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		log_request(hm, logfd);
		if (mg_http_match_uri(hm, "/") && strncmp(hm->method.ptr, "GET", hm->method.len) == 0)
		{
			if (mysql_query(conn, "SELECT JSON_ARRAYAGG(JSON_OBJECT('id', id, 'name', name, 'genus', genus, \
					'family', family, 'order', `order`, 'nutritions', nutritions)) from fruit;"))
				dprintf(2, "Error extracting data from database\n");
			MYSQL_RES *result = mysql_store_result(conn);
			MYSQL_ROW row = mysql_fetch_row(result);
			char *response = ft_strtrim (row[0], "[]");
			mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", response);
			dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : %s\n\n\n\n", response);
			free (response);
		}
		else if (mg_http_match_uri(hm, "/") && strncmp(hm->method.ptr, "POST", hm->method.len) == 0)
		{
			const char *name;
			const char *age;
			int a;
			int n;
			if (mjson_find(hm->body.ptr, hm->body.len, "$.name", &name, &n) == MJSON_TOK_STRING
				&& mjson_find(hm->body.ptr, hm->body.len, "$.age", &age, &a))
			{
				mg_http_reply(c, 400, NULL, "Bad request");
			}
			else
			{
				mg_http_reply(c, 400, NULL, "Bad request");
				dprintf (logfd, "RESPONSE CODE : 400\nRESPONSE HEADERS :\nRESPONSE BODY : Bad request\n\n\n\n");
			}
		}
		else if (mg_http_match_uri(hm, "/") && strncmp(hm->method.ptr, "GET", hm->method.len) != 0)
		{
			mg_http_reply(c, 405, NULL, "Method not allowed");
			dprintf (logfd, "Response : HTTP Status 405 Method not allowed\n\n");
		}
		else
		{
			mg_http_reply(c, 404, NULL, "Not found");
			dprintf (logfd, "Response : HTTP Status 404 Not foundn\n\n");
		}
		fflush(stdout);
	}
}

void	close_command (int signum)
{
	(void) signum;
	loop = 0;
}

int main()
{
	struct mg_mgr mgr;
	char *str = get_api_data();
	init_mysql_db(str);
	free (str);
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "localhost:8000", fn, &mgr);
	signal (SIGINT, close_command);
	printf ("Server started\n");
	while (loop)
		mg_mgr_poll(&mgr, 1000);
	mg_mgr_free(&mgr);
	mysql_library_end();
	mysql_close(conn);
	printf ("\nClosing...\n");
	return 0;
}
