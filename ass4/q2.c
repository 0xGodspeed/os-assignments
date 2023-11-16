#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TIME 2 * 1000000

int total_passengers;
int capacity;
int passengers_on_board = 0;

sem_t ride;
sem_t carLoaded, carUnloaded;
sem_t boardMutex;

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

void load() { usleep(random() % TIME); }

void unload() { usleep(random() % TIME); }

void ride_car() {
    usleep(random() % TIME);
    printf("Car ride done.\n");
}

void board(int passenger_id) {
    printf("Passenger %d boards the car.\n", passenger_id);
    usleep(random() % TIME);
}

void offboard(int passenger_id) {
    printf("Passenger %d gets off the car.\n", passenger_id);
    usleep(random() % TIME);
}

void* passenger(void* args) {
    int passenger_id = *((int*)args);
    printf("Passenger %d is waiting for car to be loaded.\n", passenger_id);
    sem_wait(&carLoaded);
    printf("Passenger %d knows that car is loaded.\n", passenger_id);

    // while (passengers_on_board < capacity) {
    sem_wait(&boardMutex);
    passengers_on_board++;
    board(passenger_id);
    printf("Passenger %d is on board.\n", passenger_id);
    sem_post(&boardMutex);

    sem_wait(&ride);
    printf("Passenger %d knows that ride is done.\n", passenger_id);
    // }

    sem_wait(&carUnloaded);
    printf("Passenger %d knows that car is unloaded.\n", passenger_id);

    sem_wait(&boardMutex);
    offboard(passenger_id);
    printf("Passenger %d is off board.\n", passenger_id);
    passengers_on_board--;
    sem_post(&boardMutex);
}

void* car(void* args) {
    while (total_passengers > 0) {
        load();
        // let all passenger threads know that car is loaded
        for (int i = 0; i < min(total_passengers, capacity); i++) {
            sem_post(&carLoaded);
        }
        printf("Car is loaded.\n");

        while (1) {
            sem_wait(&boardMutex);
            //  for when the remaining passengers are less than capacity
            if (passengers_on_board ==
                (total_passengers < capacity ? total_passengers : capacity)) {
                sem_post(&boardMutex);
                break;
            }
            sem_post(&boardMutex);
        }

        printf("Car is full with %d passengers.\n", passengers_on_board);

        ride_car();

        // all passenger threads are waiting for ride semaphore
        // let them know that ride is done
        for (int i = 0; i < passengers_on_board; i++) {
            sem_post(&ride);
        }

        printf("Car ride done.\n");

        unload();

        // let all passenger threads know that car is unloaded
        for (int i = 0; i < min(total_passengers, capacity); i++) {
            sem_post(&carUnloaded);
        }
        printf("Car is unloaded.\n");

        while (1) {
            sem_wait(&boardMutex);
            if (passengers_on_board == 0) {
                sem_post(&boardMutex);
                break;
            }
            sem_post(&boardMutex);
        }

        printf("Car is empty with %d passengers.\n", passengers_on_board);
        total_passengers -= capacity;
        // if total_passengers goes below 0, then no passengers are left
        printf("Total passengers left: %d\n", max(total_passengers, 0));
    }
    printf("All passengers have ridden the car.\n");
}

void init() {
    sem_init(&ride, 0, 0);
    sem_init(&carLoaded, 0, 0);
    sem_init(&carUnloaded, 0, 0);
    sem_init(&boardMutex, 0, 1);
}

int main() {
    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);
    printf("Enter capacity of car: ");
    scanf("%d", &capacity);
    init();
    pthread_t passengers[total_passengers];
    pthread_t carThread;
    int passenger_id[total_passengers];
    int car_id = 0;
    pthread_create(&carThread, NULL, car, &car_id);
    for (int i = 0; i < total_passengers; i++) {
        passenger_id[i] = i;
        pthread_create(&passengers[i], NULL, passenger, &passenger_id[i]);
    }
    for (int i = 0; i < total_passengers; i++) {
        pthread_join(passengers[i], NULL);
    }
    if (pthread_join(carThread, NULL) != 0) {
        printf("Thread join failed\n");
        return 1;
    }
    return 0;
}