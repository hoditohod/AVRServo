/*
 * AVRServo
 *
 * Created: 2016.11.30. 11:31:07
 * Author : Gyorgy Szekely
 */ 

#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <util/delay.h>


uint16_t getADC(uint8_t channel)
{
    /* ADC clock div: 1MHz / 8 = 125kHz
     * PB3 mux: ADC channel3
     * PB4 mux: ADC channel2
     */
    
    ADMUX =  channel;                                                       // Reference: Vcc
    DIDR0 = _BV(ADC2D) | _BV(ADC3D);                                        // Digital logic disabled on PB3, PB4
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | _BV(ADPS1) | _BV(ADPS0);   // Enable and start, clkdiv: 8
    
    loop_until_bit_is_clear(ADCSRA, ADSC);
    
    return ADC;
}

int main(void)
{
    // PB0 output (PWM signal)
    DDRB = _BV(PB0);
    
    // Left/Right control: PB1, PB2 
    // Enable pull-up resistors
    PORTB |= _BV(PB1) | _BV(PB2);
    
    while (1) 
    {
        // 1 left, 2 right, 0 or 3 center
        volatile uint8_t direction = (PINB >> 1) & 0x03;
        
        // pulseWidth (~us resolution) 600us - 2400us
        volatile uint16_t pulseWidth = getADC(direction==1 ? 2 : 3)*18/10 + 600; //ADC channels 2&3 are in use
        
        if (direction == 3 || direction == 0)
            pulseWidth = 1500; //center
        
        // start pulse
        PORTB |= _BV(PB0);
        _delay_loop_2(pulseWidth>>2);   // one iteration is ~4us, div pulseWidth by 4
        
        // finish pulse and wait for 20ms cycle time
        PORTB &= ~_BV(PB0);
        _delay_loop_2((20200-pulseWidth)>>2); // clock is not calibrated anyway...
    }
}

