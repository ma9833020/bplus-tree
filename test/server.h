#include <pthread.h>

#define MAX_CLIENT_NUM 100

const char *port = "11111";
pthread_t tid[MAX_CLIENT_NUM];

int server_create();
void server_request(void *arg);
void exec_request(char *request, char *response);
