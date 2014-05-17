#include <stdio.h>
#include <assert.h>
#include "misc.h"

/* Function name: getServer
 * Purpose: Retrieve the URL or IP address of an IRC server to which the
 * 			bot will connect.
 * Return value: None.
 */
void getServer ( 
				char * server // buffer of length BUFFER_SIZE for the user input
			   )
{
	assert( NULL != server );

	fprintf( stdout, "Enter server: " );
	if ( NULL == fgets( server, BUFFER_SIZE, stdin ) )
	{
		perror( "fgets error" );
		exit( EXIT_FAILURE );
	}
}

/* Function name: getPort
 * Purpose: Retrieve the TCP port for a previously specified IRC server.
 * Return value: None.
 */
void getPort (
				int * port // buffer for the user input
			 )
{
	assert( NULL != port );
	
	*port = 0;
	
	while ( 0 >= *port || MAX_PORT < *port )
	{
		fprintf( stdout, "Enter port (default = 6667): " );
		if ( 1 != fscanf( stdin, "%d", port ) )
		{
			perror( "fscanf error" );
			exit( EXIT_FAILURE );
		}
		
		if ( 0 >= *port ) fprintf( stdout, "Port must be greater than zero\n" );
		else if ( MAX_PORT < *port ) fprintf( stdout, "Port must be less than %d\n", MAX_PORT );
	}
}