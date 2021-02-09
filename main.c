#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// Set up left motor on PA1 (WO1)
#define PIN_MOTOR_L 1
// Set up right motor on PA2 (WO2)
#define PIN_MOTOR_R 2

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
		// Override outputs for PWM
		TCA_SPLIT_LCMP1EN_bm |
		TCA_SPLIT_LCMP2EN_bm
	);
	
	// Enable left motor pin for output
	PORTA_DIR |= (1 << PIN_MOTOR_L) | (1 << PIN_MOTOR_R);
	
	uint8_t iLeft = 0;
	uint8_t iRight = LEN_SIN / 4;

	while (1) {
		// Update PWM duty cycles
		TCA0_SPLIT_LCMP1 = SIN[iLeft];
		TCA0_SPLIT_LCMP2 = SIN[iRight];
		
		// Increment PWM duty cycles
		iLeft++;
		if (iLeft == LEN_SIN) {
			iLeft = 0;
		}
		iRight++;
		if (iRight == LEN_SIN) {
			iRight = 0;
		}
		
		// Wait a bit
		_delay_ms(10);
	}
}

