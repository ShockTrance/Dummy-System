#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

/* Function name: sendPong
 * Purpose: Takes a PING message sent by an IRC server, extracts the message
 *			to be returned, and sends the appropriate PONG message back.
 * Return value: None.
 */
void sendPong (
				const char * ping, // string containing "PING <message>\r\n"
				int sockfd // socket file descriptor for the connection
			  )
{
	assert( NULL != ping );
	assert( 0 < sockfd );

	char msg[BUFFER_SIZE] = {0}, output[BUFFER_SIZE] = {0};
	unsigned pos = 0;
	
	// The first five letters of the ping message will always be "PING ", 
	// so the message we need to return begins at the sixth position.
	while ( ping[pos+5] != '\r' && ping[pos+5] != '\n' ) msg[pos++] = ping[pos+5];
	
	sprintf( output, "PONG %s\r\n", msg );
	
	if ( 0 > send( sockfd, output, strlen( output ), 0 ) )
	{
		perror( "send error" );
		pthread_exit( NULL );
	}
}