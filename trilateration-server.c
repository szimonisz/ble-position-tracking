/*
        udp-recv: a simple udp server
       
        Sniff, receive, and display UDP packets delivered to a specified port (within port.h)
        Biscuit device is sending to port 4120.

        usage:  ./udp-recv

        A modified version of Paul Krzyzanowski's demo-udp-03
        https://www.cs.rutgers.edu/~pxk/417/notes/sockets/demo-udp-03.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "port.h"
#include <sys/time.h>
#include <math.h>

#define BUFSIZE 2048

int g_rssiA, g_rssiB, g_rssiC;

int main(int argc, char **argv){

	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);	/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	char buf[BUFSIZE];	/* receive buffer */
        struct timeval tv;              // time value struct for timestamping

	// create a UDP socket
        // this is what the biscuit will send UDP packets to

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port (specified in port.h)*/

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now loop infinitely, waiting for data, receiving data, printing what we received */

        char* scannerA = "10.0.0.160";
        char* scannerB = "10.0.0.192";
        char* scannerC = "10.0.0.100";

        g_rssiA = 0;
        g_rssiB = 0;
        g_rssiC = 0;

        // once readings from each device are collected, run algorithm
  
	while(1) {
		printf("waiting on port %d\n", SERVICE_PORT);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
                char* fromAddress = inet_ntoa(remaddr.sin_addr);

		printf("Received %d bytes\n", recvlen);
                printf("Received packet from %s:%d\n",
                   inet_ntoa(remaddr.sin_addr),ntohs(remaddr.sin_port));
                gettimeofday(&tv, NULL);
		printf("Timestamp: %ld.%06d\n", tv.tv_sec, tv.tv_usec);
                if (recvlen > 0) {
			buf[recvlen] = '\0';
                        printf("Received message:\n\%s\n\n",buf);
		}
                // ignore timestamping for now...
                // after accumulating a reading from each device, plug in to alg
                if(strcmp(scannerA,fromAddress) == 0){
                    g_rssiA = atoi(buf); 
                }
                if(strcmp(scannerB,fromAddress) == 0){
                    g_rssiB = atoi(buf); 
                }
                if(strcmp(scannerC,fromAddress) == 0){
                    g_rssiC = atoi(buf); 
                }
                if(g_rssiA < 0 && g_rssiB < 0 && g_rssiC < 0){
                    printf("RSSI values collected from each device.\n");
                    printf("rssiA: %d\n",g_rssiA);
                    printf("rssiB: %d\n",g_rssiB);
                    printf("rssiC: %d\n\n",g_rssiC);

                    //distance (meters) = 10 ^ ((Ptx - RSSI) / (10 * n))
                    double Ptx = -58; //self-calibrated RSSI at 1 meter
                    double n = 3.0;
                    double dA = pow(10.0,((Ptx - g_rssiA) / (10.0 * n)));
                    double dB = pow(10.0,((Ptx - g_rssiB) / (10.0 * n)));
                    double dC = pow(10.0,((Ptx - g_rssiC) / (10.0 * n)));
                    printf("dA: %f\n",dA);
                    printf("dB: %f\n",dB);
                    printf("dC: %f\n",dC);
                    return 0;
                }
	}
}
