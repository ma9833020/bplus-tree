#include <pthread.h>

#define MAX_CLIENT_NUM 100

const char *port = "12345";
pthread_t tid[MAX_CLIENT_NUM];

int server_create();
void *serve_request(void *arg);
void exec_request(char *request, char *response);
