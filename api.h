#ifndef API_H
# define API_H

# define SERVER_IP "localhost:8000"

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
char	*get_db_in_json(void);
void	response_code_400(int logfd, struct mg_connection *c);
void	response_code_404(int logfd, struct mg_connection *c);
void	response_code_405(int logfd, struct mg_connection *c);
void	response_code_500(int logfd, struct mg_connection *c);

#endif