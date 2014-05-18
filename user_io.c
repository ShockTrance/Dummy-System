#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "misc.h"

/* Function name: getChannels
 * Purpose: Retrieve from the user up to MAX_CHANNELS channel to join once 
 *			the program connects to the current server.
 * Return value: None.
 */
void getChannels (
					char ** chans // input buffer (size MAX_CHANNELS * BUFFER_SIZE)
				 )
{
	assert( NULL != chans );
	
	unsigned i = 0;
	
	while ( i < MAX_CHANNELS )
	{
		fprintf( stdout, "Enter channel (%d left; $ to stop): ", MAX_CHANNELS - i );
		
		if ( NULL == fgets( chans[i], BUFFER_SIZE, stdin ) )
		{
			perror( "fgets error" );
			exit( EXIT_FAILURE );
		}
		
		if ( '$' == chans[i][0] )
		{
			// This might be changed in the future, since you don't have to 
			// connect to a channel to remain connected to an IRC server,
			// but I'll leave it this way for the time being.  I can't see 
			// much use of a bot connecting to a server and not joining a 
			// channel.
			if ( 0 == i ) 
			{
				fprintf( stdout, "At least one channel must be specified\n" );
				continue;
			}
			else break;
		}
		
		i++;
	}
}

/* Function name: getPort
 * Purpose: Retrieve the TCP port for a previously specified IRC server.
 * Return value: None.
 */
void getPort (
				int * port // input buffer
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

/* Function name: getServer
 * Purpose: Retrieve the URL or IP address of an IRC server to which the
 * 			bot will connect.
 * Return value: None.
 */
void getServer ( 
				char * server // input buffer (size BUFFER_SIZE)
			   )
{
	assert( NULL != server );
	
	char temp[BUFFER_SIZE] = {0};
	unsigned pos = 0;
	
	fprintf( stdout, "Enter server: " );
	if ( NULL == fgets( temp, BUFFER_SIZE, stdin ) )
	{
		perror( "fgets error" );
		exit( EXIT_FAILURE );
	}
	
	while ( !isspace( temp[pos] ) )
	{
		server[pos] = temp[pos];
		pos++;
	}
}

