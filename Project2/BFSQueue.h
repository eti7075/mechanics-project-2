/// File: BFSQueue.h
/// @author Ethan Iannicelli

#ifndef _BFSQUEUE_H_
#define _BFSQUEUE_H_

#include <stdbool.h>

#ifndef _QUEUE_IMPL_

/// BFSQueue is a pointer to an abstract queue data structure
/// with a checked and unchecked queue for BFS searching, both of which are void *

typedef struct { } * BFSQueue;

#endif

/// creates an empty struct for a BFS
/// @param
/// @return the initialized struct
BFSQueue que_create( );

/// destroys a struct for a bfs
/// @param queue: struct of a BFS
/// @return
void que_destroy( BFSQueue queue );

/// frees the contents of a bfs struct
/// @param queue: struct of a BFS
/// @return
void que_clear( BFSQueue queue );

/// inputs data at the end of a bfs queue
/// @param queue: a bfs struct with a queue
/// @param data: datat to be inserted
/// @return
void que_insert( BFSQueue queue, void * data );

/// removes the first element in the unchecked queue, adds it to the end of
/// the checked queue, returns that element
/// @param queue: a bfs struct with unchecked and checked values
/// @return pointer to last element that was checked
void * que_pop( BFSQueue queue );

/// checks to see if a queue still has elements to be checked
/// @param queue: bfs to be checked
/// @return state of checked values
bool que_empty( BFSQueue queue );

/// getter for number of unchecked values
/// @param queue: instance of bfs queue to check
/// @return number of unchecked values
int get_num_NC( BFSQueue bfs );

/// getter for array of unchecked values
/// @param queue: instance of bfs queue to return
/// @return array of unchecked values
void ** get_NC( BFSQueue bfs );

/// getter for number of checked values
/// @param queue: instance of bfs queue to check
/// @return number of checked values
int get_num_C( BFSQueue bfs );
#endif
