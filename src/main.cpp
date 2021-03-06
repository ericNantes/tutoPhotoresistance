#include <Arduino.h>

#define PIN_LED 3

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  const uint16_t SEUIL_NUIT = 400;
  const uint16_t SEUIL_JOUR = 800;
  const uint32_t JOUR_MIN = 5000;   // Durée minimale en millisecondes de détection du jour avant extinction
  const uint32_t NUIT_MIN = 1000;   // Durée minimale de détection de la nuit avant allumage
  static uint32_t heureJour = 0;    // Heure de la dernière détection de jour
  static uint32_t heureNuit = 0;    // Heure de la détection du début de la nuit

  enum tEtats {JOUR, DEBUT_NUIT, NUIT, DEBUT_JOUR};
  static tEtats etat = JOUR;

  uint16_t luminosite = analogRead(A0);
  
  switch(etat) {
    case JOUR:
      if (luminosite <= SEUIL_NUIT) {
        etat = DEBUT_NUIT;
        heureNuit = millis();
      }
      break;
    case DEBUT_NUIT:
      if (luminosite >= SEUIL_JOUR) etat = JOUR;
      if ((millis() - heureNuit) >= NUIT_MIN) {
        etat = NUIT;
        digitalWrite(PIN_LED, HIGH);
      }
    break;
    case NUIT:
      if (luminosite >= SEUIL_JOUR) {
        heureJour = millis();
        etat = DEBUT_JOUR;
      }
    break;
    case DEBUT_JOUR:
      if ((millis() - heureJour) >= JOUR_MIN) {
        digitalWrite(PIN_LED, LOW);
        etat = JOUR;
      }
      if (luminosite < SEUIL_JOUR) etat = NUIT;
    break;
  }
}