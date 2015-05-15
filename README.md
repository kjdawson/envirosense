# envirosense

Internet of Things (IoT) Communication
Cold Basement Project

INTRODUCTION 

Using the NRF24L01 a RF transciever my goal was to send data between two nodes.
One of the problems in the winter is that the basement becomes extremely cold,
to figure out how much colder the basement is than the rest of the house I used
an Arduino Nano ATMEGA328 with a DHT22 temperature and humidity sensor to find 
out the difference in temperature between various rooms in my home.

COMMUNICATION METHOD 

One of the great things about NRF24L01+ is that firstly it works on a 2.4Ghz
free to transmit frequency. Another unique thing about NRF24L01 is that in the 
IoT space distance is a big problem. Many of the current technologies like
Bluetooth Low Energy (BLE) or Wifi are either expensive, have short range, or
consume too much power. The NRF chip is designed to have a large range up to
1 mile (1.6KM) and is ultra low power. Which makes ideal for IoT applications.

SENSOR + HARDWARE

For this project I used an Arduino Nano with the ATMEGA328 chip.
This is a low cost, low power and small footprint development module.
A traditional arduino UNO is much to large to ever have any real
applications in the IoT space. The sensor used is the DHT22
which has a high level of accuracy +-0.5 degrees but also at the
same time is low cost. 


