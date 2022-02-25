#include "../api.h"

static void	insert_into_db(char *str, int len)
{
	const char *id, *name, *genus, *family, *order, *nutritions;
	int			i, n, g, f, o, nu;
	mjson_find(str, len, "$.id", &id, &i);
	mjson_find(str, len, "$.name", &name, &n);
	mjson_find(str, len, "$.genus", &genus, &g);
	mjson_find(str, len, "$.family", &family, &f);
	mjson_find(str, len, "$.order", &order, &o);
	mjson_find(str, len, "$.nutritions", &nutritions, &nu);
	int size = snprintf(NULL, 0, "INSERT INTO fruit(id, name, genus, family, `order`, nutritions) \
			VALUES (%.*s, %.*s, %.*s, %.*s, %.*s, '%.*s');", i, id, n, name, g, genus, f, family, o, order, nu, nutritions);
	char *insert = malloc (size + 1);
	sprintf (insert, "INSERT INTO fruit(id, name, genus, family, `order`, nutritions) \
			VALUES (%.*s, %.*s, %.*s, %.*s, %.*s, '%.*s');", i, id, n, name, g, genus, f, family, o, order, nu, nutritions);
	if (mysql_query(conn, insert))
		printf ("Error inserting data into database\n");
	free (insert);
}

void	init_mysql_db(char *str)
{
	char server[16] = "localhost";
	char username[16] = "root";
	char password[16] = "root";

	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		printf("MySQL initialization failed");
		exit (1);
	}
	if (mysql_real_connect(conn, server, username, password, "MyDb", 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		printf("Unable to connect with MySQL server\n");
		mysql_close(conn);
		exit (1);
	}
	if (mysql_query(conn, "CREATE DATABASE MyDb"))
		printf("Unable to create database (or database already exists)\n");
	mysql_query(conn, "DROP TABLE fruit;");
	if (mysql_query(conn, "CREATE TABLE fruit (id INT NOT NULL AUTO_INCREMENT, name VARCHAR (50) NOT NULL, genus VARCHAR(50) NOT NULL, family VARCHAR(50) NOT NULL, `order` VARCHAR(50) NOT NULL, nutritions JSON, PRIMARY KEY (id));"))
		printf ("Table already exists\n");
	int koff, klen, voff, vlen, vtype, off;
	for (off = 0; (off = mjson_next(str, strlen(str), off, &koff, &klen, &voff, &vlen, &vtype)) != 0;)
	{
		insert_into_db(str + voff, vlen);
	}
}