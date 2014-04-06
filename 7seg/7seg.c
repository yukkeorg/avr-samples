#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>

// -----------------------------

volatile uint16_t g_counter = 0;
volatile uint16_t g_counter2 = 0;

// -----------------------------
//const prog_uint8_t digit[10] = { 
const uint8_t digit[] = { 
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111100, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01100111, // 9
  0b00000000, // sp
  0b01000000, // -
};

void init() {
  // PORT Settings
  PORTB = 0x00;
  PORTD = 0x00;
  DDRB = 0xff;
  DDRD = 0xff;
  
  // Timer0 : about 1ms.
  TCCR0A = 0x00;
  //TCCR0B = (_BV(CS02) | _BV(CS00));  // 1/1024
  TCCR0B = (_BV(WGM01) | _BV(CS01) | _BV(CS00));  // CTC Mode, 1/64
  //TCCR0B = (_BV(WGM01) | _BV(CS02) | _BV(CS00) );  // CTC Mode, 1/256
  TCNT0 = 0x00;
  OCR0A = 0x7c;
  TIMSK0 |= (_BV(OCIE0A));
  //TIMSK0 |= _BV(TOIE0);

  // Enable interrupt.
  sei();
}

// -----------------------------
volatile static uint8_t disp_data[2][4] = {{0}};
volatile static uint8_t disp_max_ch = 0;
volatile static uint16_t disp_ticks = 1000;

void _led_disp() {
  static uint8_t pos= 0;
  static uint8_t ch = 0;
  static uint16_t nticks = 0;

  PORTD = digit[disp_data[ch][pos]];
  PORTB = (1 << (3 - pos));

  pos++;
  pos &= 0x03;

  nticks++;
  if(nticks > disp_ticks) {
    ch++;
    if(ch > disp_max_ch) {
      ch = 0;
    }
    nticks = 0;
  }
}

#define led_set(data, ch, pos)    (disp_data[ch][pos] = data)
#define led_set_max_ch(ch)        (disp_max_ch = ch)

void _num2led(uint16_t num, uint8_t ch) {
  for(uint8_t i = 0; i < 4; i++) {
    uint16_t t = num % 10;
    if(i > 0 && num == 0 && t == 0) {
      t = 10;
    }
    led_set(t, ch, i);
    num /= 10;
  }
}

void num2led(uint16_t num) {
  uint16_t h = 0;
  h = num / 10000;

  if(h != 0) {
    _num2led(h, 1);
    led_set_max_ch(1);
  }
  else {
    led_set_max_ch(0);
  }
  _num2led(num, 0);
}

// -----------------------------

ISR(TIMER0_COMPA_vect) {
//ISR(TIMER0_OVF_vect) {
  _led_disp();
  g_counter++;
  if(g_counter > 1000) {
    g_counter2++;
    g_counter = 0;
  }
  TCNT0 = 0;
}

// -----------------------------

void main() {
  init();
  for(;;) { num2led(g_counter2); }
}
