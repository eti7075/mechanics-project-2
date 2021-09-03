/// File: BFSQueue.c
/// @author Ethan Iannicelli

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define QUEUE_SIZE 32

/// struct for a BF

typedef struct BFSQueue_S {
    void ** not_checked;
    void ** checked;
    int capacity;
    int num_checked;
    int num_not_checked;
} BFSQueue_T;

#define _QUEUE_IMPL_

typedef BFSQueue_T * BFSQueue;

#include "BFSQueue.h"

/// creates a BFS struct and inits non ptr values
/// and allocates space for the struct
/// @param
/// @return initialzed BFS struct
BFSQueue que_create( ) {
    BFSQueue bfs;
    bfs = (BFSQueue)malloc(sizeof(struct BFSQueue_S));
    bfs->num_checked = 0;
    bfs->num_not_checked = 0;
    bfs->capacity = QUEUE_SIZE;
    bfs->not_checked = NULL;
    bfs->checked = NULL;
    return bfs;
}

/// checks if any thing still remains in the struct and frees all relevent ptrs
/// @param queue: struct to be destroyed
/// @return
void que_destroy( BFSQueue queue ) {
    if (queue != NULL) {
        if (queue->not_checked != NULL) {
            free( queue->not_checked );
        }
        if (queue->checked != NULL) {
            free( queue->checked );
        }
        free( queue );
    }
}

/// sets all values for checked and unchecked values to NULL
/// @param queue: bfs struct
/// @return
void que_clear( BFSQueue queue ) {
    for (int i=0; i<queue->num_checked; i++) {
        queue->checked[i] = NULL;
    }
    for (int i=0; i<queue->num_not_checked; i++) {
        queue->not_checked[i] = NULL;
    }
}

/// inserts data into the last position in the unchecked queue and 
/// allocates mempry if needed
/// @param queue: bfs struct
/// @param data: pointer to unckech value
/// @return
void que_insert( BFSQueue queue , void * data ) {
    if (data == NULL)
        return;
    for (int i=0; i<queue->num_checked; i++) {
        if (data == queue->checked[i])
            return;
    }
    if (queue->capacity == queue->num_not_checked) {
        queue->capacity += QUEUE_SIZE;
        queue->checked = realloc(queue->checked, queue->capacity*sizeof(void *));
        queue->not_checked = realloc(queue->not_checked, queue->capacity*sizeof(void *));
    }
    assert( queue->num_not_checked < queue->capacity );
    if (queue->not_checked == NULL) {
        queue->checked = malloc( sizeof(void *) * queue->capacity );
        queue->not_checked = malloc( sizeof(void *) * queue->capacity );
        queue->not_checked[queue->num_not_checked] = data;
        queue->num_not_checked += 1;
    } else {
        queue->not_checked[queue->num_not_checked] = data;
        queue->num_not_checked += 1;
    }
}

/// "checks" a value at start of unchecked queue by removing it and putting
/// it into the checked queue. returns that value
/// @param queue: bfs struct
/// @return a now checked ptr to a val
void * que_pop( BFSQueue queue ) {
    if (queue->not_checked[0] == NULL) {
        fprintf(stderr, "BFS is empty");
        return NULL;
    }
    void * retval = queue->not_checked[0];
    assert (queue->num_not_checked > 0);
    for (int i=0; i<queue->num_not_checked; i++) {
        queue->not_checked[i] = queue->not_checked[i+1];
    }
    queue->num_not_checked -= 1;
    queue->checked[queue->num_checked] = retval;
    queue->num_checked += 1;
    queue->not_checked[queue->num_not_checked] = NULL;
    return retval;
}

/// checks to see any unchecked values remain
/// @param queue: bfs struct
/// @return state of unchecked values
bool queue_empty( BFSQueue queue ) {
    if (queue->not_checked[0] == NULL)
        return true;
    return false;
}

/// gets the number of unchecked values remain in queue
/// @param bfs: bfs struct
/// @return number of unchecked values
int get_num_NC( BFSQueue bfs ) {
    return bfs->num_not_checked;
}

/// gets the queue of unchecked values
/// @param bfs: bfs struct
/// @return array of unchecked values
void ** get_NC( BFSQueue bfs ) {
    return bfs->not_checked;
}

/// gets number of checked value
/// @param bfs: bfs struct
/// @return number of checked values
int get_num_C( BFSQueue bfs ) {
    return bfs->num_checked;
}
