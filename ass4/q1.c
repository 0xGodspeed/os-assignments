#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// int phil_ids[5] = {0, 1, 2, 3, 4};
void *phil_ids[5] = {(void *)0, (void *)1, (void *)2, (void *)3, (void *)4};

pthread_mutex_t lock;
pthread_cond_t cond[5];
pthread_t phils[5];

#define NOT_AVAILABLE 0
#define AVAILABLE 1

int forks[5] = {AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE};

void thinking(int id) {
    printf("Philosopher %d is thinking\n", id);
    sleep(rand() % 5);
}

void eating(int id) {
    printf("Philosopher %d is eating\n", id);
    sleep(rand() % 5);
    printf("Philosopher %d finished eating\n", id);
}

int left(int id) { return id; }

int right(int id) { return (id + 1) % 5; }

void get_forks(int id) {
    int left_id = left(id);
    int right_id = right(id);
    pthread_mutex_lock(&lock);
    // philosopher 4 is left handed, rest are right handed
    if (id == 4) {
        while (forks[left_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, left_id);
            pthread_cond_wait(&cond[left_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, left_id);
        while (forks[right_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, right_id);
            pthread_cond_wait(&cond[right_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, right_id);
    } else {
        while (forks[right_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, right_id);
            pthread_cond_wait(&cond[right_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, right_id);
        while (forks[left_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, left_id);
            pthread_cond_wait(&cond[left_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, left_id);
    }
    forks[left_id] = NOT_AVAILABLE;
    forks[right_id] = NOT_AVAILABLE;
    pthread_mutex_unlock(&lock);
}

void put_forks(int id) {
    int left_id = left(id);
    int right_id = right(id);
    pthread_mutex_lock(&lock);
    forks[left_id] = AVAILABLE;
    printf("Philosopher %d put down fork %d\n", id, left_id);
    pthread_cond_broadcast(&cond[left_id]);
    forks[right_id] = AVAILABLE;
    printf("Philosopher %d put down fork %d\n", id, right_id);
    pthread_cond_broadcast(&cond[right_id]);
    pthread_mutex_unlock(&lock);
}

void philosopher(void *args) {
    int id = *((int *)args);
    while (1) {
        thinking(id);
        get_forks(id);
        eating(id);
        put_forks(id);
        sleep(rand() % 5);
    }
}

int main() {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        if (pthread_cond_init(&cond[i], NULL) != 0) {
            printf("Cond init failed\n");
            return 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (pthread_create(&phils[i], NULL, (void *)philosopher,
                           (void *)&phil_ids[i]) != 0) {
            printf("Thread creation failed\n");
            return 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(phils[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    for (int i = 0; i < 5; i++) {
        pthread_cond_destroy(&cond[i]);
    }
    return 0;
}
