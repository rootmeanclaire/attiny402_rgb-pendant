#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Set up left motor on PA1 (WO1)
#define PIN_MOTOR_L 1
// Set up right motor on PA2 (WO2)
#define PIN_MOTOR_R 2

#define LEN_SIN 32
const uint8_t SIN[LEN_SIN] = {127, 152, 176, 198, 217, 233, 244, 252, 254, 252, 244, 233, 217, 198, 176, 152, 127, 102, 78, 56, 37, 21, 10, 2, 0, 2, 10, 21, 37, 56, 78, 102};

volatile uint8_t iLeft = 0;
volatile uint8_t iRight = LEN_SIN / 4;

void main() {
	// Wait for RTC to be ready
	while (RTC_STATUS);

	// Set up RTC timer for fade effect
	// Set clock source to 1.024kHz from LFO
	RTC_CLKSEL = RTC_CLKSEL_INT1K_gc;
	// Wait for changes to be applied
	while (RTC_STATUS);
	RTC_CTRLA = (
		// Set RTC prescaler to 1 (technically not necessary)
		RTC_PRESCALER_DIV1_gc
	);
	// Set RTC period
	RTC_PER = 100;
	RTC_INTCTRL |= (
		// Enable interrupt on overflow
		RTC_OVF_bm
	);
	// Wait for changes to be applied
	while (RTC_STATUS);
	RTC_CTRLA |= (
		// Enable RTC
		RTC_RTCEN_bm
	);
	
	// Set up PWM timer
	TCA0_SPLIT_CTRLD |= (
		// Enable split mode
		TCA_SPLIT_SPLITM_bm
	);
	// Set PWM timer period
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
	
	// Enable global interrupts
	sei();
	
	while (1);
}


ISR(RTC_CNT_vect) {
	// Update PWM duty cycles
	TCA0_SPLIT_LCMP1 = SIN[iLeft];
	TCA0_SPLIT_LCMP2 = SIN[iRight];
	
	// Increment PWM duty cycles
	iLeft++;
	iRight++;
	if (iLeft == LEN_SIN) {
		iLeft = 0;
	}
	if (iRight == LEN_SIN) {
		iRight = 0;
	}
	
	// Clear interrupt flag
	RTC_INTFLAGS = RTC_OVF_bm;
}
