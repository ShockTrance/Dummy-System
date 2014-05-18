#ifndef MISC_H
#define MISC_H

#define BUFFER_SIZE 1024
#define MAX_PORT 65535
#define MAX_THREAD_COUNT 10
#define MAX_CHANNELS 5

typedef struct
{
	int sockfd;
} thread_args;

// irc.c function prototypes
extern void sendPong( const char *, int );

// network.c function prototypes
extern int connectToServer( const char *, int );

// user_io.c function prototypes
extern void getChannels( char ** );
extern void getPort( int * );
extern void getServer( char * );

// thread_loop.c function prototypes
extern void * connectedLoop( void * );

#endif