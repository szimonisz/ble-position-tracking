#include "trilatmath.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

Coordinate centroidOfTriangle(Coordinate* trianglePoints){
   Coordinate centroid;
   centroid.x = (trianglePoints[0].x + trianglePoints[1].x + trianglePoints[2].x)/3;
   centroid.y = (trianglePoints[0].y + trianglePoints[1].y + trianglePoints[2].y)/3;
   return centroid;
}

// Find distance between circle center and point.
// If distance < radius: point is inside circle
//    distance == radius: point is on circle's edge
//    distance > radius: point is outside circle
//    d^2 = (x-center_x)^2 + (y - center_y)^2
// Use distance squared for efficiency, compare to r^2

bool pointInCircle(Coordinate point, Circle circle){
    double dx = circle.center.x - point.x;
    double dy = circle.center.y - point.y;

    // .5 is for C++ double rounding error...
    double squaredDistance = ((dx*dx) + (dy*dy))-.5;
    if(squaredDistance <= (circle.radius*circle.radius)){
        return true;
    }
    return false;
}

bool pointInAllCircles(Coordinate point, Circle circleA, Circle circleB, Circle circleC){
    bool inCircleA = pointInCircle(point,circleA);
    bool inCircleB = pointInCircle(point,circleB);
    bool inCircleC = pointInCircle(point,circleC);

    if(pointInCircle(point,circleA) && pointInCircle(point,circleB) && pointInCircle(point,circleC)){
        return true;
    }
    return false;
}

/*
 * Function: intersectionTwoCircles
 * ----------------------------------
 * Returns the points of intersection of two overlapping circles.
 *     circleA, circleB: two overlapping circles
 *
 *     returns: two coordinates where the overlapping circles intersect
 *
 * http://stackoverflow.com/a/3349134/798588
 * Distance between centers C1 and C2 is calculated as d:
 *     d^2 = (x1 - x2)^2 + (y1 - y2)^2
 *     d   = sqrt((x1 - x2)^2 + (y1 - y2)^2)
 */
Coordinate* intersectionTwoCircles(Circle circleA, Circle circleB){
    Coordinate* intersections = malloc(2 * sizeof(Coordinate)); 

    // vertical and horizontal distances between the two centers
    double dx = (circleB.center.x - circleA.center.x);
    double dy = (circleB.center.y - circleA.center.y);
    
    // straight line distance between the two centers
    double d = hypot(dx,dy);

    if( d > circleA.radius + circleB.radius){ 
        printf("ERROR: Circles do not intersect\n");
    }

    if(d < fabs(circleA.radius - circleB.radius)){
        printf("ERROR: A circle is contained inside the other\n");
    }

    double a = ((circleA.radius*circleA.radius) - (circleB.radius*circleB.radius) + (d*d)) / (2.0 * d) ;

 /* Determine the coordinates of point 2. */

    double x2 = circleA.center.x + (dx * a/d);
    double y2 = circleA.center.y + (dy * a/d);

    double R = circleA.radius;
    double h = sqrt((R*R) - (a*a));
    //printf("h = sqrt(%f*%f - %f*%f)\n",R,R,a,a);
    double xm = circleA.center.x + a*dx/d;
    double ym = circleA.center.y + a*dy/d;
    
    double rx = -dy * (h/d);
    double ry = dx * (h/d);

    intersections[0].x = x2 + rx;
    intersections[1].x = x2 - rx;

    intersections[0].y = y2 + ry;
    intersections[1].y = y2 - ry;

    return intersections;
}

/*
 * Function: intersectionThreeCircles
 * ----------------------------------
 * Returns the points of intersection of three overlapping circles.
 *     circleA, circleB, circleC: three overlapping circles
 *
 *     returns: six coordinates where the overlapping circles intersect
 *
 */

Coordinate* intersectionThreeCircles(Circle circleA, Circle circleB, Circle circleC){
    Circle circle[3] = {circleA, circleB, circleC};
    Coordinate* intersections = malloc(6 * sizeof(Coordinate));

    Coordinate* intersectionsAB = intersectionTwoCircles(circleA,circleB);   
    Coordinate* intersectionsAC = intersectionTwoCircles(circleA,circleC);   
    Coordinate* intersectionsBC = intersectionTwoCircles(circleB,circleC);   

    intersections[0] = intersectionsAB[0];
    intersections[1] = intersectionsAB[1];

    intersections[2] = intersectionsAC[0];
    intersections[3] = intersectionsAC[1];

    intersections[4] = intersectionsBC[0];
    intersections[5] = intersectionsBC[1];

    return intersections;
}

