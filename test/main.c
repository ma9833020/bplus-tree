#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#if defined(BPTREE)||defined(BULK)||defined(BPTREEC)
#include "../include/bplus.h"
#define BP_FILE "/tmp/bp_tree.bp"
#include <unistd.h>
#endif

#define DICT_FILE "./test/dictionary/1990su.txt"
#define NUM 88800
#define rnum 2
char foundNameArr[15][20];
void* reader_thread(void*);

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
   return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

#if defined(BPTREEC)
void* reader_thread(void* db_) {
    bp_db_t* db = (bp_db_t*) db_;
    for (int i = 0; i < rnum; i++) {
        char* value;
        bp_gets(db, foundNameArr[i], &value);
        free(value);
    }
    return NULL;
}
#endif


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

#if defined(BPTREE)||defined(BULK)||defined(BPTREEC)
    if(access(BP_FILE,F_OK)==0) assert(unlink(BP_FILE) ==0);
    bp_db_t db;
    bp_open(&db, "BP_FILE");
#else
    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__) && !defined(BPTREE) && !defined(BULK) && !defined(BPTREEC)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
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
#if defined(BPTREE)||defined(BPTREEC)
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
    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "ZYSKOWSKI";
#if defined(BPTREE)||defined(BULK)||defined(BPTREEC)
    char* foundName;
    char* foundName2;
    char* foundName3;
    char* foundName4;
    char* foundName5;
    char* foundName6;
    char* foundName7;
    char* foundName8;
    char* foundName9;
    char* foundName10;
    char* foundName11;
    char* foundName12;
    char* foundName13;
    char* foundName14;
    char* foundName15;

    assert(bp_gets(&db, "MOCZYGEMBA", &foundName) == BP_OK);
    assert(0== strcmp(foundName, "MOCZYGEMBA"));
    free(foundName);    

    assert(bp_gets(&db, "ROGUGBAKAA", &foundName2) == BP_OK);
    assert(0== strcmp(foundName2, "ROGUGBAKAA"));
    free(foundName2);    

    assert(bp_gets(&db, "VANISOUVONG", &foundName3) == BP_OK);
    assert(0== strcmp(foundName3, "VANISOUVONG"));
    free(foundName3);    

    assert(bp_gets(&db, "WEIDNER", &foundName4) == BP_OK);
    assert(0== strcmp(foundName4, "WEIDNER"));
    free(foundName4);    

    assert(bp_gets(&db, "ZYSKOWSKI", &foundName5) == BP_OK);
    assert(0== strcmp(foundName5, "ZYSKOWSKI"));
    free(foundName5);

#else
    e = pHead;
    assert(findName(input, e));
    assert(0 == strcmp(findName(input, e)->lastName, "ZYSKOWSKI"));
#endif
#if defined(__GNUC__) && !defined(BPTREE) && !defined(BULK) && !defined(BPTREEC)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    strcpy(foundNameArr[0],"ABDELRAHMAN");
    strcpy(foundNameArr[1],"BARMETTLER");
    strcpy(foundNameArr[2],"DEBARDELABEN");
    strcpy(foundNameArr[3],"EVERSMEYER");
    strcpy(foundNameArr[4],"GAISFORD");
    strcpy(foundNameArr[5],"HANBERRY");
    strcpy(foundNameArr[6],"JABLONOWSKI");
    strcpy(foundNameArr[7],"KOCZWARA");
    strcpy(foundNameArr[8],"LINDENBERGER");
    strcpy(foundNameArr[9],"MIKUSZEWSKI");
    strcpy(foundNameArr[10],"NAVARRETTE");
    strcpy(foundNameArr[11],"PALCZYNSKI");
    strcpy(foundNameArr[12],"RADAKOVICH");
    strcpy(foundNameArr[13],"STODOMINGO");
    strcpy(foundNameArr[14],"ZARRALUQUI");
    clock_gettime(CLOCK_REALTIME, &start);
#if defined(BPTREE)||defined(BULK)
    bp_gets(&db, "ABDELRAHMAN", &foundName);
    bp_gets(&db, "BARMETTLER", &foundName2);
    bp_gets(&db, "DEBARDELABEN", &foundName3);    
    bp_gets(&db, "EVERSMEYER", &foundName4);    
    bp_gets(&db, "GAISFORD", &foundName5);
    bp_gets(&db, "HANBERRY", &foundName6);
    bp_gets(&db, "JABLONOWSKI", &foundName7);
    bp_gets(&db, "KOCZWARA", &foundName8);    
    bp_gets(&db, "LINDENBERGER", &foundName9);    
    bp_gets(&db, "MIKUSZEWSKI", &foundName10);    
    bp_gets(&db, "NAVARRETTE", &foundName11);
    bp_gets(&db, "PALCZYNSKI", &foundName12);
    bp_gets(&db, "RADAKOVICH", &foundName13);    
    bp_gets(&db, "STODOMINGO", &foundName14);    
    bp_gets(&db, "ZARRALUQUI", &foundName15);
#elif defined(BPTREEC)
    pthread_t readers[rnum];
    for (i = 0; i < rnum; i++) {
         pthread_create(&readers[i], NULL, reader_thread, (void*) &db);
    }
 
    for (i = 0; i < rnum; i++) {
         pthread_join(readers[i], NULL);
    }

#else
    findName(input, e);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);
    FILE *output;
#if defined(OPT)
    output = fopen("opt.txt", "a");
#elif defined(BPTREE)
    output = fopen("bptree.txt", "a");
#elif defined(BULK)
    output = fopen("bulk.txt","a");
#elif defined(BPTREEC)
    output = fopen("bptreec.txt","a");
#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);
    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
#if defined(BULK)
    free(bulk_buffer);
    bp_close(&db);
#elif defined(BPTREE)||defined(BPTREEC)
    bp_close(&db);
#else
    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#endif
    return 0;
}
