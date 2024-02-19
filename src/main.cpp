#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800
#include <TinyGSM.h>
#include <TinyGsmClient.h>
#include <TinyGsmCommon.h>

#define LED_PIN 13

#define SerialMon Serial
#define SerialAT Serial1

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

void blink_led();

void setup()
{
  SerialMon.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  int res;

  SerialMon.println("[+] GSM Relay Station has started...");

  DBG("[+] Setting up the GSM Relay Station...")

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

  SerialMon.println("[+] GSM Relay Station: Modem Name - " + modem.getModemName());

  SerialMon.print("[+] GSM Relay Station: Modem Info");
  SerialMon.println(modem.getModemInfo());

  SerialMon.println("[+] GSM Relay Station: Sim Status");
  // int modem.getSimCCID();

  // bool res = modem.isGprsConnected();
  // SerialMon.println("GPRS status:", res ? "connected" : "not connected");

  const char *imei = modem.getIMEI().c_str();
  SerialMon.print("IMEI: ");
  SerialMon.println(imei);

  String imsi = modem.getIMSI();
  SerialMon.print("IMSI: ");
  SerialMon.println(imsi);

  // String cop = modem.getOperator();
  // SerialMon.println("Operator:", cop);

  // IPAddress local = modem.localIP();
  // SerialMon.println("Local IP:", String(local));

  // int csq = modem.getSignalQuality();
  // SerialMon.println("Signal quality:", String(csq));

  res = modem.testAT();
  SerialMon.print("SIM800 Test AT result -> ");
  SerialMon.println(res);
}

void loop()
{
  if (!modem.isNetworkConnected())
  {
    SerialMon.println("[+] GSM Relay Station: Network disconnected");
    if (!modem.waitForNetwork(1 * 60 * 1000, true))
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

  // modem.maintain();
}

void blink_led()
{
  digitalWrite(LED_PIN, LOW);
  modem.sendAT("+CNETLIGHT=0");
  delay(1000);
  digitalWrite(LED_PIN, HIGH);
  modem.sendAT("+CNETLIGHT=1");
  delay(1000);
}
