#ifndef API_H
# define API_H

# define SERVER_IP "localhost:8000"
# define MYSQL_SERVER "localhost"
# define MYSQL_USER "root"
# define MYSQL_PASS "root"

# include "libft/libft.h"
# include "mongoose/mongoose.h"
# include "mjson/src/mjson.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <time.h>
# include <mysql.h>
# include <signal.h>
# include <curl/curl.h>

MYSQL* conn;

char	*get_api_data(void);
void	init_mysql_db(char *str);
int	insert_into_db(char *str, int len);
int		get_db_in_json(char **response);
int		get_id_in_json(char **response, char *id, int len);
int		delete_row_from_db(char *str, int len);
void	uri_root_path(struct mg_http_message *hm, int logfd, struct mg_connection *c);
void	uri_fruit_path(struct mg_http_message *hm, int logfd, struct mg_connection *c);
void	uri_id_path(struct mg_http_message *hm, int logfd, struct mg_connection *c);
int		check_post_request(char *str, int len);
void	response_code_400(int logfd, struct mg_connection *c);
void	response_code_404(int logfd, struct mg_connection *c);
void	response_code_405(int logfd, struct mg_connection *c);
void	response_code_500(int logfd, struct mg_connection *c);
void	response_code_200(int logfd, struct mg_connection *c, char *response);
void	response_code_201(int logfd, struct mg_connection *c, char *response);
char	*get_next_line(int fd);
int		str_check(const char *s1, const char *s2);
void	reset_mysql_db(void);
void	show_mysql_db(void);
void	db_show(void);

#endif