/*
 * Function: trilateration
 * ----------------------------------
 * 1. Given three coordinate centers and radii, form circles.
 * 2. Collect the points of intersection for each circle pair.
 * 3. Determine which of these points are found within each circle's area. 
 *    (Points defining the space where all three circles overlap)
 * 4. Form a triangle from the circle overlap points. 
 * 5. Find and return the centroid coordinate of the triangle. 
 *    (Estimated position coordinate of the target device) 
 *
 *     Coordinate posA, posB, posC: 2-D position of each object (circle center)
 *     double     rA, rB, rC: radius of each circle 
 *
 *     returns: a coordinate representing the centroid of the overlapping circles
 */
/*
Coordinate posA = {0,0};
Coordinate posB = {-3.236, 2.286};
Coordinate posC = {-2.9062, 6.668};
double rA = 3.414549;
double rB = 2.154435;
double rC = 4.298662;
Circle circleA = {posA,rA};    
Circle circleB = {posB,rB};    
Circle circleC = {posC,rC};    
*/

Coordinate trilateration(Coordinate posA, Coordinate posB, Coordinate posC, double rA, double rB, double rC){
    Circle circleA = {posA,rA};    
    Circle circleB = {posB,rB};    
    Circle circleC = {posC,rC};    
   
    // valid three-circle overlap should always involve 6 points of intersection
    // (two PoI for each circle)

    Coordinate* intersections = intersectionThreeCircles(circleA,circleB,circleC);
    Coordinate* overlapTriangle = malloc(3 * sizeof(Coordinate));

    // determine which intersections of overlapping circles exist in each circle
    // finding triangle of points that represent the central overlap of ALL 3 circles
    int t = 1;

    //printf("Intersections\n");
    for(int i = 0; i < 6; i++){
        //printf("(%f,%f)\n",intersections[i].x,intersections[i].y);
        if(pointInAllCircles(intersections[i],circleA,circleB,circleC)){
            overlapTriangle[t] = intersections[i];
            //printf("Triangle point: (%f,%f)\n",intersections[i].x,intersections[i].y);
            t++;
        } 
    }  
    printf("Triangle points: %d\n", t);
    Coordinate centroid = centroidOfTriangle(overlapTriangle);
    return centroid;
}

Coordinate perfectTrilateration(Coordinate posA, Coordinate posB, Coordinate posC, double r1, double r2, double r3){
   double x1 = posA.x, x2 = posA.x, x3 = posC.x, y1 = posA.y, y2 = posB.y, y3 = posC.y;

   double A = 2*x2 - 2*x1;
   double B = 2*y2 - 2*y1;
   double C = (r1*r1) - (r2*r2) - (x1*x1) + (x2*x2) - (y1*y1) + (y2*y2);
   double D = 2*x3 - 2*x2;
   double E = 2*y3 - 2*y2;
   double F = (r2*r2) - (r3*r3) - (x2*x2) + (x3*x3) - (y2*y2) + (y3*y3);

   double estimated_target_x = (C*E - F*B) / (E*A - B*D);
   double estimated_target_y = (C*D - A*F) / (B*D - A*E);
   Coordinate target_location = {estimated_target_x,estimated_target_y};
   return target_location;
}

/* 
 * Function: rssiToMeters
 * ----------------------------------
 * Converts an RSSI reading to meters using: 
 *     distance in meters = 10 ^ ((measuredPower - RSSI) / (10 * pathLossConstant))
 *         
 *     RSSI: Received Signal Strength Indicator
 *     measuredPower: RSSI at 1 meter (calibrated by user physical via measurement)
 *     pathLossConstant: Ranges from 1.8-4.5 depending on environment. Urban areas: 3. Open space: 1.8
 *     
 * https://iotandelectronics.wordpress.com/2016/10/07/how-to-calculate-distance-from-the-rssi-value-of-the-ble-beacon/
 */

double rssiToMeters(int rssi, int measuredPower, double pathLossConstant){
    double ratio_linear = pow(10.0, (measuredPower - rssi)/(10.0 * pathLossConstant));
    double distance = sqrt(ratio_linear); 
    //expands the radii enough to intersect consistently...
    double fudge = 2;
    return distance + fudge;
}
