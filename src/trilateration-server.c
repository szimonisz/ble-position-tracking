/*
        udp-recv: a simple udp server
       
        Sniff, receive, and display UDP packets delivered to a specified port (within port.h)
        Biscuit device is sending to port 4120.

        usage:  ./udp-recv

        UDP related initalization credited to Paul Krzyzanowski's demo-udp-03
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
#include "trilatmath.h"

#define BUFSIZE 2048
#define NUM_SCANNERS 3
#define SCANNERADDRESS_A "10.0.0.160"
#define SCANNERADDRESS_B "10.0.0.192"
#define SCANNERADDRESS_C "10.0.0.100"
<<<<<<< HEAD:src/trilateration-server.c

#define WORLDPOSITION_X_A 0.0
#define WORLDPOSITION_Y_A 0.0

#define WORLDPOSITION_X_B -3.236
#define WORLDPOSITION_Y_B 2.286

#define WORLDPOSITION_X_C -2.9062
#define WORLDPOSITION_Y_C 6.668
=======
#define WORLDPOSITION_A (0,0)
#define WORLDPOSITION_B (1,3)
#define WORLDPOSITION_C (1,3)
>>>>>>> 35b93c2c1a3bc9472885262febdee3270dff6041:trilateration-server.c

#define MEASURED_POWER_A -58
#define MEASURED_POWER_B -55
#define MEASURED_POWER_C -54
#define PATH_LOSS_CONSTANT 2

#define WORLDPOSITION_X_TARGET -2.5149
#define WORLDPOSITION_Y_TARGET 4.4577

typedef struct{
    char* address;
    char name; // to differientiate the scanners (A,B,C)
    int measuredPower; // RSSI value at 1 Meter (measured physically)
    int rssi;
    double distance; // estimated distance from the target device in meters (we calculate)
    Coordinate worldPos; // measured location of scanner relative to origin
}BLEscanner;

int main(int argc, char **argv){

	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);	/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	char buf[BUFSIZE];	/* receive buffer */
        struct timeval tv;              // time value struct for timestamping

	// create a UDP socket

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
        
        // initialize BLE Scanners
        BLEscanner* scanner = malloc(3 * sizeof(BLEscanner));
        
        scanner[0].address = SCANNERADDRESS_A;
        scanner[0].name =  'A';
        scanner[0].measuredPower = MEASURED_POWER_A;
        scanner[0].worldPos.x = WORLDPOSITION_X_A;
        scanner[0].worldPos.y = WORLDPOSITION_Y_A;

        scanner[1].address = SCANNERADDRESS_B;
        scanner[1].name =  'B';
        scanner[1].measuredPower = MEASURED_POWER_B;
        scanner[1].worldPos.x = WORLDPOSITION_X_B;
        scanner[1].worldPos.y = WORLDPOSITION_Y_B;

        scanner[2].address = SCANNERADDRESS_C;
        scanner[2].name =  'C';
        scanner[2].measuredPower = MEASURED_POWER_C;
        scanner[2].worldPos.x = WORLDPOSITION_X_C;
        scanner[2].worldPos.y = WORLDPOSITION_Y_C;
   
       
        // once readings from each device are collected, run algorithm
  
	while(1){
	    printf("waiting on port %d\n", SERVICE_PORT);
            recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
            char* clientAddress = inet_ntoa(remaddr.sin_addr);
            unsigned short clientPort = ntohs(remaddr.sin_port); 

            printf("Received %d bytes in packet from %s:\n",recvlen,clientAddress);
            if (recvlen > 0) {
                buf[recvlen] = '\0';
                printf("Received message: %s\n",buf);
            }
                
            // When scanners first connect, they send UDP packet 'Connected'
            if (recvlen == 3){
              // received packet was an RSSI value
              // determine which scanner the packet was sent from, thens set RSSI of scanner
                for(int i = 0; i < NUM_SCANNERS; i++){
                    if(strcmp(scanner[i].address,clientAddress) == 0){
                        scanner[i].rssi = atoi(buf); 
                        scanner[i].distance = rssiToMeters(scanner[i].rssi, scanner[i].measuredPower, PATH_LOSS_CONSTANT); 
                    }
                }

                // have to wait for all scanners to be updated with an rssi before trilateration

                if(scanner[0].rssi < 0 && scanner[1].rssi < 0 && scanner[2].rssi < 0){

                    printf("RSSI values collected from each device:\n");
                    for(int i = 0; i < NUM_SCANNERS; i++){
                        printf("\tScanner %c RSSI: %d\n",scanner[i].name,scanner[i].rssi);
                    } 
                    printf("Estimated distance values calculated for each device:\n");
                    for(int i = 0; i < NUM_SCANNERS; i++){
                        printf("\tScanner %c Distance (meters): %f\n",scanner[i].name,scanner[i].distance);
                    }
                    Coordinate estimatedTargetPos = trilateration(scanner[0].worldPos,scanner[1].worldPos,scanner[2].worldPos,scanner[0].distance,scanner[1].distance,scanner[2].distance);
                    printf("Estimated target location via trilateration: (%f,%f)\n",estimatedTargetPos.x,estimatedTargetPos.y);
                    printf("Real target location via trilateration: (%f,%f)\n\n", WORLDPOSITION_X_TARGET, WORLDPOSITION_Y_TARGET);

                    printf("X-position error: %f meters off\n",fabs(fmax(WORLDPOSITION_X_TARGET,estimatedTargetPos.x) - fmin(WORLDPOSITION_X_TARGET,estimatedTargetPos.x)));
                    printf("Y-position error: %f meters off\n",fabs(fmax(WORLDPOSITION_Y_TARGET,estimatedTargetPos.y) - fmin(WORLDPOSITION_Y_TARGET,estimatedTargetPos.y)));

                    //reset RSSI and distances
                    for(int i = 0; i < 3; i++){
                        scanner[i].rssi = 0;
                        scanner[i].distance = 0;
                    }
                }
            }
        }
    return 0;
}
