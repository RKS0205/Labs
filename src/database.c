#include "../api.h"

int	get_db_in_json(char **response)
{
	if (mysql_query(conn, "SELECT JSON_ARRAYAGG(JSON_OBJECT('id', id, 'name', name, 'genus', genus, \
			'family', family, 'order', `order`, 'nutritions', nutritions)) from fruit;"))
		return (500);
	MYSQL_RES *result = mysql_store_result(conn);
	MYSQL_ROW row = mysql_fetch_row(result);
	*response = ft_strtrim (row[0], "[]");
	mysql_free_result(result);
	return (1);
}

int	get_id_in_json(char **response, char *id, int len)
{
	int size = snprintf(NULL, 0, "SELECT JSON_ARRAYAGG(JSON_OBJECT('id', id, 'name', name, 'genus', genus, \
			'family', family, 'order', `order`, 'nutritions', nutritions)) from fruit WHERE FIND_IN_SET (\'%.*s\', id);", len, id);
	char *insert = malloc (size + 1);
	sprintf (insert, "SELECT JSON_ARRAYAGG(JSON_OBJECT('id', id, 'name', name, 'genus', genus, \
			'family', family, 'order', `order`, 'nutritions', nutritions)) from fruit WHERE FIND_IN_SET (\'%.*s\', id);", len, id);
	if (mysql_query(conn, insert))
	{
		free (insert);
		return (500);
	}
	free (insert);
	MYSQL_RES *result = mysql_store_result(conn);
	MYSQL_ROW row = mysql_fetch_row(result);
	if (row[0] == NULL)
		return (404);
	*response = ft_strtrim (row[0], "[]");
	mysql_free_result(result);
	return (200);
}

int	delete_row_from_db(char *str, int len)
{
	int	size = snprintf(NULL, 0, "DELETE FROM fruit WHERE `id` IN (%.*s)", len, str);
	char *insert = malloc (size + 1);
	sprintf (insert, "DELETE FROM fruit WHERE `id` IN (%.*s)", len, str);
	if (mysql_query(conn, insert))
	{
		free (insert);
		return (500);
	}
	free (insert);
	return (1);
}

int	insert_into_db(char *str, int len)
{
	const char *id, *name, *genus, *family, *order, *nutritions;
	char 		*insert;
	int			i, n, g, f, o, nu;

	mjson_find(str, len, "$.name", &name, &n);
	mjson_find(str, len, "$.genus", &genus, &g);
	mjson_find(str, len, "$.family", &family, &f);
	mjson_find(str, len, "$.order", &order, &o);
	mjson_find(str, len, "$.nutritions", &nutritions, &nu);
	if (mjson_find(str, len, "$.id", &id, &i) == MJSON_TOK_NUMBER)
	{
		int size = snprintf(NULL, 0, "INSERT INTO fruit(id, name, genus, family, `order`, nutritions) \
				VALUES (%.*s, %.*s, %.*s, %.*s, %.*s, '%.*s');", i, id, n, name, g, genus, f, family, o, order, nu, nutritions);
		insert = malloc (size + 1);
		sprintf (insert, "INSERT INTO fruit(id, name, genus, family, `order`, nutritions) \
				VALUES (%.*s, %.*s, %.*s, %.*s, %.*s, '%.*s');", i, id, n, name, g, genus, f, family, o, order, nu, nutritions);
	}
	else
	{
		int size = snprintf(NULL, 0, "INSERT INTO fruit(name, genus, family, `order`, nutritions) \
				VALUES (%.*s, %.*s, %.*s, %.*s, '%.*s');", n, name, g, genus, f, family, o, order, nu, nutritions);
		insert = malloc (size + 1);
		sprintf (insert, "INSERT INTO fruit(name, genus, family, `order`, nutritions) \
				VALUES (%.*s, %.*s, %.*s, %.*s, '%.*s');", n, name, g, genus, f, family, o, order, nu, nutritions);	
	}
	if (mysql_query(conn, insert))
	{
		free (insert);
		return 0;
	}
	free (insert);
	return 1;
}

void	init_mysql_db(char *str)
{
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("MySQL initialization failed");
		exit (1);
	}
	if (mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASS, "MyDb", 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		printf("Unable to connect with MySQL server\n");
		mysql_close(conn);
		exit (1);
	}
	if (mysql_query(conn, "CREATE DATABASE MyDb"))
		printf("Unable to create database (or database already exists)\n");
	if (mysql_query(conn, "CREATE TABLE fruit (id INT NOT NULL AUTO_INCREMENT, name VARCHAR (50) NOT NULL, \
			genus VARCHAR(50) NOT NULL, family VARCHAR(50) NOT NULL, `order` VARCHAR(50) NOT NULL, nutritions JSON, \
			PRIMARY KEY (id), UNIQUE (family, name, genus, `order`));"))
		printf ("Table already exists\n");
	int koff, klen, voff, vlen, vtype, off;
	for (off = 0; (off = mjson_next(str, strlen(str), off, &koff, &klen, &voff, &vlen, &vtype)) != 0;)
		insert_into_db(str + voff, vlen);
}

void	db_show(void)
{
	int size = snprintf(NULL, 0, "mysql MyDb -u%s -p%s -e \"SELECT * FROM fruit;\"", MYSQL_USER, MYSQL_PASS);
	char *command = malloc (size + 1);
	sprintf (command, "mysql MyDb -u%s -p%s -e \"SELECT * FROM fruit;\"", MYSQL_USER, MYSQL_PASS);
	printf ("%s", command);
	fflush(stdout);
	system(command);
}

void	reset_mysql_db(void)
{
	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, MYSQL_SERVER, MYSQL_USER, MYSQL_PASS, NULL, 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		printf("Unable to connect with MySQL server\n");
		mysql_close(conn);
		exit (1);
	}
	if (mysql_query(conn, "DROP DATABASE MyDb"))
		printf ("Failed to delete database\n");
	if (mysql_query(conn, "CREATE DATABASE MyDb"))
		printf("Unable to create database (or database already exists)\n");
	if (mysql_query(conn, "USE MyDb"))
		printf("Unable to access database\n");
	if (mysql_query(conn, "CREATE TABLE fruit (id INT NOT NULL AUTO_INCREMENT, name VARCHAR (50) NOT NULL, \
			genus VARCHAR(50) NOT NULL, family VARCHAR(50) NOT NULL, `order` VARCHAR(50) NOT NULL, nutritions JSON, \
			PRIMARY KEY (id), UNIQUE (family, name, genus, `order`));"))
		printf ("Table already exists\n");
	char *str = get_api_data();
	int koff, klen, voff, vlen, vtype, off;
	for (off = 0; (off = mjson_next(str, strlen(str), off, &koff, &klen, &voff, &vlen, &vtype)) != 0;)
		insert_into_db(str + voff, vlen);
	printf ("Database reset successful\n");
	mysql_close(conn);
}
