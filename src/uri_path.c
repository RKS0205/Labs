#include "../api.h"

void	uri_root_path(struct mg_http_message *hm, int logfd, struct mg_connection *c)
{
	if (strncmp(hm->method.ptr, "GET", hm->method.len) == 0)
	{
		mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"fruit\":\"%s/fruit\", \"id\":\"%s/fruit/id\"}", SERVER_IP, SERVER_IP);
		dprintf (logfd, "RESPONSE CODE : 200\nRESPONSE HEADERS : Content-Type: application/json\nRESPONSE BODY : {\"fruit\":\"%s/fruit\", \"id\":\"%s/fruit/id\"}\n\n\n\n", SERVER_IP, SERVER_IP);
	}
	else
		response_code_405(logfd, c);
}

void	uri_fruit_path(struct mg_http_message *hm, int logfd, struct mg_connection *c)
{
	if (strncmp(hm->method.ptr, "GET", hm->method.len) == 0)
	{
		char *response;
		if (get_db_in_json(&response) == 500)
			response_code_500(logfd, c);
		else
			response_code_200(logfd, c, response);
	}
	else if (strncmp(hm->method.ptr, "POST", hm->method.len) == 0)
	{
		if (check_post_request((char *)hm->body.ptr, hm->body.len) && insert_into_db((char *)hm->body.ptr, hm->body.len))
		{
			char *response;
			if (get_db_in_json(&response) == 500)
				response_code_500(logfd, c);
			else
				response_code_201(logfd, c, response);
		}
		else
			response_code_400(logfd, c);
	}
	else
		response_code_405(logfd, c);
}

void	uri_id_path(struct mg_http_message *hm, int logfd, struct mg_connection *c)
{
	char	*response;
	int		code = get_id_in_json(&response, (char *)hm->uri.ptr + 7, hm->uri.len - 7);
	if (code == 500)
		response_code_500(logfd, c);
	else if (code == 404)
		response_code_404(logfd, c);
	else
	{
		if (strncmp(hm->method.ptr, "GET", hm->method.len) == 0)
			response_code_200(logfd, c, response);
		if (strncmp(hm->method.ptr, "DELETE", hm->method.len) == 0)
		{
			if (delete_row_from_db((char *)hm->uri.ptr + 7, hm->uri.len - 7) == 500)
				response_code_500(logfd, c);
			else
				response_code_200(logfd, c, strdup("{\"resource\": \"deleted\"}"));
		}
		else
			response_code_405(logfd, c);
	}
}