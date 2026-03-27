#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define N 5
#define STEP_DELAY 300000   // 0.3 seconds for smooth output

sem_t *chopstick[N];
int mode;

void philosopher(int id) {
    int left = id;
    int right = (id + 1) % N;
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        fflush(stdout);
        usleep(STEP_DELAY);
        printf("Philosopher %d is hungry\n", id);
        fflush(stdout);
        usleep(STEP_DELAY);
        if (mode == 0) {
            // MODE 0: Before synchronization (deadlock)
            sem_wait(chopstick[left]);
            printf("Philosopher %d picked LEFT chopstick %d\n", id, left);
            fflush(stdout);
            usleep(STEP_DELAY);

            sem_wait(chopstick[right]);
            printf("Philosopher %d picked RIGHT chopstick %d\n", id, right);
            fflush(stdout);
        } else {
            // MODE 1: After synchronization (deadlock-free)
            if (id == N - 1) {
                sem_wait(chopstick[right]);
                printf("Philosopher %d picked RIGHT chopstick %d\n", id, right);
                fflush(stdout);
                usleep(STEP_DELAY);

                sem_wait(chopstick[left]);
                printf("Philosopher %d picked LEFT chopstick %d\n", id, left);
                fflush(stdout);
            } else {
                sem_wait(chopstick[left]);
                printf("Philosopher %d picked LEFT chopstick %d\n", id, left);
                fflush(stdout);
                usleep(STEP_DELAY);

                sem_wait(chopstick[right]);
                printf("Philosopher %d picked RIGHT chopstick %d\n", id, right);
                fflush(stdout);
            }
        }
        printf("Philosopher %d is eating\n", id);
        fflush(stdout);
        sleep(2);
        sem_post(chopstick[left]);
        sem_post(chopstick[right]);
        printf("Philosopher %d released chopsticks\n", id);
        fflush(stdout);
        usleep(STEP_DELAY);
    }
}
int main() {
    printf("Dining Philosophers Problem\n");
    printf("0 -> Before Synchronization (Deadlock)\n");
    printf("1 -> After Synchronization (Solution)\n");
    printf("Enter choice: ");
    scanf("%d", &mode);
    for (int i = 0; i < N; i++) {
        char name[20];
        sprintf(name, "/c%d", i);
        sem_unlink(name);
        chopstick[i] = sem_open(name, O_CREAT, 0644, 1);
    }
    for (int i = 0; i < N; i++) {
        if (fork() == 0) {
            philosopher(i);
            exit(0);
        }
    }
    for (int i = 0; i < N; i++)
        wait(NULL);
    return 0;
}