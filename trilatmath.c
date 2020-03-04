#include "trilatmath.h"
#include <math.h>

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

double rssiToMeters(int rssi, int measuredPower, int pathLossConstant){
    double distance = pow(10.0, (measuredPower - rssi)/(10.0 * pathLossConstant));
    return distance;
}

