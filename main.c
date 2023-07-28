#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Set up red channel on PA3
const uint8_t PIN_RED = 3;
// Set up green channel on PA1
const uint8_t PIN_GREEN = 1;
// Set up blue channel on PA7
const uint8_t PIN_BLUE = 7;
// Set up blue channel on PA6 (TCB WO)
const uint8_t PIN_PWN = 6;

typedef struct _Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

static const Color TEAL = {0, 255, 255};
static const Color PINK = {255, 0, 255};
static const Color WHITE = {255, 255, 255};
static const Color BLACK = {0, 0, 0};

const uint8_t ON_RATIO = 4;
volatile uint8_t ticks = 0;
volatile uint8_t iRGB = 0;
volatile Color currColor = BLACK;
#define LEN_SEQ 4
static const Color *SEQUENCE[LEN_SEQ] = {&TEAL, &PINK, &WHITE, &PINK};


void showColor();


void main() {
	// Set up main clock
	// Write to protected register
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL_MCLKCTRLB = (
		// Set main clock prescaler to 64
		CLKCTRL_PDIV_64X_gc
		// Enable main clock prescaler
		| CLKCTRL_PEN_bm
	);
	// Write to protected register
	CPU_CCP = CCP_IOREG_gc;
	// Use 32kHz low-power oscillator as main clock
	CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
	// Wait for changes to be applied
	while (CLKCTRL_MCLKSTATUS & CLKCTRL_SOSC_bm);
	
	// Wait for RTC to be ready
	while (RTC_STATUS);
	
	// Set up RTC timer for fade effect
	// Set clock source to 1kHz from LFO
	RTC_CLKSEL = RTC_CLKSEL_INT1K_gc;
	// Wait for changes to be applied
	while (RTC_STATUS);
	RTC_CTRLA = (
		// Set RTC prescaler to 1 (technically not necessary)
		RTC_PRESCALER_DIV1_gc
		// Allow RTC to run in standby mode
		| RTC_RUNSTDBY_bm
	);
	// Set RTC period
	RTC_PER = 16;
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
	
	// Set up Timer B for PWM
	TCB0_CTRLA |= TCB_RUNSTDBY_bm;
	TCB0_CCMPL = 0xFF;
	TCB0_CTRLB |= (
		// Set mode to 8-bit PWM
		TCB_CNTMODE_PWM8_gc
		// Enable output
		| TCB_CCMPEN_bm
	);
	// Enable timer
	TCB0_CTRLA |= TCB_ENABLE_bm;
	
	// Enable LED pins for output
	PORTA_DIR |= (1 << PIN_RED) | (1 << PIN_GREEN) | (1 << PIN_BLUE) |
		(1 << PIN_PWN);
	
	// Enable global interrupts
	sei();
	// Prepare to put CPU into standby mode
	// Only wake for RTC interrupt
	set_sleep_mode(SLEEP_MODE_STANDBY);
	// Enable sleep mode
	sleep_enable();
	// Endless loop to keep from returning
	while (1);
}


ISR(RTC_CNT_vect) {
	if (i == 0)
		if (i % ON_RATIO == 0) {
			showColor(SEQUENCE[i / ON_RATIO]);
			TCB0_CCMPH = 127;
		} else if (i % ON_RATIO == ON_RATIO - 1) {
			showColor(&BLACK);
		}
		
		// Increment sequence index
		i++;
		if (i == LEN_SEQ * ON_RATIO) {
			i = 0;
		}
	}
	
	showColor();
	i++;
	
	// Clear interrupt flag
	RTC_INTFLAGS = RTC_OVF_bm;
}

void showColor() {
	if (iColor == 0) {
		PORTA &= ~(1 << PIN_BLUE);
		TCB0_CCMPH = currColor->red;
		PORTA |= (1 << PIN_RED);
	} else if (iColor == 1) {
		PORTA &= ~(1 << PIN_RED);
		TCB0_CCMPH = currColor->green;
		PORTA |= (1 << PIN_GREEN);
	} else {
		PORTA &= ~(1 << PIN_GREEN);
		TCB0_CCMPH = currColor->blue;
		PORTA |= (1 << PIN_BLUE);
		iColor = 0;
	}
}
