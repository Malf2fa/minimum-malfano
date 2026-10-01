#ifndef MINEMU_UART_H
#define MINEMU_UART_H


//send to console (task1)
void uart_putc(char c); //char
void uart_puts(const char *s); //string
void uart_printf(const char *fmt, ...);//format str, prepare for printf, cuz we cant use it #aura


//task 2
void uart_init(void);
void uart_irq_handle(void); //interrupt handler
char uart_getc(void); //stop read
#endif