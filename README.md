# gsm-relay-station

## Device

SIM 800L / 800H
- LILYGO T-Call SIM800L
- Datasheet - https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/blob/master/doc/SIM800L_IP5306.MD


## Setup Development Environment

```bash

pip install -U platformio

```

## Compile Firmware


```shell

pio run

```

- firmware will be at `.pio/build/*` directory

## Flash Firmware


### Option-I: Using esptool
- Install esptool.py - `pip install -U esptool`
```
python -m esptool write_flash -z 0 firmware.bin
```

---

## References

- https://github.com/vshymanskyy/TinyGSM
- https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800


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



res = modem.callNumber(CALL_TARGET);
DBG("Call Status: ", res ? "OK" : "failed");

res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
DBG("Send SMS Status: ", res ? "OK" : "failed");

modem.sendUSSD("*111#");
modem.dtmfSend('A', 1000);

String location = modem.getGsmLocation();
DBG("GSM Based Location String:", location);

String gps_raw = modem.getGPSraw();
DBG("GPS/GNSS Based Location String:", gps_raw);


DBG("Chip temperature:", modem.getTemperature());

modem.maintain();

modem.poweroff();

```