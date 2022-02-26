#include "../api.h"

int loop = 1;

int	check_post_request(char *str, int len)
{
	const char *id, *name, *genus, *family, *order, *nutritions, *n_carb, *n_prot, *n_fat, *n_cal, *n_sug;
	int			i, n, g, f, o, nu, nc, np, nf, ncal, nsug, count;
	count = 0;    /*count is storing the ammount of ":" in the string in order to determine the number of elements in the string*/
	if (mjson_find(str, len, "$.id", &id, &i) != MJSON_TOK_NUMBER)
		count++;
	if (mjson_find(str, len, "$.name", &name, &n) != MJSON_TOK_STRING)
		return 0;
	else if (mjson_find(str, len, "$.genus", &genus, &g) != MJSON_TOK_STRING)
		return 0;
	else if (mjson_find(str, len, "$.family", &family, &f) != MJSON_TOK_STRING)
		return 0;
	else if (mjson_find(str, len, "$.order", &order, &o) != MJSON_TOK_STRING)
		return 0;
	else if (mjson_find(str, len, "$.nutritions", &nutritions, &nu) != MJSON_TOK_OBJECT)
		return 0;
	else if (mjson_find(str, len, "$.nutritions.carbohydrates", &n_carb, &nc) != MJSON_TOK_NUMBER)
		return 0;
	else if (mjson_find(str, len, "$.nutritions.protein", &n_prot, &np) != MJSON_TOK_NUMBER)
		return 0;
	else if (mjson_find(str, len, "$.nutritions.fat", &n_fat, &nf) != MJSON_TOK_NUMBER)
		return 0;
	else if (mjson_find(str, len, "$.nutritions.calories", &n_cal, &ncal) != MJSON_TOK_NUMBER)
		return 0;
	else if (mjson_find(str, len, "$.nutritions.sugar", &n_sug, &nsug) != MJSON_TOK_NUMBER)
		return 0;
	while (len > 0)
	{
		if (str[len] == ':')
			count++;
		len--;
	}
	if (count != 11)
		return 0;
	return 1;
}

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
			char *response = get_db_in_json();
			if (response == NULL)
				response_code_500(logfd, c);
			else
			{
				mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", response);
				dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : %s\n\n\n\n", response);
			}
			free (response);
		}
		else if (mg_http_match_uri(hm, "/") && strncmp(hm->method.ptr, "POST", hm->method.len) == 0)
		{
			if (check_post_request((char *)hm->body.ptr, hm->body.len) && insert_into_db((char *)hm->body.ptr, hm->body.len))
			{
				char *response = get_db_in_json();
				if (response == NULL)
					response_code_500(logfd, c);
				mg_http_reply(c, 201, "Content-Type: application/json\r\n", "%s", response);
				dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : %s\n\n\n\n", response);
				free (response);
			}
			else
				response_code_400(logfd, c);
		}
		else if (mg_http_match_uri(hm, "/"))
			response_code_405(logfd, c);
		else
			response_code_404(logfd, c);
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
