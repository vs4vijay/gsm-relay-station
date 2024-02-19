#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800
#include <TinyGSM.h>
#include <TinyGsmClient.h>
#include <TinyGsmCommon.h>

#define LED_PIN 13

#define SerialMon Serial
#define SerialAT Serial1
// #define SerialAT Serial2

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

void print_modem_info();
void check_and_send_sms();
void blink_led();
void inject_REPL();

int sms_counter = 0;
uint32_t rate = 0; // Module baud rate, Set to 0 for Auto-Detect

void setup()
{
  SerialMon.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  int res;

  delay(1000);
  SerialMon.println("[+] GSM Relay Station has started...");

  DBG("[+] Setting up the GSM Relay Station...")

  // int min_baud_rate = 9600;
  // int max_baud_rate = 115200;
  // TinyGsmAutoBaud(SerialAT, min_baud_rate, max_baud_rate);
  // blink_led();
  // SerialMon.println("[+] GSM Relay Station: Set Auto Baud Rate");

  rate = TinyGsmAutoBaud(SerialAT);
  SerialMon.println("[+] GSM Relay Station: Got Rate - " + String(rate));

  delay(5000);

  // modem.init();
  modem.restart();
  blink_led();
  SerialMon.println("[+] GSM Relay Station: Init Modem");

  delay(5000);

  print_modem_info();

  // bool res = modem.isGprsConnected();
  // SerialMon.println("GPRS status:", res ? "connected" : "not connected");

  res = modem.testAT();
  SerialMon.println("[+] GSM Relay Station: SIM800 Test AT Result - " + String(res));
}

void loop()
{
  if (!modem.isNetworkConnected())
  {
    SerialMon.println("[+] GSM Relay Station: Network disconnected");
    if (!modem.waitForNetwork(1 * 10 * 1000, true))
    {
      SerialMon.println("[+] GSM Relay Station: Network re-connect failed");
      delay(10000);
      // return;
    }

    if (modem.isNetworkConnected())
    {
      SerialMon.println("[+] GSM Relay Station: Network re-connected");
    }
  }

  delay(1000);

  print_modem_info();

  check_and_send_sms();

  delay(1000);

  // modem.maintain();
}

void print_modem_info()
{
  int csq = modem.getSignalQuality();
  SerialMon.println("[+] GSM Relay Station: Signal quality - " + String(csq));
  SerialMon.println("[+] GSM Relay Station: Modem Name - " + modem.getModemName());
  SerialMon.println("[+] GSM Relay Station: Modem Info - " + modem.getModemInfo());
  SerialMon.println("[+] GSM Relay Station: Sim Status - " + String(modem.getSimStatus()));
  SerialMon.println("[+] GSM Relay Station: GPRS Status - " + String(modem.isGprsConnected()));
  SerialMon.println(modem.isGprsConnected());
  SerialMon.println("[+] GSM Relay Station: IMEI - " + modem.getIMEI());
  SerialMon.println("[+] GSM Relay Station: IMSI - " + modem.getIMSI());
  SerialMon.println("[+] GSM Relay Station: Operator - " + modem.getOperator());
  SerialMon.println("[+] GSM Relay Station: Local IP - " + modem.getLocalIP());
  SerialMon.println("[+] GSM Relay Station: GSM Location - " + modem.getGsmLocationRaw());

  // int modem.getSimCCID();

  SerialMon.println("---");
}

void check_and_send_sms()
{
  if (sms_counter < 1)
  {
    String target_number = "+919166276700";
    String message = "Hello from GSM Relay Station";

    SerialMon.println("[+] GSM Relay Station: Sending SMS to " + target_number);

    bool status = modem.sendSMS(target_number, message);

    if (status)
    {
      SerialMon.println("[+] GSM Relay Station: SMS sent successfully");
      sms_counter++;
    }
    else
    {
      SerialMon.println("[+] GSM Relay Station: SMS sending failed");
    }
  }
}

void blink_led()
{
  digitalWrite(LED_PIN, LOW);
  // modem.sendAT("+CNETLIGHT=0");

  delay(1000);

  digitalWrite(LED_PIN, HIGH);
  // modem.sendAT("+CNETLIGHT=1");

  delay(1000);

  digitalWrite(LED_PIN, LOW);
}

void inject_REPL()
{
  SerialAT.begin(rate);

  // Access AT commands from Serial Monitor
  SerialMon.println(F(" You can now send AT commands"));

  while(true) {
    if (SerialAT.available()) {
      SerialMon.write(SerialAT.read());
    }
    if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
    delay(0);
  }
}