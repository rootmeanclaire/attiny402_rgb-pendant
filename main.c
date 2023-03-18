#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Set up red channel on PA3 (TCA WO0)
const uint8_t PIN_RED = 3;
// Set up green channel on PA1 (TCA WO1)
const uint8_t PIN_GREEN = 1;
// Set up blue channel on PA6 (TCB WO)
const uint8_t PIN_BLUE = 6;

typedef struct _Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

static const Color TEAL = {0, 255, 255};
static const Color PINK = {255, 0, 255};
static const Color WHITE = {255, 255, 255};

// The number of steps per color
const uint8_t STEP_BASE = 4;
const uint8_t STEPS = 1 << STEP_BASE;
// The total number of steps since the start of the sequence
uint8_t i = 0;
#define LEN_SEQ 4
static const Color *SEQUENCE[LEN_SEQ] = {&TEAL, &PINK, &WHITE, &PINK};


void showColor(const Color *color);
Color fadeColor(const Color *c1, const Color *c2, uint8_t weight);


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
	RTC_PER = 256;
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
		TCA_SPLIT_LCMP0EN_bm |
		TCA_SPLIT_LCMP1EN_bm
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
	PORTA_DIR |= (1 << PIN_RED) | (1 << PIN_GREEN) | (1 << PIN_BLUE);
	
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
	Color color;

	if (i >> STEP_BASE == LEN_SEQ - 1) {
		color = fadeColor(
			SEQUENCE[LEN_SEQ - 1],
			SEQUENCE[0],
			(i % STEPS) << (8 - STEP_BASE)
		);
	} else {
		color = fadeColor(
			SEQUENCE[i >> STEP_BASE],
			SEQUENCE[(i >> STEP_BASE) + 1],
			(i % STEPS) << (8 - STEP_BASE)
		);
	}

	showColor(&color);

	// Increment sequence index
	i++;
	if (i == LEN_SEQ << STEP_BASE) {
		i = 0;
	}

	// Clear interrupt flag
	RTC_INTFLAGS = RTC_OVF_bm;
}

void showColor(const Color *color) {
	TCA0_SPLIT_LCMP0 = color->red;
	TCA0_SPLIT_LCMP1 = color->green;
	TCB0_CCMPH = color->blue;
}

Color fadeColor(const Color *c1, const Color *c2, uint8_t weight) {
	Color c3;

	if (c1->red == c2->red) {
		c3.red = c1->red;
	} else {
		c3.red = (
			(uint16_t) ((0xFF - weight) * c1->red) +
			(uint16_t) ((weight) * c2->red)
		) >> 8;
	}

	if (c1->green == c2->green) {
		c3.green = c1->green;
	} else {
		c3.green = (
			(uint16_t) ((0xFF - weight) * c1->green) +
			(uint16_t) ((weight) * c2->green)
		) >> 8;
	}

	if (c1->blue == c2->blue) {
		c3.blue = c1->blue;
	} else {
		c3.blue = (
			(uint16_t) ((0xFF - weight) * c1->blue) +
			(uint16_t) ((weight) * c2->blue)
		) >> 8;
	}

	return c3;
}
