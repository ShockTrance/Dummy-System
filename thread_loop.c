#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include "misc.h"

static int getLineFromInput (
								const char * input, // string containing multiple lines
								char * buf // buffer to store one line
							)
{
	assert( NULL != input );
	assert( NULL != buf );
	
	unsigned i = 0;
	
	memset( buf, 0, BUFFER_SIZE * sizeof(char) );
	
	for ( i = 0; i < BUFFER_SIZE - 1; ++i )
	{
		if ( '\0' == input[i] )
			break;
		else if ( '\n' == input[i] || ('\r' == input[i] && '\n' == input[i+1] ) )
		{
			buf[i] = '\0';
			while ( isspace( input[i] ) ) i++;
			break;
		}
			
		buf[i] = input[i];
	}
	
	buf[i] = '\0';
	
	return i;
}

void * connectedLoop( void * args )
{
	/* INITIALIZATION SECTION - START */

	assert( NULL != args );

	thread_args_t * param = (thread_args_t *) args;
	bool * connected = param->conn;
	int socket = param->sockfd;
	
	assert( NULL != connected );
	assert( 0 < socket );
	
	sendNick( "DUMMY_SYS", socket );
	sendUser( "DMY_SYS", "Dummy System", socket );
	
	/* INITIALIZATION SECTION - END */


	/* CENTRAL CONNECTION LOOP - START */
	
	char * input = calloc( BUFFER_SIZE, sizeof(char) );
	char * tmpInput = NULL;
	char * curr = calloc( BUFFER_SIZE, sizeof(char) );
	unsigned pos = 0, i = 0;
	int len = 0;
	bool init_ping_sent = false;
	
	while ( *connected )
	{
		if ( 0 >= recv( socket, input, BUFFER_SIZE, 0 ) )
		{
			perror( "recv error" );
			pthread_exit( NULL );
		}
		
		tmpInput = input;
		
		while ( 0 < (len = getLineFromInput( (const char *) tmpInput, curr )) )
		{
			fprintf( stdout, "%s\n", curr );
		
			pos = getCommandEndPos( curr );
		
			if ( 0 == strncmp( (const char *) curr, "PING", pos ) )
			{
				sendPong( curr, socket );
				
				// The server doesn't allow us to start joining channels until
				// after we respond to the initial PING message.
				if ( !init_ping_sent )
				{
					sleep(1);
				
					for ( i = 0; i < MAX_CHANNELS; ++i )
					{
						if ( '\0' == param->chans[i][0] ) break;
						else if ( '$' == param->chans[i][0] ) break;
						else joinChannel( (const char *) param->chans[i], socket );
					}
					
					init_ping_sent = true;
				}
			}
			
			tmpInput = &tmpInput[len];
		}
		
		memset( input, 0, BUFFER_SIZE * sizeof(char) );
		memset( curr, 0, BUFFER_SIZE * sizeof(char) );
	}
	
	free( input );
	
	/* CENTRAL CONNECTION LOOP - END */
	
	return NULL;
}