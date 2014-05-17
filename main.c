#include <stdlib.h>
#include <pthread.h>
#include "misc.h"

int main( int argc, char ** argv )
{
	char server[BUFFER_SIZE] = {0};
	int port = 0, socket = -1;
	
	// We need to connect to at least one server for the program to continue,
	// so loop until we establish a valid connection.
	while ( 0 > socket )
	{
		getServer( server );
		getPort( &port );
	
		socket = connectToServer( server, port );
	}
	
	int count = 0;
	pthread_t threads[MAX_THREAD_COUNT] = {0};

	return EXIT_SUCCESS;
}