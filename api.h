#ifndef API_H
# define API_H

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

#endif