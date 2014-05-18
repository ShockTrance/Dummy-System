#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <ctype.h>
#include "misc.h"

/* Function name: getCommandEndPos
 * Purpose: Locates the ending position of the command identifier in a given
 *			IRC command received from the server.
 * Return value: The position of the character immediately after the end of 
 *				 the command.
 */
unsigned getCommandEndPos (
							const char * cmd // string containing the IRC command
				          )
{
	assert( NULL != cmd );

	unsigned ret = 0;
	
	while ( !isspace( cmd[ret] ) ) ret++;
	
	return ret;
}

/* Function name: joinChannel
 * Purpose: Sends a JOIN <channel> command to the IRC server.
 * Return value: None.
 */
void joinChannel (
					const char * chan, // name of the channel to join
					int sockfd // socket file descriptor for the connection
				 )
{
	char output[BUFFER_SIZE] = {0};
	
	sprintf( output, "JOIN %s\r\n", chan );
	
	if ( 0 > send( sockfd, output, strlen( output ), 0 ) )
	{
		perror( "send error" );
		pthread_exit( NULL );
	}
}

/* Function name: sendNick
 * Purpose: Sends a message to the IRC server specifying the new nickname
 *			to be used by the program.
 * Return value: None.
 */
void sendNick (
				const char * nick, // string containing the desired nick
				int sockfd // socket file descriptor for the connection
			  )
{
	assert( NULL != nick );
	assert( 0 < sockfd );

	char output[BUFFER_SIZE] = {0};
	
	sprintf( output, "NICK %s\r\n", nick );
	
	if ( 0 > send( sockfd, output, strlen( output ), 0 ) )
	{
		perror( "send error" );
		pthread_exit( NULL );
	}
}

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
	while ( !isspace( ping[pos+5] ) && '\0' != ping[pos+5] )
	{
		msg[pos] = ping[pos+5];
		pos++;
	}
	
	sprintf( output, "PONG %s\r\n", msg );
	
	if ( 0 > send( sockfd, output, strlen( output ), 0 ) )
	{
		perror( "send error" );
		pthread_exit( NULL );
	}
}

/* Function name: sendUser
 * Purpose: Sends the desired username and real name as a USER command to 
 *			the IRC server.
 * Return value: None. 
 */
void sendUser (
				const char * user, // string containing the desired username
				const char * name, // string containing the desired real name
				int sockfd // socket file descriptor for the connection
			  )
{
	assert( NULL != user );
	assert( NULL != name );
	assert( 0 < sockfd );
	
	char output[BUFFER_SIZE] = {0};
	
	sprintf( output, "USER %s 8 * :%s\r\n", user, name );
	
	if ( 0 > send( sockfd, output, strlen( output ), 0 ) )
	{
		perror( "send error" );
		pthread_exit( NULL );
	}
}