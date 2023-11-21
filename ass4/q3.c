#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// #define TIME 2 * 1000000
#define TIME 2 * 100000
#define LEFT 1
#define RIGHT 0

int current_direction;
int count_in_direction;
int left, right;
int remaining_left, remaining_right;

sem_t crossing_left, crossing_right;
sem_t mutex;

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
char* num_to_direction(int num) { return num == LEFT ? "left" : "right"; }

void passing() { usleep(TIME); }

// left car thread
void* car_left(void* args) {
    int car_id = *((int*)args);
    printf("Car %d is waiting for crossing.\n", car_id);
    sem_wait(&crossing_left);

    sem_wait(&mutex);
    count_in_direction++;
    remaining_left--;
    printf("before cond count_in_direction: %d\n", count_in_direction);
    if (current_direction == LEFT &&
        (count_in_direction <= 5 && remaining_left > 0) || remaining_right == 0) {
        sem_post(&crossing_left);
        sem_post(&mutex);

    // rem
    } else {
        printf("-----switching directions-----\n");
        current_direction = RIGHT;
        // remaining_left = max(0, remaining_left - 5);
        count_in_direction = 0;
        sem_post(&crossing_right);
        sem_post(&mutex);
    }

    // sem_wait(&mutex);
    printf("Car %d is crossing.\n", car_id);
    // count_in_direction++;
    printf("Current direction: %s\n", num_to_direction(current_direction));
    printf("Count in direction: %d\n", count_in_direction);
    printf("Remaining left: %d | Remaining right: %d\n", remaining_left, remaining_right);
    passing();
    // if (count_in_direction == min(5, remaining_left)) {
    //     printf("bruh");
    //     current_direction = RIGHT;
    //     count_in_direction = 0;
    //     sem_post(&crossing_right);
    // }
    // sem_post(&mutex);
}

// right car thread
void* car_right(void* args) {
    int car_id = *((int*)args);
    printf("Car %d is waiting for crossing.\n", car_id);
    sem_wait(&crossing_right);
    sem_wait(&mutex);
    count_in_direction++;
    remaining_right--;
    printf("before cond count_in_direction: %d\n", count_in_direction);
    if (current_direction == RIGHT && count_in_direction <= 5 && remaining_right > 0 || remaining_left == 0) {
        sem_post(&crossing_right);
        sem_post(&mutex);
    } else {
        printf("-----switching directions-----\n");
        current_direction = LEFT;
        count_in_direction = 0;
        // remaining_right = max(0, remaining_right - 5);
        sem_post(&crossing_left);
        sem_post(&mutex);
    }
    printf("Car %d is crossing.\n", car_id);
    printf("Current direction: %s\n", num_to_direction(current_direction));
    printf("Count in direction: %d\n", count_in_direction);
    printf("Remaining left: %d | Remaining right: %d\n", remaining_left,
           remaining_right);
    passing();
    // sem_wait(&mutex);
    // count_in_direction++;
    // if (count_in_direction == min(5,f remaining_right)) {
    //     current_direction = LEFT;
    //     count_in_direction = 0;
    // }
    // sem_post(&mutex);
    // sem_post(&crossing_left);
}

void init_semaphores() {
    sem_init(&crossing_left, 0, 1);
    sem_init(&crossing_right, 0, 0);
    sem_init(&mutex, 0, 1);
}

int main() {
    init_semaphores();
    printf("Enter number of cars on the left: ");
    scanf("%d", &left);
    printf("Enter number of cars on the right: ");
    scanf("%d", &right);
    remaining_left = left;
    remaining_right = right;
    current_direction = max(left, right) == left ? LEFT : RIGHT;
    count_in_direction = 0;

    pthread_t cars[left + right];
    int car_id[left + right];

    for (int i = 0; i < left; i++) {
        car_id[i] = i;
        pthread_create(&cars[i], NULL, car_left, &car_id[i]);
    }

    for (int i = left; i < left + right; i++) {
        car_id[i] = i;
        pthread_create(&cars[i], NULL, car_right, &car_id[i]);
    }

    // special case when there are no cars on the left
    if (left == 0) {
        sem_post(&crossing_right);
    }

    for (int i = 0; i < left + right; i++) {
        pthread_join(cars[i], NULL);
    }

    sem_destroy(&crossing_left);
    sem_destroy(&crossing_right);
    sem_destroy(&mutex);
    return 0;
}