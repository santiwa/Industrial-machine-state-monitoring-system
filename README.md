Arduino IDE combined with the Ethernet module acts as the SERVER, and ThingSpeak is considered the CLIENT.
The analog values from Arduino board is shared to ThingSpeak Cloud.
Following sensors are used as an example: LM35 temperature at A0, LDR at A1, 2 Potentiometers at A2 and A3, respectively.
Changes in the sensor readings can be observed on Serial monitor and ThingSpeak cloud.
Data is updated in the interval of 10 seconds.
