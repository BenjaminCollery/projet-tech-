/* Two sensors plugged to an ESP8266 (Adafruit HUZZAH)
 *  - Light : TEMT6000 on AnalogIn pin, through a voltage divider (Vout = 0.33 Vin)
 *            The analog input accepts values up to 1.0V.
 *  - Temperature/Humidity : DHT22 or DS1820, data pin on GPIO pin #4
 *  
 * Required card definitions:
 *  - ESP8266: http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 * Required libraries:
 *  - DallasTemperature
 *  - OneWire (Included in the latest versions of DallasTemperature)
 *  - PubSubClient (MQTT clients)
 *  - Adafruit Unified Sensor
 *  - Adafruit DHT sensor
 *
 */
 

#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266WiFi.h>

#include <PubSubClient.h>

/* ################################################################### */
/* When the module is INTO_THE_WILD, it is no more
 * in testing conditions.
 */
#define INTO_THE_WILD

#include "user_settings.h"
#include "sensors_settings.h"
#include "pins.h"


/* ################################################################### */
/* Software version
 */
#define FIRMWARE_VERSION "v0.7"

/* Set to 'true' to enable debugging output on Serial.
 */
#define DEBUG_OK false


/* ######################################################################
   ######################################################################
 */
#define debug(a) if ( DEBUG_OK ) { Serial.print(a) ; }

uint32_t t0_lum, t_lum ;
uint32_t t0_tmp, t_tmp ;
uint32_t t0_reconnect, t_reconnect ;
#if ( HEARTBEAT == 1 )
uint32_t t0_heartbeat, t_heartbeat ;
#define PULSE (5*60*1000) // 5 minutes
#endif
double lum, prev_lum ;
double tmp, prev_tmp ;
#ifdef HAS_DHT22
double hmdt, prev_hmdt ;
#endif
unsigned long timeout ;
uint8_t nb_tmp_sensors ;
String topic, payload ;
boolean force ;

void callback(const char* intopic, byte* payload, unsigned int length) ;

WiFiClient espClient ;
PubSubClient client(host, port, callback, espClient) ;

#ifdef HAS_DHT22
DHT dht(TMP, DHT22) ;
#else
OneWire oneWire(TMP);
DallasTemperature tmp_sensors(&oneWire);
#endif

/* ################################################################### */
void setup() {
  Serial.begin(9600) ;
  delay(100) ;                                                   // idée : bouton , au clic l'écran LCD donne l'état du module ( IP , etc ) , mais dans un deuxième temps
  debug("Initializing...\n") ;
  Serial.print("Module ") ;
  Serial.print(ID[0]) ;
  Serial.print(" (") ;
  Serial.print(ID[1]) ;
  Serial.println(")") ;
  Serial.print("Firmware ") ;
  Serial.println(FIRMWARE_VERSION) ;

  pinMode(BLUE_LED, OUTPUT) ;
  pinMode(RED_LED, OUTPUT) ;
  digitalWrite(BLUE_LED, LOW) ;  // HIGH means off here
  // ===================================================
#ifdef HAS_DHT22                                             // nous on a ca , possibilité d'erreur si le capteur ne répond pas -> à prévoir
  dht.begin() ;
#else
  // ===================================================
  tmp_sensors.begin() ;
  tmp_sensors.setWaitForConversion(true) ;

  debug("TMP sensors found: ") ;
  debug(nb_tmp_sensors = tmp_sensors.getDeviceCount()) ;
  debug("\n") ;
#endif
  // ===================================================
  debug("Connecting to ");
  debug(ssid);
  debug("\n") ;
 
  WiFi.mode(WIFI_STA);  // be a client only (not an AP)
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {                     // tant qu'il est pas connecté
    int sequence1[4]= {1,0,1,1};
    digitalWrite(BLUE_LED,0);
    digitalWrite(RED_LED,0);
    Serial.println("bleue et rouge éteint 2 sec");
    delay(2000);
    digitalWrite(BLUE_LED,1);                //     1 = allumé    ,    0 = éteint
    Serial.println("bleue allumé 2 sec");
    delay(2000);
    digitalWrite(BLUE_LED,0);
    for(int i = 0;i<4;i++){
      Serial.println("séquence en cours");
      digitalWrite(RED_LED,sequence1[i]);
      delay(2000);
    }
    
    
    
   // digitalWrite(BLUE_LED, LOW) ;
    //delay(500);
  //  digitalWrite(BLUE_LED, HIGH) ;
    debug(".");

    
  }
  digitalWrite(BLUE_LED, LOW) ;

  debug("\n");
  debug("WiFi connected\n");
  debug("IP address: \n");
  debug(WiFi.localIP().toString());
  debug("\n") ;
  // ===================================================
  client.setServer(host, port) ;
  // ===================================================  
  tmp = lum = -1.0 ;
  prev_tmp = prev_lum = -1.0 ;
#ifdef HAS_DHT22
  hmdt = -1.0 ;
  prev_hmdt = -1.0 ;
#endif
  force = true ;
  // ===================================================
  topic  = ROOT_TOPIC ; // Defined in user_settings.h
  topic += ID[1] ;      // Room
  topic += "/" ;
  topic += ID[2] ;      // Object type
  topic += "/" ;
  topic += ID[0] ;      // UUID
  // ===================================================
  debug("Done!\n") ;
  
  // ===================================================
  t0_reconnect = t0_tmp = t0_lum = millis() ;
#if ( HEARTBEAT == 1 )
  t0_heartbeat = t0_reconnect ;
#endif
}

