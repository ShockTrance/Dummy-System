#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"

#include <string.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct addrinfo addrinfo;

/* Function name: connectToServer
 * Purpose: Resolves the specified server URL, then connects to it using the
 * 			specified port through TCP.
 * Return value: A socket file descriptor if the connection attempt was 
 *				 successful; -1 otherwise.
 */
int connectToServer (
						const char * server, // URL or IP address of the server
						int port // port to use for the connection
					)
{
	addrinfo * list = NULL;
	int ret_val = 0;
	
	ret_val = getaddrinfo( server, NULL, NULL, &list );

	if ( 0 == ret_val );
	else
	{
		fprintf( stdout, "getaddrinfo returned %d\n", ret_val );
	
		//perror( "getaddrinfo error" );
		if (ret_val == EAI_SYSTEM)
        {
            perror("getaddrinfo");
        }
        else
        {
            fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(ret_val));
        }   
		
		exit( EXIT_FAILURE );
	}
	
	sockaddr * conn_addr = NULL;
	sockaddr_in * addr_in = NULL;
	sockaddr_in6 * addr_in6 = NULL;
	int sockfd = -1;
	
	while ( NULL != list )
	{
		fprintf( stdout, "Attempting to connect to %s:%d...\n", list->ai_canonname, port );
	
		if ( 0 > (sockfd = socket( list->ai_family, SOCK_STREAM, IPPROTO_TCP )) )
		{
			perror( "socket error" );
			exit( EXIT_FAILURE );
		}
		
		// The generic sockaddr struct doesn't let us set the address and port
		// and such.  Thus, we have to check the protocol family of the current
		// address and initialize either a sockaddr_in or a sockaddr_in6 struct
		// as appropriate.
		if ( PF_INET == list->ai_family )
		{
			addr_in = calloc( 1, sizeof(sockaddr_in) );
			
			addr_in->sin_family = PF_INET;
			addr_in->sin_port = htons( port );
			addr_in->sin_addr = ((sockaddr_in *) list->ai_addr)->sin_addr;
			
			conn_addr = (sockaddr *) addr_in;
		}
		else if ( PF_INET6 == list->ai_family )
		{
			addr_in6 = calloc( 1, sizeof(sockaddr_in6) );
			
			addr_in6->sin6_family = PF_INET6;
			addr_in6->sin6_port = htons( port );
			addr_in6->sin6_addr = ((sockaddr_in6 *) list->ai_addr)->sin6_addr;
			
			conn_addr = (sockaddr *) addr_in6;
		}
		else goto next_iter;
		
		if ( 0 > connect( sockfd, (const sockaddr *) conn_addr, sizeof(sockaddr) ) )
		{
			perror( "connect error" );
			goto next_iter;
		}
		else break;
		
		next_iter:
		sockfd = -1;
		list = list->ai_next;
	}
	
	if ( -1 == sockfd ) fprintf( stderr, "Could not connect to the specified server\n" );
	else fprintf( stdout, "Successfully connected to %s\n", server );
	
	return sockfd;
}
