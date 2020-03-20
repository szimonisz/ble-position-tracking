#ifndef trilat_math_H
#define trilat_math_H
#include <stdbool.h>

typedef struct{
    double x;
    double y;
}Coordinate;

typedef struct{
    Coordinate center;
    double radius;
}Circle;


Coordinate centroidOfTriangle(Coordinate* trianglePoints);

bool pointInCircle(Coordinate point, Circle circle);

bool pointInAllCircles(Coordinate point, Circle circleA, Circle circleB, Circle circleC);

/* 
 * Function: intersectionTwoCircles
 * ----------------------------------
 * Returns the points of intersection of two overlapping circles. 
 *     circleA, circleB: two overlapping circles
 *
 *     returns: two coordinates where the overlapping circles intersect
 *     
 * http://stackoverflow.com/a/3349134/798588
 */

Coordinate* intersectionTwoCircles(Circle circleA, Circle circleB);

/*
 * Function: intersectionThreeCircles
 * ----------------------------------
 * Returns the points of intersection of three overlapping circles.
 *     circleA, circleB, circleC: three overlapping circles
 *
 *     returns: six coordinates where the overlapping circles intersect
 *
 */

Coordinate* intersectionThreeCircles(Circle circleA, Circle circleB, Circle circleC);

Coordinate perfectTrilateration(Coordinate posA, Coordinate posB, Coordinate posC, double rA, double rB, double rC);

Coordinate trilateration(Coordinate posA, Coordinate posB, Coordinate posC, double rA, double rB, double rC);

/* 
 * Function: rssiToMeters
 * ----------------------------------
 * Converts an RSSI reading to meters using: 
 *     distance in meters = 10 ^ ((measuredPower - RSSI) / (10 * n))
 *         
 *     RSSI: Received Signal Strength Indicator
 *     measuredPower: RSSI at 1 meter (calibrated by user physical via measurement)
 *     n: Path-Loss Constant (Ranges from 1.8-4.5 depending on environment. Urban areas: 3. Open space: 1.8
 *     
 * https://iotandelectronics.wordpress.com/2016/10/07/how-to-calculate-distance-from-the-rssi-value-of-the-ble-beacon/
 */

double rssiToMeters(int rssi, int measuredPower, double pathLossConstant);

#endif
