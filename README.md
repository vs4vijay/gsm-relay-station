# gsm-relay-station

## Device

SIM 800L / 800H
- LILYGO T-Call SIM800L
- Datasheet - https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/blob/master/doc/SIM800L_IP5306.MD

---

## References

https://github.com/vshymanskyy/TinyGSM
https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800


---

## Development Notes

```bash



// Restart takes quite some time
// To skip it, call init() instead of restart()
SerialMon.println("Initializing modem...");
modem.restart();


delay(10000);

String imei = modem.getIMEI();
DBG("IMEI:", imei);

bool  res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
DBG("SMS:", res ? "OK" : "fail");

// Do nothing forevermore
while (true) {
    modem.maintain();
}


```