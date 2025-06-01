# avr64ea

## About

Playground to get into the newer AVRs using the [AVR64EA28](https://www.microchip.com/en-us/product/AVR64EA28).  

Measuring temperature with a thermistor and the 12-bit ADC, powering off the 
thermistor and going to power down sleep mode in between measurements, resulting 
in idle power consumption of about 1.5 µA including ADC and USART.  
Using RTC's periodic interrupt timer to periodically wake up the controller.