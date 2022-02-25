#include "../api.h"

struct MemoryStruct
{
	char *str;
	size_t size;
};
 
static size_t write_function(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->str, mem->size + realsize + 1);
	if(!ptr)
	{
		printf("Realloc failed\n");
		exit (0);
	}
	mem->str = ptr;
	memcpy(&(mem->str[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->str[mem->size] = 0;
	return realsize;
}
 
char	*get_api_data(void)
{
	CURL *curl_handle;
	CURLcode res;
	struct MemoryStruct mem;

	mem.str = malloc(1);
	mem.size = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "https://fruityvice.com/api/fruit/all");
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_function);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	if(curl_easy_perform(curl_handle) != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return (mem.str);
}
