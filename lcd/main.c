#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"

static uint8_t cgram[][8] = {
    {
        0b00000000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010,
        0b00000001,
        0b00000000,
        0b00000000,
    }, {
        0b00000111,
        0b00000101,
        0b00000111,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    }
};

/* ---------------------------------------------------- */

// _delay_ms() の10倍バージョン
void delay_ms_x10(int count) {
    for(int i = 0; i < count; i++) {
        _delay_ms(10);
    }
}

// 文字列をnpad分右揃えにし、余った文字分をスペースで埋める
void padding(char *out, char *in, int npad) {
    int len = strlen(in);
    int padsize = npad - len;
    if(padsize > 0) {
        while(padsize--) {
            *out++ = ' ';
        }
    }
    strcpy(out, in);
}

/* ---------------------------------------------------- */

// アナログデジタル変換初期化

void adc_init(void) {
    ADMUX = (_BV(REFS0));  // AVCC利用
    ADCSRA = (_BV(ADEN) | _BV(ADPS0));  // CK/2
    ADCSRB = 0;  //連続変換動作
}

// アナログデジタル変換開始
#define adc_start() do { ADCSRA |= _BV(ADIF); ADCSRA |= _BV(ADSC); } while(0)
// アナログデジタル変換待ち
#define adc_wait()  loop_until_bit_is_set(ADCSRA, ADIF)
// アナログデジタル変換値読込
#define adc_read(d) do { (d) = ADCL; (d) |= ((uint16_t)ADCH) << 8; } while(0)

/* ---------------------------------------------------- */

void init_mcu(void) {
    lcd_init();
    adc_init();

    // CGRAM登録
    lcd_setcgram(0x01, cgram[0]);
    lcd_setcgram(0x02, cgram[1]);
}

int main(void) {
    char indicator[] = { '/', '-', '\x01', '|' };
    char buf[16], work[16];
    uint8_t i = 0;
    uint16_t volt = 0;
    int16_t deg;

    init_mcu();

    lcd_gotoxy(0, 0);
    lcd_puts("AirTemp.");

    for(;;) {
        adc_start();
        adc_wait();
        adc_read(volt);

        deg = (volt >> 1) - 50;

        itoa(deg, work, 10);
        padding(buf, work, 4);

        lcd_gotoxy(9, 0);
        lcd_puts(buf);
        lcd_puts("\x02""C");
        lcd_putdata(indicator[(++i) & 0x03]);

        delay_ms_x10(50);
    }

    return 0;
}
