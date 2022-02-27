#include "../api.h"

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
