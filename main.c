#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif
#include <util/delay.h>
#define BUZZER_PIN PB0
#define DELAY_MS 500 // 500 milliseconds delay


void ADC_Init() {
	// Set the reference voltage to AVcc with external capacitor at AREF pin
	ADMUX = (1 << REFS0);

	// Enable ADC, set the prescaler to 128
	// (For 16 MHz clock, ADC clock will be 125 kHz)
	ADCSRA = (1 << ADEN) | (7 << ADPS0);
}

uint16_t ADC_Read(uint8_t channel) {
	// Select the corresponding channel (0-7) and mask the rest
	ADMUX = (ADMUX & 0xF8) | (channel & 0x07);

	// Start the conversion
	ADCSRA |= (1 << ADSC);

	// Wait for the conversion to complete
	while (ADCSRA & (1 << ADSC));

	return ADCW; // Return the ADC value
}


void setup() {
	// Set BUZZER_PIN as an output
	DDRB |= (1 << BUZZER_PIN);
}

void loop() {
	// Toggle the buzzer on
	PORTB |= (1 << BUZZER_PIN);
	_delay_ms(DELAY_MS); // Wait for 500 milliseconds

	// Toggle the buzzer off
	PORTB &= ~(1 << BUZZER_PIN);
	_delay_ms(DELAY_MS); // Wait for 500 milliseconds
}

int main() {
	uint32_t elapsed_time = 0;
	uint32_t end_time = 5000; // Run for 5000 milliseconds (5 seconds)

	

	// Optionally, turn off the buzzer at the end
	PORTB &= ~(1 << BUZZER_PIN);
	// Initialize the ADC
	ADC_Init();
	
	// Variable to store the ADC result
	uint16_t adc_result;

	// Set Port D as output to light up an LED (if used)
	PORTB = 0x00;
	DDRB  = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	setup();

	while (1) {
		// Read the ADC value from channel 0 (PA0)
		adc_result = ADC_Read(0);

		// Do something with the adc_result
		// For example, if raindrop detected (threshold value for example is 512)
		if (adc_result < 512) {
			
			while (elapsed_time < end_time) {
				loop();
				elapsed_time += 2 * DELAY_MS; // Each loop iteration takes 2 * DELAY_MS milliseconds
			}
			//_delay_us(3500); // Generate 2000 microseconds pulse for 180 degrees
			
			// Rotate Motor to 180 degrees
			PORTC = 0x02;
			_delay_us(3500); // Generate 2000 microseconds pulse for 180 degrees
			PORTC = 0x00;
			_delay_ms(3000);
			
			PORTC = 0x01;
			_delay_us(5000); // Generate 2000 microseconds pulse for 180 degrees
			PORTC = 0x00;

			_delay_ms(3000); // Wait for 3 seconds
		}
		else
		{
			PORTC = 0x01;
			_delay_us(1000); // Generate 1000 microseconds pulse for 0 degrees
			PORTC = 0x00;
			_delay_ms(3000); // Wait for 3 seconds
			
			PORTC = 0x02;
			_delay_us(1000); // Generate 1000 microseconds pulse for 0 degrees
			PORTC = 0x00;
			_delay_ms(3000);
		}


	}
}