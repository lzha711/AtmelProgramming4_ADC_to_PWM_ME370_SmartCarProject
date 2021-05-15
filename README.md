# AtmelProgramming_ADC_to_PWM

A useful piece of code on ADC and PWM settings in ATmega328PB. Tested on the ATmega328PB Xplained Mini. 

Three channels of ADC (ADC0-2) used. Each one reads the analog input from a potentiometer, the value is then used to determine the duty cycle of a PWM output. 
The pin connections are: 

  PC0(ADC0) - PB1(OC1A), PC1(ADC1) - PB2(OC1B), PC2(ADC2) - PD6(OC0A)
  
This is a practice derived from the ME370 smart car project, where two ADC channels are used to read sensor (phototransistor) values and output PWM to control motor speed. 

Last updated: 15 May 2021. 

Circuit diagram is to be uploaded soon.
