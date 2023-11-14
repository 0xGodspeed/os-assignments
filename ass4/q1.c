#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// int phil_ids[5] = {0, 1, 2, 3, 4};
void *phil_ids[5] = {(void *)0, (void *)1, (void *)2, (void *)3, (void *)4};

pthread_mutex_t lock;
pthread_cond_t cond_fork[5];
pthread_cond_t cond_bowl[2];
pthread_cond_t bowl_available;
pthread_t phils[5];

#define NOT_AVAILABLE 0
#define AVAILABLE 1
#define TIME 2 * 1000000

int forks[5] = {AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE, AVAILABLE};
int bowls[2] = {AVAILABLE, AVAILABLE};

// to keep track of which philosopher has which bowl
int phil_bowl[5] = {-1, -1, -1, -1, -1};

void thinking(int id) {
    printf("Philosopher %d is thinking\n", id);
    usleep(random() % TIME);
}

void eating(int id) {
    printf("Philosopher %d is eating\n", id);
    usleep(random() % TIME);
    printf("Philosopher %d finished eating\n", id);
}

int left(int id) { return id; }

int right(int id) { return (id + 1) % 5; }

void get_forks_and_bowl(int id) {
    int left_id = left(id);
    int right_id = right(id);
    pthread_mutex_lock(&lock);
    // philosopher 4 is left handed, rest are right handed
    if (id == 4) {
        while (forks[left_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, left_id);
            pthread_cond_wait(&cond_fork[left_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, left_id);
        while (forks[right_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, right_id);
            pthread_cond_wait(&cond_fork[right_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, right_id);
    } else {
        while (forks[right_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, right_id);
            pthread_cond_wait(&cond_fork[right_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, right_id);
        while (forks[left_id] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for fork %d\n", id, left_id);
            pthread_cond_wait(&cond_fork[left_id], &lock);
        }
        printf("Philosopher %d got fork %d\n", id, left_id);
    }
    forks[left_id]  = NOT_AVAILABLE;
    forks[right_id] = NOT_AVAILABLE;

    // if (bowls[id % 2] == NOT_AVAILABLE) {
    //     printf("Philosopher %d is waiting for bowl %d\n", id, id % 2);
    //     pthread_cond_wait(&cond_bowl[id % 2], &lock);
    // } else {
    //     bowls[id % 2] = NOT_AVAILABLE;
    //     phil_bowl[id] = id % 2;
    //     printf("Philosopher %d got bowl %d\n", id, id % 2);
    // }

    if (bowls[0] == NOT_AVAILABLE) {
        if (bowls[1] == NOT_AVAILABLE) {
            printf("Philosopher %d is waiting for a bowl\n", id);
            // if both bowls are not available, wait for one of them to be available
            pthread_cond_wait(&bowl_available, &lock);
            if (bowls[0] == AVAILABLE) {
                bowls[0] = NOT_AVAILABLE;
                phil_bowl[id] = 0;
                printf("Philosopher %d got bowl 0\n", id);
            } else {
                bowls[1] = NOT_AVAILABLE;
                phil_bowl[id] = 1;
                printf("Philosopher %d got bowl 1\n", id);
            }
            // pthread_cond_wait(&cond_bowl[1], &lock);
        }
        else {
            bowls[1] = NOT_AVAILABLE;
            phil_bowl[id] = 1;
            printf("Philosopher %d got bowl 1\n", id);
        }
    } else {
        bowls[0] = NOT_AVAILABLE;
        phil_bowl[id] = 0; 
        printf("Philosopher %d got bowl 0\n", id);
    }
    pthread_mutex_unlock(&lock);
}

void put_forks_and_bowl(int id) {
    int left_id = left(id);
    int right_id = right(id);
    pthread_mutex_lock(&lock);
    forks[left_id] = AVAILABLE;
    printf("Philosopher %d put down fork %d\n", id, left_id);
    pthread_cond_broadcast(&cond_fork[left_id]);
    forks[right_id] = AVAILABLE;
    printf("Philosopher %d put down fork %d\n", id, right_id);
    pthread_cond_broadcast(&cond_fork[right_id]);
    bowls[phil_bowl[id]] = AVAILABLE;
    printf("Philosopher %d put down bowl %d\n", id, phil_bowl[id]);
    phil_bowl[id] = -1;
    pthread_cond_broadcast(&bowl_available);
    // pthread_cond_broadcast(&cond_bowl[phil_bowl[id]]);
    pthread_mutex_unlock(&lock);
}

void philosopher(void *args) {
    int id = *((int *)args);
    while (1) {
        thinking(id);
        get_forks_and_bowl(id);
        eating(id);
        put_forks_and_bowl(id);
        usleep(random() % TIME);
        usleep(rand() % 5000000);
    }
}

int main() {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        if (pthread_cond_init(&cond_fork[i], NULL) != 0) {
            printf("Cond init failed\n");
            return 1;
        }
    }

    pthread_cond_init(&bowl_available, NULL);

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
        pthread_cond_destroy(&cond_fork[i]);
    }
    return 0;
}
