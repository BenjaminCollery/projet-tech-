#include <string.h>

char *date = "YYYY-MM-DD HH:MM:SS" ;
uint32_t t0_cmd, t_cmd ;
uint32_t t0_sync, t_sync ;

/* This function handles the connection/reconnection to
 * the MQTT broker.
 * Each time a connection or reconnection is made, it
 * publishes a "HELLO" message containing the firmware 
 * version of the module.
 * 
 * Once connected, we subscribe to the "<topic>/request" topic to
 * be able to receive commands.
 * The answers to these commands, if any, will be sent over the
 * "<topic>/answer" topic.
 */
boolean reconnect() {                                             // essaie de se connecter
  Serial.print("MQTT: trying to connect to host ") ;
  Serial.print(host) ;
  Serial.print(" on port ") ;
  Serial.print(port) ;
  Serial.println(" ...") ;

  if ( client.connect(ID[0]) ) {
    digitalWrite(RED_LED, HIGH) ;                                    // is on se co, on éteind la LED

    Serial.println("MQTT: connected") ;
    Serial.print("MQTT: root topic is \"") ;
    Serial.print(topic) ;
    Serial.println("\"") ;
    
    client.publish(String(topic + "/HELL").c_str(), FIRMWARE_VERSION) ;

    client.subscribe(String(topic + "/request").c_str()) ;
    Serial.println(String("MQTT: Subscribed to topic " + topic + "/request").c_str()) ;
    client.subscribe("date/time") ;
    Serial.println("MQTT: Subscribed to topic date/time") ;
 }
  
  return client.connected() ;
}

#define min(a,b) ((a)<(b)?(a):(b))

/* The main callback to listen to topics.                                appelée quand qqchose se passe qqpart (évenement en symfony) */

void callback(const char* intopic, byte* payload, unsigned int length)   /* arguments obligatoires */
{
  String cmd, answer ;
  int lm ;

  t_sync = millis() ;
  if ( ! strncmp("date/time", intopic, 9) ) {
    // time to get in sync with the world!
    t0_sync = t_sync ;
    lm = length ;
    lm = (int)min(lm, 19) ;
    strncpy(date, (char *)payload, lm) ;
    Serial.print("date/time: ") ;
    Serial.print(date) ;
    Serial.print(" = ") ;
    Serial.println(t_sync) ;
  } else {
    t_cmd = t_sync ;
    if ( t_cmd - t0_cmd >= DELTA_T_CMD ) {
      t0_cmd = t_cmd ;
      
      parse_cmd(answer, intopic, payload, length, cmd) ;
      
      client.publish(String(topic + "/answer/" + cmd).c_str(), answer.c_str()) ;
    }
  }
}

/* This function parses commands received by the module on the topics it's listening on,
 *  then send answers accordingly.
 */
void parse_cmd( String &answer, const char* intopic,
                byte* payload, unsigned int length,
        				String &cmd)
{
  char *cmd_arg ;
  int i ;

  // Let's assume it's an unknown command (no offense!)
  cmd = "" ;
  for ( i=0 ; i<length ; i++ ) {
    cmd += (char)payload[i] ;
  }
  answer = "unknown command" ;

  if ( length < CMD_LENGTH ) {
    return ;
  }

  cmd = cmd.substring(0,4) ;
  if ( ! strncmp("STAT", (char *)payload, CMD_LENGTH) ) {
    // return status (so, always OK if i'm able to answer...)
    answer = "OK" ;
    
  } else if ( ! strncmp("DESC", (char *)payload, CMD_LENGTH) ) {
    // tell what I'm able to do
    answer = CAPA_LUM ;
    answer += " " ;
    answer += CAPA_TEMP ;
#ifdef HAS_DHT22
    answer += " " ;
    answer += CAPA_HMDT ;
#endif  

  } else if ( ! strncmp("HELP", (char *)payload, CMD_LENGTH) ) {
    // send available commands
    answer = "STAT DESC GET: GETA HELP" ;
    
  } else if ( ! strncmp("GET:", (char *)payload, CMD_LENGTH) ) {
    // return a specific value.
    // parameter must be one of what DESC returns
    if ( length < 2*CMD_LENGTH ) {
      answer = "missing arg" ;
    } else {
      cmd_arg = ((char *)payload) + CMD_LENGTH ;
      if ( ! strncmp(CAPA_LUM, cmd_arg, CMD_LENGTH) ) {
        lum = getLum() ;
        answer  = CAPA_LUM ;
        answer += "=" ;
        answer += lum ; ;

        force = true ;
        sendLum() ;
        force = false ;
        
      } else if ( ! strncmp(CAPA_TEMP, cmd_arg, CMD_LENGTH) ) {
        tmp = getTmp() ;
        answer  = CAPA_TEMP ;
        answer += "=" ;
        answer += tmp ;

        force = true ;
        sendTmp() ;
        force = false ;
        
#ifdef HAS_DHT22
      } else if ( ! strncmp(CAPA_HMDT, cmd_arg, CMD_LENGTH) ) {
        hmdt = getHmdt() ;
        answer  = CAPA_HMDT ;
        answer += "=" ;
        answer += hmdt ;

        force = true ;
        sendHmdt() ;
        force = false ;
#endif
      }
    }
    
  } else if ( ! strncmp("GETA", (char *)payload, CMD_LENGTH) ) {
    // get all values
    lum = getLum() ;
    tmp = getTmp() ;
#ifdef HAS_DHT22
    hmdt = getHmdt() ;
#endif
    answer  = CAPA_LUM ;
    answer += "=" ;
    answer += lum ;
    answer += " " ;
    answer += CAPA_TEMP ;
    answer += "=" ;
    answer += tmp ;
#ifdef HAS_DHT22
    answer += " " ;
    answer += CAPA_HMDT ;
    answer += "=" ;
    answer += hmdt ;
#endif

    force = true ;
    sendValues() ;
    force = false ;
  }
}
