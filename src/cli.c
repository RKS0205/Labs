#include "../api.h"

void	log_uri(char *str, int uri_size)
{
	int fd = open("server.log", O_RDONLY);
	char *line, *time1, *time2, *time3, *temp;
	int	get = 0, post = 0, delete = 0;

	line = get_next_line(fd);
	time1 = ft_strdup("temp");
	time2 = ft_strdup("temp");
	time3 = ft_strdup("temp");
	while (line != NULL)
	{
		if (line == NULL)
			break ;
		if (strncmp(line, "REQUEST TIME : ", 15) == 0)
			temp = strdup(line);
		if (str_check(line, "REQUEST METHOD : GET"))
		{
			line = get_next_line(fd);
			if (str_check(line, str) || str_check(ft_strjoin(str, "/\n"), line))
			{
				time1 = temp;
				get++;
			}
		}
		else if (str_check(line, "REQUEST METHOD : POST"))
		{
			line = get_next_line(fd);
			if (str_check(line, str) || str_check(ft_strjoin(str, "/\n"), line))
			{
				time2 = temp;
				post++;
			}
		}
		else if (str_check(line, "REQUEST METHOD : DELETE"))
		{
			line = get_next_line(fd);
			if (str_check(line, str) || str_check(ft_strjoin(str, "/\n"), line))
			{
				time3 = temp;
				delete++;
			}
		}
		line = get_next_line(fd);
	}
	close (fd);
	if (get > 0)
		printf ("%-*s| GET         | %-*s| %s", 15, ft_itoa(get), uri_size, ft_strtrim(str + 14, "\n"), time1 + 15);
	if (post > 0)
		printf ("%-*s| POST        | %-*s| %s", 15, ft_itoa(post), uri_size, ft_strtrim(str + 14, "\n"), time2 + 15);
	if (delete > 0)
		printf ("%-*s| DELETE      | %-*s| %s", 15, ft_itoa(delete), uri_size, ft_strtrim(str + 14, "\n"), time3 + 15);
}

char	**get_all_uri(void)
{
	int fd = open("server.log", O_RDONLY);
	char *line, *buff;
	char	**uri;
	int		n, uri_count = 0;

	uri = malloc (sizeof(char *) * 100);
	buff = get_next_line(fd);
	while (buff != NULL)
	{
		if (str_check(buff, "REQUEST URI : /"))
			line = buff;
		else
		{
			line = ft_strtrim(buff, "/\n");
			line = ft_my_strjoin(line, "\n");
			free (buff);
		}
		if (strncmp (line, "REQUEST URI : ", 14) == 0)
		{
			n = 0;
			while (1)
			{
				if (n == uri_count)
				{
					uri[n] = malloc (strlen(line) + 1);
					strcpy (uri[n], line);
					uri_count++;
					break ;
				}
				if (str_check(line, uri[n]))
					break ;
				n++;
			}
		}
		free (line);
		buff = get_next_line(fd);
	}
	if (uri_count == 0)
	{
		close (fd);
		return (NULL);
	}
	uri[uri_count] = NULL;
	close (fd);
	return (uri);
}

void	do_log(void)
{
	char **uri = get_all_uri();
	int n = 0;
	int	uri_size = 0;
	if (uri == NULL)
		printf ("No requests logged\n");
	else
	{	
		while (uri[n] != NULL)
		{
			if (strlen(uri[n] + 14) > uri_size)
				uri_size = strlen(uri[n] + 14);
			n++;
		}
		n = 0;
		printf ("\nTOTAL REQUESTS | HTTP METHOD | %-*s| LATEST REQUEST\n", uri_size, "PATH");
		while (uri[n] != NULL)
		{
			log_uri(ft_strtrim(uri[n], "\n"), uri_size);
			free (uri[n++]);
		}
		printf ("\n");
	}
	free (uri);
}

int	main(void)
{
	char	*line;
	while (1)
	{
		write (1, ">", 1);
		line = get_next_line(1);
		if (str_check(line, "log"))
			do_log();
		else if (str_check(line, "help"))
			printf ("Available commands:\n help\n log\n log delete\n exit\n db reset\n db show\n");
		else if (str_check(line, "exit"))
		{
			free (line);
			exit (0);
		}
		else if (str_check(line, "db reset"))
			reset_mysql_db();
		else if(str_check(line, "db show"))
			db_show();
		else if(str_check(line, "log delete"))
			fclose(fopen("server.log", "w"));
		else
			printf("Command not found\n");
		free (line);
	}
}