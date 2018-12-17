/* This function handles the sending of all the values
 * collected by the sensors.
 * Values are sent only if there has been a significant
 * change since last sent.
 */

void sendValues() {
  if ( client.connected() ) {
    sendLum() ;
    sendTmp() ;
#ifdef HAS_DHT22
    sendHmdt() ;
#endif
  }

// Test des envois des valeurs de luminosite, temperature, humidite
  if (isnan(lum)){
    if (isnan(tmp)){
      if (isnan(hmdt)) {
        Serial.println("il y a une erreur sur LUM, TMP, HMDT"); 
        flashing(0,1,1,1);
      }
      else{
        Serial.println("il y a une erreur sur LUM, TMP");
        flashing(0,1,1,0);
      }
    }
    else if (isnan(hmdt)){
      Serial.println("il y a une erreur sur LUM, HMDT");
      flashing(0,0,1,1);
    }
    else {
      Serial.println("il y a une erreur sur LUM");
      flashing(0,0,1,0);
    }
  }
  else if (isnan(tmp)){
    if (isnan(hmdt)) {
      Serial.println("il y a une erreur sur TMP, HMDT");
      flashing(0,1,0,1);
    }
    else {
      Serial.println("il y a une erreur sur TMP");
      flashing(0,1,0,0);
    }
  }
  else if (isnan(hmdt)) {
    Serial.println("il y a une erreur sur HMDT");
    flashing(0,0,0,1);
  }
  else {
    Serial.println("il n'y aucune erreur ");
    flashing(0,0,0,0);
  }
  
    
}


void sendLum() {
  if ( force || (lum != -1 && abs(lum - prev_lum) >= DELTA_V_LUM) ) {
    client.publish(String(topic + "/" + ID[3] + "/" + CAPA_LUM).c_str(), String(lum).c_str()) ;    /*    topic , la valeur  */
    prev_lum = lum ;                                                                               /* dernière valeur publiée*/
    lum = -1 ;
    Serial.println("LUM envoyée");
  }
  Serial.println(lum);

}

void sendTmp() {
  if ( force || (tmp != -1 && abs(tmp - prev_tmp) >= DELTA_V_TMP) ) {
    client.publish(String(topic + "/" + ID[3] + "/" + CAPA_TEMP).c_str(), String(tmp).c_str()) ;
    prev_tmp = tmp ;
    tmp = -1 ;
    Serial.println("TMP envoyée");
  }
  Serial.println(tmp);
}

#ifdef HAS_DHT22
void sendHmdt() {
  if ( force || (hmdt != -1 && abs(hmdt - prev_hmdt) >= DELTA_V_HUM) ) {
    client.publish(String(topic + "/" + ID[3] + "/" + CAPA_HMDT).c_str(), String(hmdt).c_str()) ;
    prev_hmdt = hmdt ;
    hmdt = -1 ;
    Serial.println("HMDT envoyée");
  }
  Serial.println(hmdt);
}
#endif

void flashing(int bit1, int bit2, int bit3, int bit4){
  int sequence1[4]= {bit1,bit2,bit3,bit4};
    digitalWrite(BLUE_LED,0);
    Serial.println("bleue allumée 2 sec");
    delay(2000);
    digitalWrite(BLUE_LED,1);
    Serial.println("bleue éteinte 2 sec");//     1 = éteint    ,    0 = allumé
    delay(2000);
    for(int i = 0;i<4;i++){
      digitalWrite(RED_LED,sequence1[i]);
      delay(2000);
    }
    digitalWrite(BLUE_LED,1);
    digitalWrite(RED_LED,1);
}
