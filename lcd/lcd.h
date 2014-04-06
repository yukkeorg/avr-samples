#ifndef __LCD_H__
#define __LCD_H__

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LCD_DDR     (DDRB)
#define LCD_PORT    (PORTB)
#define LCD_PIN     (PINB)

#define LCD_RSP     (PB0)
#define LCD_RWP     (PB1)
#define LCD_EP      (PB2)

#define LCD_DP1     (PB3)
#define LCD_DP2     (PB4)
#define LCD_DP3     (PB5)
#define LCD_DP4     (PB6)

#define LCD_RS      (_BV(LCD_RSP))
#define LCD_RW      (_BV(LCD_RWP))
#define LCD_E       (_BV(LCD_EP))

#define LCD_D1      (_BV(LCD_DP1))
#define LCD_D2      (_BV(LCD_DP2))
#define LCD_D3      (_BV(LCD_DP3))
#define LCD_D4      (_BV(LCD_DP4))
#define LCD_D       (LCD_D4 | LCD_D3 | LCD_D2 | LCD_D1)

#define LCD_8BIT_MODE   (0x03)
#define LCD_4BIT_MODE   (0x02)
#define LCD_FUNCSET     (0x24)
#define LCD_DISPOFF     (0x08)
#define LCD_DISPON      (0x0C)
#define LCD_DISPCLEAR   (0x01)
#define LCD_ENTMODE     (0x06)

#define LCD_COMMAND     0
#define LCD_DATA        1

#define LCD_WRITE       0
#define LCD_READ        1

#define BITON(v, d)     ((v) |= (d))
#define BITOFF(v, d)    ((v) &= ~(d))
#define BITTOGGLE(v, d) ((v) ^= (d))

#define LCD_PORT_ON(d)  BITON(LCD_PORT, (d))
#define LCD_PORT_OFF(d) BITOFF(LCD_PORT, (d))

void lcd_init(void);
void lcd_send_full(uint8_t mode, uint8_t rw, uint8_t data);
void lcd_send_cmd(uint8_t cmd);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_putdata(uint8_t c);
void lcd_puts(char *s);
void lcd_setcgram(uint8_t address, uint8_t data[]);

#define lcd_send(m, d)  lcd_send_full((m), LCD_WRITE, (d))

#endif
