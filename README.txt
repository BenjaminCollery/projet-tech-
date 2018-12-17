### CODE ARDUINO UNO

Le fichier capteur_optique.ino est le code qui est téléversé actuellement sur notre Uno.




### CODE ESP8266

# Capteurs_ESP8266_MQTT.ino
Le fichier Capteurs_ESP8266_MQTT.ino a été modifié pour déclencher des séquences de clignotement des LEDs lorsqu'il rencontre une erreur.

# sendValues.ino
Le fichier sendValues.ino a été modifié. Une fonction fashing reçoit en paramètre 4 valeurs 0 ou 1 et lance une fois le clignottement.
Dans ce fichier on reconnait toutes les erreurs qui concernent les capteurs. C'est à dire lorsque les capteurs envoient NaN.

# user_settings.h
Le fichier user_settings.h a été modifié de manière à ce que notre arduino se connecte au wifi que nous avons créé gräce à la Raspberry PI.

# sensors_settings.h
Le fichier sensors_settings.h a été modifié de manière à ce que des mesures soient publiées plus fréquement sur le broker MQTT. Nous avons pour celà baissé considérablement les seuils delta.

# pins.h
le fichier pins.h a été adapté afin de pouvoir utiliser des LEDs ajoutées et non pas les LEDs intégrées à l'ESP8266.


