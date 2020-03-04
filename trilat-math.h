#ifndef trilatmath_H
#define trilatmath_H

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

double rssiToMeters(int rssi, int measuredPower, int pathLossConstant);

#endif