/* ################################################################### */
void loop() {
  /* We try to connect to the broker and launch the client loop.
   */
  while ( ! client.connected() ) {                                     // si le pusblisher MQTT n'arrive pas à se co au broker
    t_reconnect = millis() ;

    
    if ( force || (t_reconnect - t0_reconnect > 5000) ) {           // il force
      t0_reconnect = t_reconnect ;

      if ( ! reconnect() ) {                                       

        Serial.println("MQTT: connection failed") ;
        debug(client.state()) ;
        Serial.println("MQTT: retrying in 5 seconds") ;
            int sequence1[4]= {1,0,1,0};
            digitalWrite(BLUE_LED,0);
            delay(2000);
            digitalWrite(BLUE_LED,1);                //     1 = éteint    ,    0 = allumé
            delay(2000);
            for(int i = 0;i<4;i++){
            digitalWrite(RED_LED,sequence1[i]);
            delay(2000);
            }
            digitalWrite(BLUE_LED,1);
            digitalWrite(RED_LED,1);
            
    }
  } else {
    client.loop() ;
  }}

#if ( HEATBEAT == 1 )
  t_heartbeat = millis() ;
  if ( t_heartbeat - t0_heartbeat > PULSE ) {
    t0_heartbeat = t_heartbeat ;
    force = true ;
  }
#endif

  /* Values are read from sensors at fixed intervals of time.
   */
  // ===================================================
  t_lum = millis() ;
  if ( force || (t_lum - t0_lum > DELTA_T_LUM) ) {
    t0_lum = t_lum ;
    lum = getLum() ;

    debug("LUM: ") ;
    debug(lum) ;
    debug("\n") ;
  }

  // ===================================================
  t_tmp = millis() ;
  if ( force || (t_tmp - t0_tmp > DELTA_T_TMP) ) {
    t0_tmp = t_tmp ;

#ifdef HAS_DHT22
    hmdt = getHmdt() ;
    debug("HUM: ") ;
    debug(hmdt) ;
    debug("\n") ;
#endif

    tmp = getTmp() ;
    debug("TMP: ") ;
    debug(tmp) ;
    debug("\n") ;
  }

  // ===================================================
  sendValues() ;

  force = false ;
}


/* ------------------------------------------------------------------- */
double getLum()
{
  double acc = 0 ;
  uint8_t n_val ;
  
  for ( n_val = 0 ; n_val < 10 ; n_val++ ) {
    acc += analogRead(A0) ;
    delay(5) ;
  }
  
  return acc/n_val ;
}


/* ------------------------------------------------------------------- */
double getTmp()
{
#ifdef HAS_DHT22
  return dht.readTemperature() ;
#else
  tmp_sensors.requestTemperaturesByIndex(0);
  return tmp_sensors.getTempCByIndex(0) ;
#endif
}


/* ------------------------------------------------------------------- */
#ifdef HAS_DHT22
double getHmdt()
{
  return dht.readHumidity() ;
}
#endif
