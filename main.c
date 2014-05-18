#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "misc.h"

static void freeChannelArray( char ** array )
{
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i ) free( array[i] );
	free( array );
}

static char ** initChannelArray( void )
{
	char ** ret = malloc( MAX_CHANNELS * sizeof(char *) );
	
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i )
		ret[i] = malloc( BUFFER_SIZE * sizeof(char) );
	
	return ret;
}

int main( int argc, char ** argv )
{
	/* INITIALIZATION SECTION - START */
	
	char * server = malloc( BUFFER_SIZE * sizeof(char) );
	char ** chans = initChannelArray();
	int port = 0, socket = -1;
	
	/* INITIALIZATION SECTION - END */
	
	
	/* INITIAL CONNECTION SECTION - START */
	
	// We need to connect to at least one server for the program to continue,
	// so loop until we establish a valid connection.
	while ( 0 > socket )
	{
		getServer( server );
		getPort( &port );
		
		// Apparently, some manner of stray character is left in stdin thanks to
		// prior input calls, so this line is here to eliminate that character.
		fgetc( stdin );
		
		// The user must specify the channel(s) that they intend to join 
		// before the connection is established so that the connection
		// doesn't time out due to the program waiting for input.
		// Note that this may be changed if the program's flow is reorganized.
		getChannels( chans );
	
		socket = connectToServer( (const char *) server, port );
	}
	
	/* INITIAL CONNECTION SECTION - END */
	
	
	/* THREAD CREATION SECTION - START */
	
	thread_conn_t threads[MAX_THREAD_COUNT];
	unsigned cnt = 0;
	void * ret = NULL;
	thread_args_t * arg = malloc( sizeof(thread_args_t) );
	bool * connection = malloc( sizeof(bool) );
	*connection = true;
	
	/** Add extra initialization lines here as more arguments are needed **/
	arg->sockfd = socket;
	arg->chans = initChannelArray();
	memcpy( arg->chans, chans, MAX_CHANNELS * BUFFER_SIZE * sizeof(char) );	
	arg->conn = connection;
	threads[cnt].conn = connection;

	
	if ( 0 != pthread_create( &(threads[cnt++].threadID), NULL, connectedLoop, arg ) )
	{
		perror( "pthread_create error" );
		exit( EXIT_FAILURE );
	}
	
	for ( unsigned i = 0; i < cnt; ++i )
	{
		//threads[i].conn = false;
		pthread_join( threads[i].threadID, &ret );
	}
	
	/* THREAD CREATION SECTION - END */
	
	
	freeChannelArray( chans );
	free( server );
	free( chans );
	free( arg );

	return EXIT_SUCCESS;
}