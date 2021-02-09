#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// Set up motor on PA1 (WO1)
#define PIN_MOTOR_L 1

#define LEN_SIN 32
const uint8_t SIN[LEN_SIN] = {127, 152, 176, 198, 217, 233, 244, 252, 254, 252, 244, 233, 217, 198, 176, 152, 127, 102, 78, 56, 37, 21, 10, 2, 0, 2, 10, 21, 37, 56, 78, 102};

void main() {
	// Set up timer
	TCA0_SPLIT_CTRLD |= (
		// Enable split mode
		TCA_SPLIT_SPLITM_bm
	);
	// Set timer period
	TCA0_SPLIT_LPER = 0xFF;
	TCA0_SPLIT_CTRLA |= (
		// Enable timer
		TCA_SPLIT_ENABLE_bm |
		// Set clock divder
		TCA_SPLIT_CLKSEL_DIV1_gc
	);
	
	// Set up PWM output
	TCA0_SPLIT_CTRLB |= (
		// Override output for PWM
		TCA_SPLIT_LCMP1EN_bm
	);
	/*TCA0_SINGLE_EVCTRL &= ~(
		// Count in clock ticks
		TCA_SINGLE_CNTEI_bm
	);
	TCA0_SPLIT_CTRLB |= (
		// Set waveform generation mode to single-slope PWM
		TCA_SINGLE_WGMODE_SINGLESLOPE_gc
	);*/
	// Enable left motor pin for output
	PORTA_DIR |= (1 << PIN_MOTOR_L);
	
	uint8_t i = 0;

	while (1) {
		// Update PWM duty cycle
		TCA0_SPLIT_LCMP1 = SIN[i];
		
		// Increment PWM duty cycle
		i++;
		if (i == LEN_SIN) {
			i = 0;
		}
		
		// Wait a bit
		// for (uint16_t i = 0; i < 10000; ++i);
		_delay_ms(10);
	}
}

