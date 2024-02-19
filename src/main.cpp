#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800
#include <TinyGSM.h>
#include "TinyGsmCommon.h"

#define LED_PIN 13

#define SerialMon Serial
#define SerialAT Serial1

TinyGsm modem(SerialAT);

void blink_led();

void setup()
{
  SerialMon.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  SerialMon.println("[+] GSM Relay Station has started...");

  int min_baud_rate = 9600;
  int max_baud_rate = 115200;
  TinyGsmAutoBaud(SerialAT, min_baud_rate, max_baud_rate);
  blink_led();
  SerialMon.println("[+] GSM Relay Station: Set Auto Baud Rate");

  delay(5000);

  modem.init();
  // modem.restart();
  blink_led();
  SerialMon.println("[+] GSM Relay Station: Init Modem");

  delay(5000);

  SerialMon.print("[+] GSM Relay Station: Modem Info");
  SerialMon.println(modem.getModemInfo());

  SerialMon.println("[+] GSM Relay Station: Sim Status");
  SerialMon.println(modem.getSimStatus());
}

void loop()
{
  if (!modem.isNetworkConnected())
  {
    SerialMon.println("[+] GSM Relay Station: Network disconnected");
    if (!modem.waitForNetwork(180000L, true))
    {
      SerialMon.println("[+] GSM Relay Station: Network re-connect failed");
      delay(10000);
      return;
    }

    if (modem.isNetworkConnected())
    {
      SerialMon.println("[+] GSM Relay Station: Network re-connected");
    }
  }
}

void blink_led() {
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
}
