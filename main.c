#include <stdlib.h>
#include "misc.h"

#include <stdio.h>

int main( int argc, char ** argv )
{
	char * server = malloc( BUFFER_SIZE * sizeof(char) );
	char ** chans = malloc( MAX_CHANNELS * sizeof(char *) );
	int port = 0, socket = -1;
	
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i )
		chans[i] = malloc( BUFFER_SIZE * sizeof(char) );
	
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
	
	for ( unsigned i = 0; i < MAX_CHANNELS; ++i ) free( chans[i] );
	free( server );
	free( chans );

	return EXIT_SUCCESS;
}