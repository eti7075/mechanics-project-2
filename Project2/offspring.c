/// File name: offspring.c
///
/// @author: Ethan Iannicelli
///

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "trimit.h"
#include "BFSQueue.h"
#include <stdbool.h>
#include <unistd.h>

typedef struct NaryTree_S {
    char * name;			
    struct NaryTree_S ** children;
    size_t child_count;		 
    size_t capacity;
} NaryTree_T;

/// creates a new node of NaryTree to be returned
/// @param new_name: the name of the new node
/// @return tp: if successful, NULL otherwise
NaryTree_T * create_node( char * new_name ) {
    NaryTree_T * tp;
    tp = (NaryTree_T *)malloc(sizeof(struct NaryTree_S));
    tp->name = malloc(100);
    if (new_name != NULL)
        strcpy(tp->name, new_name);
    tp->capacity = 2;
    tp->children = malloc(tp->capacity*sizeof(NaryTree_T*));
    tp->child_count = 0;
    if (tp != NULL)
        return tp;
    return NULL;
}

/// frees all memory of the tree and its offspring
/// @param tree: tree to be deleted
/// @return void
void destroy_tree( NaryTree_T * tree ) {
    if (tree != NULL){
        for( int i=0; i<(int)tree->child_count; i++) {
            if ( tree->children[i] != NULL) {
                destroy_tree( tree->children[i] );
            }
        }
        if (tree->name != NULL)
            free( tree->name );
        if (tree->children != NULL)
            free( tree->children );
        free( tree );
    }
}

/// uses a BFS alrogithm to find the first child/parent with a name
/// @param tree: root to search though
/// @param new_name: the name to search for within a tree
/// @return NULL if name is not found, NaryTree_T * if name is foumd
NaryTree_T * find_node( NaryTree_T * tree, char * new_name ) {
    if (tree == NULL)
        return NULL;
    BFSQueue bfs = que_create();
    que_insert( bfs, tree );
    while ((NaryTree_T *)get_NC( bfs )[0] != NULL  ){
        NaryTree_T * tmp = (NaryTree_T *)get_NC( bfs )[0];
        if (strcmp(tmp->name, new_name) == 0){
            que_clear( bfs );
            que_destroy( bfs );
            return tmp;
        } else {
            for (int i=0; i<(int)tmp->child_count; i++) {
                que_insert( bfs, tmp->children[i] );
            }
        }
        que_pop( bfs );
    }
    que_clear( bfs );
    que_destroy( bfs );
    return NULL;

}

/// prints the tree of a given name in a BFS manner
/// @param tree: root to print from
/// @param name: name of new root if != NULL
/// @return
void print_tree( NaryTree_T * tree, char * name ) {
    NaryTree_T * root;
    if (name == NULL) {
        root = tree;
        printf("print Ancestry Tree:\n");
        if (root == NULL){
            printf("\n");
            return;
        }
    } else {
        root = find_node( tree, name );
        printf("print %s:\n", name);
        if (root == NULL) {
            fprintf(stderr, "error: '%s' not found\n", name);
            return;
        }
    }

    BFSQueue bfs = que_create();
    que_insert( bfs, root );
    while ((NaryTree_T *)get_NC( bfs )[0] != NULL) {
        NaryTree_T * tmp = (NaryTree_T *)get_NC( bfs )[0];
        printf("%s had ", tmp->name);
        if (tmp->child_count > 0) {
            for (int i=0; i<(int)tmp->child_count; i++) {
                printf("%s", tmp->children[i]->name);
                que_insert( bfs, tmp->children[i] );
                if ((i+2)<(int)tmp->child_count)
                    printf(", ");
                else if ((i+2) == (int)tmp->child_count)
                    printf(" and ");
                else 
                    printf(".");
            }
        } else {
            printf("no offspring.");
        }
        printf("\n");
        que_pop( bfs );
    }
    que_clear( bfs );
    que_destroy( bfs );

}                   

