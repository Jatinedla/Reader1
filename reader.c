#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Controls access to rc (reader count)
pthread_mutex_t db = PTHREAD_MUTEX_INITIALIZER; // Controls access to the database
int rc = 0; // Number of threads reading

// Function to simulate the reader's behavior
void* reader(void* arg) {
    int i = *((int*)arg);

    // Debug print to check if reader thread starts
    printf("Reader %d is trying to enter the critical section\n", i);

    pthread_mutex_lock(&mutex); // Lock to modify rc
    rc = rc + 1; // One more reader
    if (rc == 1) {
        pthread_mutex_lock(&db); // First reader locks the database
        printf("Reader %d is the first reader, locking the database\n", i); // Debug print
    }
    pthread_mutex_unlock(&mutex); // Unlock rc modification

    // Simulating reading
    printf("Reader %d in critical section\n", i);
    usleep(1000); // Simulate time spent reading

    pthread_mutex_lock(&mutex); // Lock to modify rc
    rc = rc - 1; // One fewer reader
    if (rc == 0) {
        pthread_mutex_unlock(&db); // Last reader unlocks the database
        printf("Reader %d is the last reader, unlocking the database\n", i); // Debug print
    }
    pthread_mutex_unlock(&mutex); // Unlock rc modification

    printf("Reader %d in non-critical section\n", i); // Debug print
    return NULL;
}


void* writer(void* arg) {
    printf("Writer tries to enter the critical section\n");

    pthread_mutex_lock(&db); // Writer locks the database
    printf("Writer is in critical region\n");

    
    usleep(2000); // Simulate time spent writing

    pthread_mutex_unlock(&db); // Writer releases the database lock
    printf("Writer is in non-critical region\n");

    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    // Create 10 threads, with the second one being a writer
    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i + 1;
        if (i == 1) {
            pthread_create(&threads[i], NULL, writer, (void*)&thread_ids[i]);
            printf("Writer thread %d created\n", i + 1); // Debug print
        } else {
            pthread_create(&threads[i], NULL, reader, (void*)&thread_ids[i]);
            printf("Reader thread %d created\n", i + 1); // Debug print
        }
    }

   
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d joined\n", i + 1); // Debug print
    }

  
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&db);

    return 0;
}

