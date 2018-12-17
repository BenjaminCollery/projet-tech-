#ifndef _USER_SETTINGS_H_
#define _USER_SETTINGS_H_


/* Common topic for all messages sent by this module.
 */
#define ROOT_TOPIC "emse/fayol/"

/* Does the module send data on a regular basis or only when things change?
 */
#define HEARTBEAT 1


#ifndef INTO_THE_WILD // TESTING CONDITIONS   // si pas en mode production
/* Each module must have a different ID (see list at the end of file)
 */
char *ID[] = {
  "24a89ddc-23c8-4d9f-9f5e-cff4eba32fb5", // This in not a valid UID                                                 // ici on met l' ID du module
  "ME",     // The module is located in a specific room
  "sensors",  // The module has a type
  "metrics"   // The module provides some type of data
} ;

const char* ssid     = "RaspberryBenji";
const char* password = "mines";

const char* host = "10.0.0.42" ;                                                                                     //    192.168.1.2 à quoi correspond cette IP ?
const uint16_t port =  1883 ;


#else // INTO THE WILD!
/* Each module must have a different ID (see list at the end of file)
 */
char *ID[] = {
  "24a89ddc-23c8-4d9f-9f5e-cff4eba32fb5",
  "S432",     // The module is located in a specific room
  "sensors",  // The module has a type
  "metrics"   // The module provides some type of data
} ;

/* Wi-Fi Network to connect to and WEP key.
 * The module can only connect to OPEN ou WEPed Wi-Fi networks.
 */
const char* ssid     = "RaspberryBenji";                                       // modifier avec le SSID de mon routeur ( RPI )
const char* password = "mines";                     // le mot de passe

/* Server IP address and port number
 */
const char* host = "10.0.0.42" ;                                      //   server sur lequel tourne le broker MQTT 
const uint16_t port =  1883 ;

#endif

/* IDs list (already in use and to be used).
  # Already in use
  7b051d3a-8547-463d-9d28-a2d50c5098b4;T,L;Chez Nicolas et Oudom - bureau 421
  6bd134b6-339c-4168-9aeb-ae7d0f236851;T,L;Hall d'entrée du 4eme
  8aa60f58-fc6f-49e2-a53a-f5cc96bb9021;T,L;Chez Philippe Jaillon - bureau 423
  03f5ca58-aa70-47b3-980c-c8f486cac9ee;T,H,L;Toilettes hommes 4eme sud
  140ade6c-4418-4d86-a14e-25b7db5ae83b;T,H,L;Salle 432
  24a89ddc-23c8-4d9f-9f5e-cff4eba32fb5;T,H;Bureau 424
  f9538ac8-4fdb-4049-9ff6-ac4855e3bcc5;T,H,L;Toilettes femmes 4eme sud
  70345659-3f50-49af-98e7-bbc93961df92;T,H,L;Bureau 425

  d1d7e6d4-db84-4a7c-a5e0-5bbddc9f130b;T,H,L;Mobile1
  8c1a3d81-82bc-40d7-9b77-1b2e5abe151c;T,H,L;Mobile2
  eefcdb28-7887-469a-a395-7b866cac3a81;T,H,L;Mobile3
  3bfad906-c282-4bf3-9227-cd28259d6b9d;T,H,L;Mobile4
  8a3f92cf-aebf-43b5-9c9e-7f1862584a6c;T,H,L;Mobile5

  # Not in use yet

  28bb16da-5d54-4882-9c2b-70c746586185;;
  757e0b46-0efe-4f36-bf2c-e8008e49d950;;
  88cb0522-478a-456c-b63b-9c402b5e03b2;;
  cdeff900-05af-4758-8fe8-15505c7765d4;;
  77b4e6bb-b11c-4c56-b95d-eb20283dc8ff;;
  987798b7-0235-42d6-a56a-59d19faec68b;;
  70eb56de-cd83-4218-8933-a1cc622a77d5;;
  c4af67a2-ffa7-4376-ad81-b9662aa30491;;
  9f420707-0526-4d6e-a613-dd49ec742da2;;
  fc5d768d-541f-4434-a6cf-7da2830db427;;
  ff149e7b-b727-42b0-9af3-2c41d2b540bf;;

 */

#endif
