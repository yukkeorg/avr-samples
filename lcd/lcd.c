#include "config.h"
#include "lcd.h"

void lcd_send_full(uint8_t mode, uint8_t rw, uint8_t data) {
    uint8_t p = LCD_PORT;
    p &= ~(LCD_D | LCD_RS | LCD_RW);
    p |= (mode << LCD_RSP);
    p |= (rw << LCD_RWP);
    p |= (((data & 0x01)     ) << LCD_DP1);
    p |= (((data & 0x02) >> 1) << LCD_DP2);
    p |= (((data & 0x04) >> 2) << LCD_DP3);
    p |= (((data & 0x08) >> 3) << LCD_DP4);
    LCD_PORT_OFF(LCD_E);
    _delay_us(1);
    LCD_PORT = p;
    _delay_us(1);
    LCD_PORT_ON(LCD_E);
    _delay_us(1);
    LCD_PORT_OFF(LCD_E);
}

static void _lcd_check_bf(void) {
    BITOFF(LCD_DDR, LCD_D4);
    BITOFF(LCD_PORT, LCD_D4);
    do {
        lcd_send_full(LCD_COMMAND, LCD_READ, 0);
#ifdef INHIBIT_PULLUP
    } while(LCD_PIN & LCD_D4);
#else
    } while(!(LCD_PIN & LCD_D4));
#endif
    BITON(LCD_DDR, LCD_D4);
}

void lcd_send_cmd(uint8_t cmd) {
    lcd_send(LCD_COMMAND, (cmd >> 4));
    lcd_send(LCD_COMMAND, cmd);
    _lcd_check_bf();
}

void lcd_putdata(uint8_t c) {
    lcd_send(LCD_DATA, (c >> 4));
    lcd_send(LCD_DATA, c     );
    _lcd_check_bf();
}

void lcd_gotoxy(uint8_t x, uint8_t y) {
    uint8_t address = 0x80 + ((0x40 * y) + x);
    lcd_send_cmd(address);
    _lcd_check_bf();
}

void lcd_puts(char *s) {
    while(*s) {
        lcd_putdata((uint8_t)*s);
        s++;
    }
}

void lcd_setcgram(uint8_t address, uint8_t data[]) {
    lcd_send_cmd(0x40 | ((address << 3) & 0x38));
    for(uint8_t i = 0; i < 8; i++) {
        lcd_putdata(data[i]);
    }
}

void lcd_init(void) {
    BITON(LCD_DDR, (LCD_D | LCD_RS | LCD_RW | LCD_E));
    BITOFF(LCD_PORT, (LCD_D | LCD_RS | LCD_RW | LCD_E));

    _delay_ms(30);
    lcd_send(LCD_COMMAND,LCD_8BIT_MODE);
    _delay_ms(5);
    lcd_send(LCD_COMMAND,LCD_8BIT_MODE);
    _delay_ms(5);
    lcd_send(LCD_COMMAND,LCD_8BIT_MODE);
    _delay_ms(5);
    lcd_send(LCD_COMMAND,LCD_4BIT_MODE);

    lcd_send_cmd(LCD_FUNCSET);
    _delay_us(50);
    lcd_send_cmd(LCD_DISPOFF);
    _delay_us(50);
    lcd_send_cmd(LCD_DISPON);
    _delay_us(50);
    lcd_send_cmd(LCD_DISPCLEAR);
    _delay_ms(3);
    lcd_send_cmd(LCD_ENTMODE);
    _delay_us(50);
}

