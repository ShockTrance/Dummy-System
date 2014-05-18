#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "misc.h"

int main( int argc, char ** argv )
{
	/* INITIALIZATION SECTION - START */
	
	char * server = malloc( BUFFER_SIZE * sizeof(char) );
	char ** chans = malloc( MAX_CHANNELS * sizeof(char *) );
	int port = 0, socket = -1;
	
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i )
		chans[i] = malloc( BUFFER_SIZE * sizeof(char) );
		
	/* INITIALIZATION SECTION - END */
	
	
	/* INITIAL CONNECTION SECTION - START */
	
	// We need to connect to at least one server for the program to continue,
	// so loop until we establish a valid connection.
	while ( 0 > socket )
	{
		getServer( server );
		getPort( &port );
		
		// The user must specify the channel(s) that they intend to join 
		// before the connection is established so that the connection
		// doesn't time out due to the program waiting for input.
		// Note that this may be changed if the program's flow is reorganized.
		getChannels( chans );
	
		socket = connectToServer( (const char *) server, port );
	}
	
	/* INITIAL CONNECTION SECTION - END */
	
	
	/* THREAD CREATION SECTION - START */
	
	pthread_t threadID;
	void * ret = NULL;
	thread_args * arg = malloc( sizeof(thread_args) );
	
	/** Add extra initialization lines here as more arguments are needed **/
	arg->sockfd = socket;
	
	if ( 0 != pthread_create( &threadID, NULL, connectedLoop, arg ) )
	{
		perror( "pthread_create error" );
		exit( EXIT_FAILURE );
	}
	
	pthread_join( threadID, &ret );
	
	/* THREAD CREATION SECTION - END */
	
	
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i ) free( chans[i] );
	free( server );
	free( chans );
	free( arg );

	return EXIT_SUCCESS;
}