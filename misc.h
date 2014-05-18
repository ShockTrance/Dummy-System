#ifndef MISC_H
#define MISC_H

#include <stdbool.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_PORT 65535
#define MAX_THREAD_COUNT 10
#define MAX_CHANNELS 5

typedef struct
{
	int sockfd;
	char ** chans;
	bool * conn;
} thread_args_t;

typedef struct
{
	pthread_t threadID;
	bool * conn;
} thread_conn_t;

// irc.c function prototypes
extern unsigned getCommandEndPos( const char * );
extern void sendNick( const char *, int );
extern void sendPong( const char *, int );
extern void sendUser( const char *, const char *, int );

// network.c function prototypes
extern int connectToServer( const char *, int );

// user_io.c function prototypes
extern void getChannels( char ** );
extern void getPort( int * );
extern void getServer( char * );

// thread_loop.c function prototypes
extern void * connectedLoop( void * );

#endif