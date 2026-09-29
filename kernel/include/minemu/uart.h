#ifndef MINEMU_UART_H
#define MINEMU_UART_H


//send to console
void uart_putc(char c); //char
void uart_puts(const char *s); //string
void uart_printf(const char *fmt, ...);//format str, prepare for printf, cuz we cant use it #aura

#endif