/// goes thru cases to add child to a parent node
/// @param tree: the current tree 
/// @param parent_name: name of the parent node
/// @param child_name: name of the child node
/// @return returns the new parent node, NULL if neither parent nor child is present
NaryTree_T * add_child( NaryTree_T * tree, char * parent_name, char * child_name ) {
    char * p;
    char * c;
    p = malloc(100);
    c = malloc(100);
    strcpy(p, parent_name);
    if (child_name != NULL)
        strcpy(c, child_name);
    if ( tree == NULL ) {
        tree = create_node(p);
        if ( child_name != NULL) {
            tree->children[tree->child_count] = create_node(c);
            tree->child_count += 1;
        }
        free(c);
        free(p);
        return tree;
    } else if (find_node( tree, parent_name ) != NULL ) {
        NaryTree_T * pt = find_node( tree, p);
        if ( pt->child_count == pt->capacity ) {
            pt->capacity = pt->capacity*2;
            pt->children = realloc(pt->children, pt->capacity*sizeof(NaryTree_T*));
        }
        if ( find_node( pt, c) != NULL ) {
            fprintf(stderr, "error: '%s' is already a child of '%s'.\n", c, p);
            free(c);
            free(p);
            return tree;
        } else {
            pt->children[pt->child_count] = create_node( c );
            pt->child_count = pt->child_count + 1; 
            free(c);
            free(p);
            return tree;
        }
    } else if (find_node( tree, p ) == NULL) {
        if (strcmp(tree->name, c) == 0) {
            NaryTree_T * new_tree = create_node( p );
            new_tree->children[new_tree->child_count] = tree;
            new_tree->child_count += 1;
            free(p);
            free(c);
            return new_tree;
        } else {
            fprintf(stderr, "error: '%s' is not in the tree and '%s' is not the root.\n", p, c);
        }
    }
    free(p);
    free(c);
    return tree;
}

/// finds the number of nodes in a tree with root of a given name
/// @param tree: tree to find size from
/// @param node_name: new root to find size of tree of
/// @return number of nodes from a root
int tree_size( NaryTree_T * tree, char * node_name ) {
    NaryTree_T * root;
    if (node_name == NULL)
        root = tree;
    else 
        root = find_node( tree, node_name );
    if (root == NULL)
        return 0;
    BFSQueue bfs = que_create();
    que_insert( bfs, root );
    while ((NaryTree_T *)get_NC( bfs )[0] != NULL) {
        NaryTree_T * tmp = (NaryTree_T *)get_NC( bfs )[0];
        for (int i=0; i<(int)tmp->child_count; i++) {
            que_insert( bfs, tmp->children[i] );
        }
        que_pop( bfs );
    }
    int ret = get_num_C(bfs);
    que_clear(bfs);
    que_destroy(bfs);
    return ret;
}

/// finds the height of a tree [from a given name/node]
/// @param tree: tree to find height from
/// @param node_name: optional name of new root node to findheight from
/// @return height of tree
int tree_height( NaryTree_T * tree, char * node_name ) {
    NaryTree_T * root;
    if (node_name == NULL) {
        root = tree;
    } else {
        root = find_node( tree, node_name);
    }
    if (root == NULL)
        return 0;
    if (root->child_count == 0)
        return 1;
    int height = 1;
    int tmp = 0;
    for (int i=0; i<(int)root->child_count; i++) {
        tmp = tree_height( root->children[i], NULL );
        if (tmp > height)
            height = tmp;
    }
    return height+1;
}

