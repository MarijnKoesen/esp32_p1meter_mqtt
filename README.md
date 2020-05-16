# ESP8266 P1 Meter to MQTT

[![Build Status](https://travis-ci.com/MarijnKoesen/esp8266_p1meter_mqtt.svg?branch=master)](https://travis-ci.org/MarijnKoesen/esp8266_p1meter_mqtt)

Software for the ESP2866 that sends P1 smart meter data to an MQTT broker using Auto Discovery (with OTA firmware updates)


## Connection of the P1 meter to the ESP8266

As the Serial data is inverted you can't simply hook up the P1 meter to your ESP8266.

I know there is the SoftwareSerial library that can actually do the inversion with code, but in my experience it's unstable and it didn't
give me a stable enough result that was useable for me.

So I use a small hardware invertor so I can hookup the P1 meter directly to the hardware serial connection of the ESP8266, this has the downside that you can't debug using `Serial.print` anymore, but the benefit is that it's very stable. When it's stable you don't need to debug. See below for development instructions.


### Hook up the P1 meter 

As there is not a good RJ11 jack in Fritzing it's a bit hard to see the exact pinout in the breadboard schematic below, so this shows the correct pins for the RJ11 plug used below.

| ESP8266 Pin | P1 Pin |
| ----        | ----   |
| GND         | GND    |
| 5V          | RTS    |
| DATA (RXD)  | D5     |

Use a 10K resistor in between between DATA (RXD) and RTS.

![RJ11 P1 connetor](http://gejanssen.com/howto/Slimme-meter-uitlezen/RJ11-pinout.png)

### Connecting the ESP8266

![Connection diagram](https://github.com/MarijnKoesen/esp8266_p1meter_mqtt/raw/master/doc/P1_Meter_Schematic.png)


Note to developers: if you want to do debugging, you can hook up any board with multiple Serial ports, like an ESP32 or an Arduino Mega, and use that to debug. I just happened
to only have some ESP8266 laying around, so that's what I used for this project.



## Installation

1. Install platform.io
2. Run `make upload`


## Data Sent

All metrics are send to their own MQTT topic.

By default the ESP8266 sends out all data received by the P1 meter, but this is modifieable in the settings.h.

```cpp
/**
 * Define the data we're interested in, as well as the datastructure to
 * hold the parsed data. This list shows all supported fields, remove
 * any fields you are not using from the below list to make the parsing
 * faster, and the data in mqtt smaller. 
 * Each template argument below results in a field of the same name.
 */
using MyData = ParsedData<
    /* String */ identification,
    /* String */ p1_version,
    /* String */ timestamp,
    /* String */ equipment_id,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2,
    /* FixedValue */ energy_returned_tariff1,
    /* FixedValue */ energy_returned_tariff2,
    /* String */ electricity_tariff,
    /* FixedValue */ power_delivered,
    /* FixedValue */ power_returned,
    /* FixedValue */ electricity_threshold,
    /* uint8_t */ electricity_switch_position,
    /* uint32_t */ electricity_failures,
    /* uint32_t */ electricity_long_failures,
    /* String */ electricity_failure_log,

    /* uint32_t */ electricity_sags_l1,
    /* uint32_t */ electricity_sags_l2,
    /* uint32_t */ electricity_sags_l3,
    /* uint32_t */ electricity_swells_l1,
    /* uint32_t */ electricity_swells_l2,
    /* uint32_t */ electricity_swells_l3,
    /* String */ message_short,
    /* String */ message_long,

    /* FixedValue */ voltage_l1,
    /* FixedValue */ voltage_l2,
    /* FixedValue */ voltage_l3,

    /* FixedValue */ current_l1,
    /* FixedValue */ current_l2,
    /* FixedValue */ current_l3,

    /* FixedValue */ power_delivered_l1,
    /* FixedValue */ power_delivered_l2,
    /* FixedValue */ power_delivered_l3,
    /* FixedValue */ power_returned_l1,
    /* FixedValue */ power_returned_l2,
    /* FixedValue */ power_returned_l3,

    /* uint16_t */ gas_device_type,
    /* String */ gas_equipment_id,
    /* uint8_t */ gas_valve_position,
    /* TimestampedFixedValue */ gas_delivered,

    /* uint16_t */ thermal_device_type,
    /* String */ thermal_equipment_id,
    /* uint8_t */ thermal_valve_position,
    /* TimestampedFixedValue */ thermal_delivered,

    /* uint16_t */ water_device_type,
    /* String */ water_equipment_id,
    /* uint8_t */ water_valve_position,
    /* TimestampedFixedValue */ water_delivered,

    /* uint16_t */ slave_device_type,
    /* String */ slave_equipment_id,
    /* uint8_t */ slave_valve_position,
    /* TimestampedFixedValue */ slave_delivered
>;
```


## Home Assistant Configuration

Just enable MQTT AutoDiscovery and set the correct topic in the settings.h and all sensors will be automatically added.


## Thanks to

Standing on the heads of giants, big thanks and great respect to the writers and/or creators of the projects below that inspired this project:

- https://github.com/matthijskooijman/arduino-dsmr
- http://domoticx.com/p1-poort-slimme-meter-uitlezen-hardware/
- https://github.com/WhoSayIn/esp8266_dsmr2mqtt
- https://github.com/fliphess/esp8266_p1meter
- https://github.com/jantenhove/P1-Meter-ESP8266
- https://github.com/neographikal/P1-Meter-ESP8266-MQTT
- http://gejanssen.com/howto/Slimme-meter-uitlezen/
- https://github.com/rroethof/p1reader/
- http://romix.macuser.nl/software.html
- http://blog.regout.info/category/slimmeter/

