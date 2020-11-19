#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "cJSON.h"
struct string {
  char *ptr;
  size_t len;
};
char* replaceWord(const char* s, const char* oldW, 
                  const char* newW) 
{ 
    char* result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++;
            i += oldWlen - 1; 
        } 
    } 
 result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);   
    i = 0; 
    while (*s) { 
        if (strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
} 
void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
void parse_json(cJSON *reddit_json, int num1, int num2){
        const cJSON *data = NULL;
        const cJSON *children = NULL;
        cJSON *post_number = NULL;
        cJSON *Data = NULL;
        cJSON *Title = NULL;
        cJSON *Text = NULL;
	char c[] = "\\n";
	char d[] = "\n";
	char *result = NULL;
        data = cJSON_GetObjectItemCaseSensitive(reddit_json, "data");
        children = cJSON_GetObjectItemCaseSensitive(data, "children");
	char *reddit_title;
	char *reddit_text;
        for(int number = num1; number < num2; number++){
        post_number = cJSON_GetArrayItem(children, number);
        Data = cJSON_GetObjectItemCaseSensitive(post_number, "data");
        Title = cJSON_GetObjectItemCaseSensitive(Data, "title");
        Text = cJSON_GetObjectItemCaseSensitive(Data, "selftext");
        reddit_title = cJSON_Print(Title);
	reddit_text = cJSON_Print(Text);
	printf("\033[1;34m");
        printf("%s:\n", reddit_title);
	printf("\033[0;32m");
	result = replaceWord(reddit_text, c, d);
	printf("%s\n\n", result);
        }
	free(reddit_title);
	free(reddit_text);
	free(result);
}
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int main(int argc, char *argv[])
{
	char *time;
	char *sort;
	if(argc < 5){
puts("Usage:\nreddit-get SUBREDDIT SORT POST-RANGE\nSUBREDDIT can be any subreddit on reddit\nSORT can be: tt(top today), tn(top now), tw(top this week), tm(top this month), ty(top this year), ta(top all time), h(hot), and n(new)\nPOST-RANGE can be any range of posts\nthis example gets 5 posts from r/copypasta top all time: reddit-get copypasta ta 0 5");
exit(0);
	}
	if(argc > 5){
puts("too many args");
exit(0);
	}
	if (strcmp(argv[2], "tt") == 0){
sort = "top";
time = "t=day&";
	}
	if(strcmp(argv[2], "tn") == 0){
sort = "top";
time = "t=hour&";
	}
	if(strcmp(argv[2], "tw") == 0){
sort = "top";
time = "t=week&";
        }
	if(strcmp(argv[2], "tm") == 0){
sort = "top";
time = "t=month&";
        }
	if(strcmp(argv[2], "ty") == 0){
sort = "top";
time = "t=year&";
        }
	if(strcmp(argv[2], "ta") == 0){
sort = "top";
time = "t=all&";
        }
	if(strcmp(argv[2], "h") == 0){
sort = "hot";
time = "";
        }
	if(strcmp(argv[2], "n") == 0){
sort = "new";
time = "";
        }
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);
char url[150];
sprintf(url, "https://www.reddit.com/r/%s/%s/.json?%sshowmedia=false&mediaonly=false&is_self=true&limit=%s", argv[1], sort, time, argv[4]);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);
    int i;
    cJSON *reddit_json = cJSON_Parse(s.ptr);
    parse_json(reddit_json, atoi(argv[3]), atoi(argv[4]));
    cJSON_Delete(reddit_json);
    free(s.ptr);
    printf("\033[0m");

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}
