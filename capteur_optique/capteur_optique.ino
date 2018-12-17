

#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

// Ci-dessous les librairies permettant la communication avec l'ecran LCD via le bus I2C
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>


// On indique l'adresse du capteur et sa résolution
LiquidCrystal_I2C lcd(0x27, 20, 4);

// deux séquences permettant de stocker les valeurs lues des photorésistances
int seq_n[5];
int seq_n_plus_un[4];

// numéro des pins utilisé pour les LEDs
int pinLed = 13;
int pin_PRB = A0;
int pin_PRR = A1;

// cette variable permet de savoir lorsque quelquechose est affiché sur l'écran
// 0 signifie que rien n'est affiché, sinon elle vaut 1
int affiche = 0;

// variables permettant de stocker les valeurs lues des photorésistances
int ledBleu;
int ledRouge;

// variable permettant de savoir si on a déjà rempli une fois seq_n
// la valeur 0 permet de dire qu'on doit l'initialiser dans un premier temps
int seq_n_remplie = 0;


void setup() {
  Serial.begin(9600);

  // on initialise l'écran avec le retroéclairage
  lcd.init();
  lcd.backlight();
  lcd.print("Lecture en cours");
  
}

void loop() {

  // premier test pour savoir si rien n'est déjà affiché, et que seq_n n'est pas encore remplie
  if(affiche==0 && seq_n_remplie==0){
    // ici on lit la valeur de la photorésistance connectée à la LED bleue
    ledBleu = analogRead(A0);

    // on teste si la LED bleue est allumée
    if(ledBleu>200){
      delay(1000); //recentrage sur la sequence pour une meilleure lecture (voir rapport)
      int i;

      // boucle pour remplir seq_n avec la séquence d'erreur
      for(i=1;i<5;i++){
        delay(2000);
        //lecture de la valeur de la photorésistance connectee à la LED rouge
        ledRouge = analogRead(A1);

        // si LED rouge allumée on met 1 , sinon 0
        if(ledRouge > 200){
          seq_n[i]=1;
           }
            else{
            seq_n[i]=0;
           }
        }

        // initialisation terminée, seq_n est lue
        seq_n_remplie = 1;
        seq_n[5]=1;                               // A voir avec le prof , pas normal
      }
   }

   
   // si on a toujours rien d'affiche, et que seq_n est initialisee
   if(affiche==0 && seq_n_remplie==1){
      ledBleu = analogRead(A0);

      // tant que la LED bleue est éteinte on attend ici
      while(ledBleu<200){
        // cadencage de lecture rapide pour mieux controller la lecture
        delay(100);
        ledBleu = analogRead(A0);
      }

      // si la LED bleue est allumee, on lance le remplissage de seq_n_plus_un
      if(ledBleu>200){
        // on recentre la lecture au milieu des clignottements
        delay(1000);
        int i;

        //remplissage de seq_n_plus_un
        for(i=1;i<5;i++){
          delay(2000);
          ledRouge = analogRead(A1);
            if(ledRouge > 200){
              seq_n_plus_un[i]=1;               
            }
            else{
              seq_n_plus_un[i]=0;           
            }
        }

        // affichage des deux sequences lues
        Serial.println("seq_n");
        Serial.println(seq_n[1]);
        Serial.println(seq_n[2]);
        Serial.println(seq_n[3]);
        Serial.println(seq_n[4]);
        Serial.println("seq_n_plus_un");
        Serial.println(seq_n_plus_un[1]);
        Serial.println(seq_n_plus_un[2]);
        Serial.println(seq_n_plus_un[3]);
        Serial.println(seq_n_plus_un[4]);
      }

      // on verifie si les deux sequences sont egales
      if(seq_n[1] == seq_n_plus_un[1] && seq_n[2] == seq_n_plus_un[2] && seq_n[3] == seq_n_plus_un[3] && seq_n[4] == seq_n_plus_un[4]){

        // si c est le cas alors on affiche l erreur sur l ecran LCD , et on dit qu on a affiche
        AfficherPb();
        affiche = 1;
      }

      // sinon on affiche que la panne est inconnue et on copie seq_n_plus_un dans seq_un
      else{
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print(" panne inconnue");
        seq_n[1]=seq_n_plus_un[1] ;
        seq_n[2]=seq_n_plus_un[2] ;
        seq_n[3]=seq_n_plus_un[3] ;
        seq_n[4]=seq_n_plus_un[4] ;
      }
      
    }
    delay(100);//cadencage rapide pour lecture de la led bleu  
}


// fonction permettant d'associer une sequence a une erreur 
void AfficherPb(){
  Serial.println("2 sequences egales");
  lcd.clear();
  Serial.println("seq_n");
  Serial.println(seq_n[1]);
  Serial.println(seq_n[2]);
  Serial.println(seq_n[3]);
  Serial.println(seq_n[4]);
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  if(seq_n[1]==0 && seq_n[2]==0 && seq_n[3]==0 && seq_n[4]==0){
    lcd.print("Aucune erreur");
  }

  else if(seq_n[1]==1 && seq_n[2]==0 && seq_n[3]==1 && seq_n[4]==1){
    lcd.print("PB WIFI");
  }

  else if(seq_n[1]==0 && seq_n[2]==0 && seq_n[3]==0 && seq_n[4]==1){
    lcd.print("PB HUM");
  }

  
  else if(seq_n[1]==0 && seq_n[2]==0 && seq_n[3]==1 && seq_n[4]==0){
    lcd.print("PB LUM");
  }

  else if(seq_n[1]==0 && seq_n[2]==0 && seq_n[3]==1 && seq_n[4]==1){
    lcd.print("PB LUM HUM");
  }

  else if(seq_n[1]==0 && seq_n[2]==1 && seq_n[3]==0 && seq_n[4]==0){
    lcd.print("PB TEMP");
  }

  else if(seq_n[1]==0 && seq_n[2]==1 && seq_n[3]==0 && seq_n[4]==1){
    lcd.print("PB TEMP HUM");
  }

  else if(seq_n[1]==0 && seq_n[2]==1 && seq_n[3]==1 && seq_n[4]==0){
    lcd.print("PB TEMP LUM");
  }

  else if(seq_n[1]==0 && seq_n[2]==1 && seq_n[3]==1 && seq_n[4]==1){
    lcd.print("PB TEMP LUM  HUM ");
  }
  else if(seq_n[1]==1 && seq_n[2]==0 && seq_n[3]==1 && seq_n[4]==0){
    lcd.print("PB CONNECTION MQTT ");
  }
  else{
    lcd.print("Unkown error");
  }

  //on enregistre que quelquechose a ete affiche
  affiche = 1;
}
