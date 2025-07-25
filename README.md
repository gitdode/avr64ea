# avr64ea

## About

Playground to get into the newer AVRs using the [AVR64EA28](https://www.microchip.com/en-us/product/AVR64EA28).  

Measuring temperature with a thermistor and the 12-bit ADC, transmitting it with 
an RFM radio module, powering off the thermistor, putting the radio module in 
sleep mode and going to power down sleep mode including disabling digital input 
buffer on all pins in between measurements & transmissions, resulting in idle 
power consumption of about 2 µA (4 µA with USB to serial bridge connected to PC).  
Using RTC's periodic interrupt timer to periodically wake up the controller.  
Measuring power supply (battery) voltage with a voltage divider and the internal 
2.048 voltage reference, skipping temperature measurement and radio transmission below 
2.100 Volt.  
Also using the event system for nothing useful at all, just to see how it works 👀

![IMG_20250717_232617](https://github.com/user-attachments/assets/50fb9e70-7732-4ab2-9bdc-80dddf8f4a9e)
