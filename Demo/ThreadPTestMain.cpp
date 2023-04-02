//
// Created by wjc on 2023/3/31.
//
#include <pthread.h>
#include <iostream>
using namespace std;

static long long total = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* func(void*) {
    long long i;
    for (i = 0; i < 1000L; ++i) {
        pthread_mutex_lock(&m);
        total += i;
        cout<<i<<endl;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int mainpthread_t(void) // pthread_t
{
    pthread_t  thread1, thread2;
    if (pthread_create(&thread1, NULL, &func, NULL)) {
        throw;
    }
    if (pthread_create(&thread2, NULL, &func, NULL)) {
        throw;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    cout<<total<<endl;

    return 0;
}