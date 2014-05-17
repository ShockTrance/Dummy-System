#include <stdlib.h>
#include "misc.h"

int main( int argc, char ** argv )
{
	char server[BUFFER_SIZE] = {0}, chans[MAX_CHANNELS][BUFFER_SIZE] = {0};
	int port = 0, socket = -1;
	
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
	
		socket = connectToServer( server, port );
	}

	return EXIT_SUCCESS;
}