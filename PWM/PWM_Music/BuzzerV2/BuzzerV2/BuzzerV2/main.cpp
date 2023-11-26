#define F_CPU 7372800

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h> 
#include <stdint.h>
#include "pcm_sample.h"

volatile uint16_t sample;
int sample_count;

/* initialise the PWM */
void pwm_init(void)
{
    /* use OC1A pin as output */
    DDRD = 0xff;

    /*
    * clear OC1A on compare match				(1 << COM1A1)
    * set OC1A at BOTTOM, non-inverting mode
    * Fast PWM, 8bit							(1 << WGM10)
    */
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
   
    /*
    * Fast PWM, 8bit										(1 << WGM12)
    * Prescaler: clk/1 = 7372800 Hz							(1 << CS10) - no prescaler
    * PWM frequency = 7372800 Hz / (255 + 1) = 28800 Hz  
    */
    TCCR1B = (1 << WGM12) | (1 << CS10);
   
    /* set initial duty cycle to zero */
    OCR1A = 0;
   
   
    /* Setup Timer0 */
    TCCR0 |= (1 << CS00);
    TCNT0 = 0;
    TIMSK |= (1 << TOIE0);
    sample_count = 2;
    sei(); //Enable interrupts
}



ISR(TIMER0_OVF_vect)
{
	sample_count--;
	
	if (sample_count == 0)
		{
			sample_count = 2;          
			OCR1A = pgm_read_byte(&pcm_samples[sample++]);
			
			if(sample>pcm_length)
			{
				sample=0;
			}
		}
}



int main(void)
{
   pwm_init();
   
   DDRC = (1 << PC0);
   while(1);
}


