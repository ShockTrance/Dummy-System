#ifndef MISC_H
#define MISC_H

#define BUFFER_SIZE 1024
#define MAX_PORT 65535
#define MAX_THREAD_COUNT 10

// network.c function prototypes
extern int connectToServer( char *, int );

// user_io.c function prototypes
extern void getServer( char * );
extern void getPort( int * );

#endif