#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <assert.h>
#include <pthread>
#include <unistd.h>

#include IMPL

#if defined(BPTREE)||defined(BULK)
#include "../include/bplus.h"
#define BP_FILE "/tmp/bp_tree.bp"
#include <unistd.h>
bp_db_t db;
#endif

#define DICT_FILE "./test/dictionary/1990su.txt"
#define NUM 88800

int server_create()
{
    int fd = socket(PF_INET, SOCK_STREAM, 0);

    /*if server creation is failed*/
    if(fd == -1) {
        fprintf(stderr, "socket error: %s\n", gai_strerror(fd));
        exit(EXIT_FAILURE);
    }

    int error;
    struct sockaddr_in s_addr;
    struct addrinfo *res;

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(atoi(port));
    s_addr.sin_addr.s_addr = INADDR_ANY;

    /*bind the server_fd*/
    error = bind(fd, (struct sockaddr*)&s_addr, sizeof(s_addr));

    /*bind fail*/
    if(error != 0) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(error));
        exit(EXIT_FAILURE);
    }

    return fd;
}

void server_request(void *arg)
{
    int c_fd = (intptr_t) arg;
    int error;
    size_t request_size, response_size;

    while(true) {
        char *request, *response;
        
        if((error = read(c_fd, &request_size, sizeof(size_t))) == -1) {
            fprintf(stderr, "read request size failed\n");
            exit(EXIT_FAILURE);
        } else {
            request = (char *) malloc(sizeof(char) * request_size);
        }

        /* client has left */
        if(error == 0) {
            printf("client has left\n");
            break;
        }

        if((error = read(c_fd, &request, size)) == -1) {
            fprintf(stderr, "read request failed\n");
            exit(EXIT_FAILURE);
        } else
            exec_request(request, response);

        response_size = sizeof(response);
        if((error = write(c_fd, &response_size, sizeof(size_t))) == -1) {
            fprintf(stderr, "write response size failed\n");
            exit(EXIT_FAILURE);
        }

        if((error = write(c_fd, &response, response_size)) == -1) {
            fprintf(stderr, "write response failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void exec_request(char *request, char *response)
{
    response = (char *) malloc(sizeof(char) * 50);

    char *tmp = strtok(request, " ");

    if(strtmp(tmp, "FIND") == 0) {
        tmp = strtok(NULL, " ");
        if(tmp != NULL) {
            char foundName[strlen(tmp)];
#if defined(BPTREE)
            bp_gets(&db, tmp, &foundName);
#endif
            if(strcmp(tmp, foundName) == 0) {
                /* found */
                sprintf(response, "Data Found");
            } else {
                /* not found */
                sprintf(response, "Data Not Found");
            }
    } else if(strcmp(tmp, "INSERT")) {
        tmp = strtok(NULL, " ");
#if defined(BPTREE)
        if(tmp != NULL) {
	        assert(bp_sets(&db, tmp, tmp) == BP_OK);
            sprintf(response, "Insert Successfully");   
        } else {
            /* request error */
            sprintf(response, "Bad Request");
        }
#endif
    } else if(strcmp(tmp, "REMOVE")) {

    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    int s_fd, c_fd;
    s_fd = server_create();
    pthread_t tid[MAX_CLIENT_NUM];
    int thread_num = 0;

#if defined(BPTREE)||defined(BULK)
    if(access(BP_FILE,F_OK)==0) assert(unlink(BP_FILE) ==0);
    bp_open(&db, "BP_FILE");
#else
    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__) && !defined(BPTREE) && !defined(BULK)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#if defined(BULK)
    char *bulk_buffer, *bulk_data[NUM];
    // number of data in the buffer
    int bulk_data_count = 0;

    bulk_buffer = (char*) malloc(NUM * MAX_LAST_NAME_SIZE);
    for(i=0; i < NUM; i++) {
        bulk_data[i] = bulk_buffer +MAX_LAST_NAME_SIZE*i;
    }
#endif
    i = 0;
    int data_num_count = 0;
  //  while (fgets(line,sizeof(line),fp)) {
    while (data_num_count != NUM) {
        data_num_count++;
	fgets(line, sizeof(line), fp);
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#if defined(BPTREE)
	assert(bp_sets(&db, line, line) == BP_OK);
    }
#elif defined(BULK)
        strcpy(bulk_data[bulk_data_count++],line);

        if(bulk_data_count == NUM) {
            bp_bulk_sets(&db,
                         bulk_data_count,
                         (const char**) bulk_data,
                         (const char**) bulk_data);
           bulk_data_count = 0;
        }
    }
#else
        e = append(line, e);
    }
#endif
    data_num_count=0;
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);
    /* close file as soon as possible */
    fclose(fp);

	/* listen on port */
    listen(s_fd, MAX_CLINET_NUM);
	
	while(true) {
        struct sockaddr_storage c_addr;
        int c_addr_len = sizeof(c_addr);

        memset(&c_addr, 0, sizeof(c_addr));

        /* accept for client */
        c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &c_addr_len);

        if(c_fd > 0) {
            printf("client request\n");
            int tmp = thread_num;
            pthread_create(&tid[tmp], NULL, (void *) &serve_request, (void *)(intptr_t) c_fd);
            thread_num = (thread_num++) % MAX_CLIENT_NUM;
        }
    }

    for(int i = 0; i < MAX_CLIENT_NUM; i++) {
        pthread_join(tid[i], NULL);
    }

#if defined(BULK)
    free(bulk_buffer);
    bp_close(&db);
#elif defined(BPTREE)
    bp_close(&db);
#else
    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#endif
    return 0;
}
