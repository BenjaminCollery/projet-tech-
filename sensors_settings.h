#ifndef _SENSORS_SETTINGS_H_
#define _SENSORS_SETTINGS_H_

/* Does the module include a DHT22 instead of a DS1820
 */
#define HAS_DHT22 // ligne à commenter


/* Time intervals sensors are polled for their value.
 * Given in milliseconds.
 */
#define DELTA_T_LUM 3000
#define DELTA_T_TMP 3000
#define DELTA_T_HUM 3000

/* Threshold for sensor values.
 * If the difference between two consecutive values
 * is under this threshold, the value is not sent.
 */
#define DELTA_V_LUM 1                //10
#define DELTA_V_TMP 0.001                //0.1
#define DELTA_V_HUM 0.01                     //0.1


/* We do not accept commands if they are sent too fast.
 * Wait 2 seconds between commands.
 */
#define DELTA_T_CMD 2000

/* We do not accept commands shorter than this.
 * Argument to commands (if any) are of the same length.
 */
#define CMD_LENGTH 4

/* What are the names of the values sent.
 * A dirty hack to allow to modify them quickly.
 */
#define CAPA_LUM "LUMI"
#define CAPA_TEMP "TEMP"
#ifdef HAS_DHT22
#define CAPA_HMDT "HMDT"
#endif

#endif
