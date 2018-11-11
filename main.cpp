/**
 * A minimal example how to push data (parallel) to 8bit shift register with ATMega8
 * The example is an binary counter and outputs data from 0 to 255
 *
 * Port assignments
 * `-> SER:   PD5 // Port to transmit data
 * `-> SRCLK: PD6 // Clock port for data transmission
 * `-> RCLK:  PD7 // Latch port. Show data.
 *
 * @author Igor Martens
 * @since 11.11.2018
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void setup() {
    // Setup output ports
    DDRD |= (1<<DDD5);  // SER Port
    DDRD |= (1<<DDD6);  // SRCLK Port
    DDRD |= (1<<DDD7);  // RCLK Port
}

/**
 * Push the given byte to the register and finally execute latch.
 * The left bit will be pushed first.
 */
void pushByteAndLatch(uint8_t byte) {
    for (uint8_t i=0; i<8; ++i) {
        (byte & 128) ? PORTD |= (1 << PD5) : PORTD &= ~(1 << PD5);
        PORTD |= (1 << PD6);
        PORTD &= ~(1 << PD6);
        byte = byte << 1;
    }

    PORTD |= (1 << PD7);
    PORTD &= ~(1 << PD7);
}

/**
 * Set the data inside register to zero and latch
 * This we only need to save one port on the MCU.
 * If the MCU have enough ports SRCLR on the shift register can be used to clear data instead of this method.
 */
void clearRegister() {
    pushByteAndLatch(0b00000000);
}

int main(void) {
    setup();
    clearRegister();

    while(1) {
        for (uint8_t i=0; i<=255; ++i) {
            // Push the data to show 0 to 9 and A to F on the eight segment display
            pushByteAndLatch(i);
            _delay_ms(300);
        }
    }

    return 0;
}
