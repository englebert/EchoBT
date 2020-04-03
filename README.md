Objective:

TX (nRF24L01) -----> RX (nRF24L01) -----> ESP32 ------> Emulates BT GameController -----> Android Phone


Due to worldwide lock down and I need to practice my FPV skills, I had created this to help me relay the signal
from my custom TX to my android. Basically I am using ESP32 BLE code for this whole thing to be able to work.

1. Add https://github.com/lemmingDev/ESP32-BLE-Gamepad to your Arduino Environment.
2. Download the code and upload to the ESP32 and link up with your NRF24L01 module.

The Wiring Diagram as below:


ESP32                   NRF24L01
-----                   --------
MOSI (GPIO23)           MOSI
SDA  (GPIO21)           CSN
MISO (GPIO19)           MISO
SCK  (GPIO18)           SCK
CE   (GPIO5 )           CE

Enjoy the code.
