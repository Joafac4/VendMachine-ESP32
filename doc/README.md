![Austral Ingenieria](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcQooGo7vQn4t9-6Bt46qZF-UY4_QFpYOeh7kVWzwpr_lbLr5wka)

#   Project 03-pwm

##  Using a potentiometer connected to the A/D converter, changes LED brightness (PWM)

    Electronica Digital - Austral - 2023 - EAM
    Uses ESP32 microcontroller

###  Hardware

    1K to 10K potetiometer
    Light emitting diode LED
    220 ohm resistor
    cables

###  Connections

    Connect potentiometer ends between GND and 3.3 volt
    Connect potentiometer cursor to GPIO "ANAIN" (see platformio.ini)

    Connect LED anode to GPIO "LED1"
    Connect LED cathode to one end of 220 ohm
    Connect other end of resistor 220 ohm to GND

###  Verification

    As you change potentiometer form one end to the other, serial monitor
    shows converted value and LED change brightness

###  Links

[ESP32 PWM Example: How To Use PWM On An ESP32 Microcontroller](https://www.kompulsa.com/esp32-pwm-example-how-to-use-pwm-on-an-esp32-microcontroller/)

[Electronics Hub - In-depth ESP32 PWM Tutorial | How to use PWM in ESP32?](https://www.electronicshub.org/esp32-pwm-tutorial/)


