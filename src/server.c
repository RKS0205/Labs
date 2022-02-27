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
		if (mg_http_match_uri(hm, "/"))
			uri_root_path(hm, logfd, c);
		else if (mg_http_match_uri(hm, "/fruit") || mg_http_match_uri(hm, "/fruit/"))
			uri_fruit_path(hm, logfd, c);
		else if (strncmp(hm->body.ptr, "/fruit/", 7))
			uri_id_path(hm, logfd, c);
		else
			response_code_404(logfd, c);
	}
}

void	close_command (int signum)
{
	(void) signum;
	loop = 0;
}

int main(void)
{
	struct mg_mgr mgr;
	char *str = get_api_data();
	init_mysql_db(str);
	free (str);
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, SERVER_IP, fn, &mgr);
	signal (SIGINT, close_command);
	printf ("Server started\n");
	while (loop)
		mg_mgr_poll(&mgr, 1000);
	printf ("\nClosing...\n");
	mg_mgr_free(&mgr);
	mysql_library_end();
	mysql_close(conn);
	return 0;
}
