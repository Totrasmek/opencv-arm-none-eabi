#include "FreeRTOS.h"
#include "task.h"

#define MAX_PTHREAD_KEYS configNUM_THREAD_LOCAL_STORAGE_POINTERS

typedef int pthread_key_t;

static void (*destructors[MAX_PTHREAD_KEYS])(void *) = { NULL };

/* Create a new pthread key */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *)) {
    static int next_key = 0;

    if (next_key >= MAX_PTHREAD_KEYS) {
        return -1; // No more TLS slots available
    }

    *key = next_key++;
    destructors[*key] = destructor;
    return 0;
}

/* Delete a pthread key */
int pthread_key_delete(pthread_key_t key) {
    if (key >= MAX_PTHREAD_KEYS) {
        return -1;
    }

    destructors[key] = NULL;
    return 0;
}

/* Set thread-specific data */
int pthread_setspecific(pthread_key_t key, const void *value) {
    if (key >= MAX_PTHREAD_KEYS) {
        return -1;
    }

    vTaskSetThreadLocalStoragePointer(NULL, key, (void *)value);
    return 0;
}

/* Get thread-specific data */
void *pthread_getspecific(pthread_key_t key) {
    if (key >= MAX_PTHREAD_KEYS) {
        return NULL;
    }

    return pvTaskGetThreadLocalStoragePointer(NULL, key);
}

/* Destructor Cleanup (Call when task is deleted) */
void pthread_cleanup_task(void *param) {
    TaskHandle_t task = (TaskHandle_t)param;
    
    for (int key = 0; key < MAX_PTHREAD_KEYS; key++) {
        void *data = pvTaskGetThreadLocalStoragePointer(task, key);
        if (data && destructors[key]) {
            destructors[key](data); // Call destructor
        }
    }
}