int main(int argc, char * argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Command line should be in $ offspring [offspring-file] or $ offspring format.");
    }
    char * running = "start";
    char * cmd;
    char * add = "add";
    char * find = "find";
    char * print = "print";
    char * size = "size";
    char * height = "height";
    char * help = "help";
    char * init = "init";
    char * quit = "quit";
    char * input;
    NaryTree_T * tree = NULL;
    if (argc == 2) {
        FILE * fp;
        char * filename = argv[1];
        if ( access( filename, F_OK) == 0 ) {
            fp = fopen(filename, "r");
            char * line;
            line = malloc(1024);
            while (fgets( line, 1024, fp ) != NULL) {
                const char * s = ",";
                char * p = strtok(line, s);
                p = trim(p);
                char * c = strtok(NULL, s);
                c = trim(c);
                while (c != NULL ) {
                    tree = add_child( tree, p, c );
                    c = strtok(NULL, s);
                    c = trim(c);
                }
            }
            free(line);
            fclose(fp);
        } else { 
            fprintf(stderr, "%s: No such file or directory\n", filename);
        }
    }
    input = malloc(1024);
    char * eof;
    char * start = "start";
    while (strcmp(running, start) == 0) {
        printf("offspring> ");
        eof = fgets(input, 1024, stdin);
        if (eof == NULL)
            break;
        const char * a = " ";
        const char * b = ",";
        cmd = strtok(input, a);
        if (cmd[strlen(cmd)-1] == '\n')
            cmd[strlen(cmd)-1] = 0;
        if (strcmp(cmd, add) == 0) { //add parent-name, child1, ...
            char * parent;
            char * child;
            parent = strtok(NULL, b);
            parent = trim(parent);
            child = strtok(NULL, b);
            child = trim(child);
            if (parent == NULL || child == NULL) {
                printf("Usage: 'add parent name, child1[, child2 ...]'\n");
            } else {
                printf("adding...\n"); 
                while(child != NULL) { 
                    if (child[strlen(child)-1] == '\n')
                        child[strlen(child)-1] = 0;
                    tree = add_child( tree, parent, child );
                    child = strtok(NULL, b);
                    child = trim(child);
                }
            }
        } else if (strcmp(cmd, find) == 0) { //find name
            char * name;
            name = strtok(NULL, b);
            name = trim(name);
            if (name != NULL)
                if (name[strlen(name)-1] == '\n')
                    name[strlen(name)-1] = 0;
            if (name == NULL) {
                printf("Usage: 'find name'\n");
            } else {
                NaryTree_T * tmp = find_node( tree, name );
                if (tmp != NULL) {
                    printf("find %s:\n", name);
                    printf("%s had ", tmp->name);
                    for (int i=0; i<(int)tmp->child_count; i++) {
                        printf("%s", tmp->children[i]->name);
                        if ((i+2) < (int)tmp->child_count)
                            printf(", ");
                        else if ((i+2) == (int)tmp->child_count)
                            printf(" and ");
                        else 
                            printf(".\n");
                    }
                    if (tmp->child_count == 0)
                        printf("no offspring.\n");
                } else {
                    fprintf(stderr, "error: '%s' not found\n", name);
                }
            }
        } else if (strcmp(cmd, print) == 0) { //print [name]
            char * name;
            name = strtok(NULL, b);
            name = trim(name);
            if (name != NULL)
                if (name[strlen(name)-1] == '\n')
                    name[strlen(name)-1] = 0;
            print_tree( tree, name );
        } else if (strcmp(cmd, size) == 0) { //size [name]
            char * name;
            name = strtok(NULL, b);
            name = trim(name);
            if (name != NULL)        
                if (name[strlen(name)-1] == '\n')
                    name[strlen(name)-1] = 0;
            int size = tree_size( tree, name );
            if (name == NULL)
                printf("size Ancestry Tree: %d\n", size);
            else 
                printf("size %s: %d\n", name, size);
        } else if (strcmp(cmd, height) == 0) { //height [name]
            char * name;
            name = strtok(NULL, b);
            name = trim(name);
            if (name != NULL)
                if (name[strlen(name)-1] == '\n')
                    name[strlen(name)-1] = 0;
            int height = tree_height( tree, name );
            height -= 1;
            if (name == NULL)
                printf("height Ancestry Tree: %d\n", height);
            else
                printf("height %s: %d\n", name, height);
        } else if (strcmp(cmd, help) == 0) {
            printf("User Commands for offspring:\n"
            "add parent_name, child1 [, child2 ...]\t# find parent and add child1,...\n"
            "find name\t# search for name and print their family if name is found.\n"
            "print [name]\t# breadth first traversal of offspring from name.\n"
            "size [name]\t# count members in the [sub]tree.\n"
            "height [name]\t# return the height of [sub]tree.\n"
            "init\t\t# detele current tree and restart with an empty tree.\n"
            "help\t\t# print this information.\n"
            "quit\t\t# delete current tree and end program.\n");
        } else if (strcmp(cmd, init) == 0) { //init
            printf("init\n");
            destroy_tree( tree );
            tree = NULL;
        } else if (strcmp(cmd, quit) == 0) { //quit
            destroy_tree( tree );
            running = "quit";
        } else {
            destroy_tree( tree );
            running = "quit"; 
           // fprintf(stderr, "commmand not recognized\n");
        }
    }
    printf("\n");
    free( input );
    return 0;
}

