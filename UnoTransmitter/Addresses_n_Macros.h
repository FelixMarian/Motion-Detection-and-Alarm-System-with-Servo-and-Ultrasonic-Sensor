#ifndef ADRESSES.H
#define ADRESSES.H

// Addresses
#define SERVO_STEP_ADR 0x00
#define SERVO_TIME_ADR 0x04


// Macros
#define write_pin_LOW_pD(pin) PORTD &= ~(1 << pin); 
#define write_pin_HIGH_pD(pin) PORTD |= (1<<pin);
#define set_pin_OUTPUT_pD(pin) DDRD |= (1 << pin);
#define set_pin_INPUT_pD(pin) DDRD &= ~(1 << pin);
#define set_pin_INPUT_pB(pin) DDRB &= ~(1 << (pin-8));
#define check_pin_INPUT_state_pB(pin) PINB & (1 << (pin-8));

#endif