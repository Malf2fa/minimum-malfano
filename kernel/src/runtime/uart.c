#include "minemu/platform.h"
#include "minemu/uart.h"
#include <stdarg.h>

void uart_putc(char c){ //sends a SINGLE char over UART0
    while(!(MINEMU_UART0->status & MINEMU_UART_STATUS_TX_READY)){} 
    MINEMU_UART0->tx_data = (uint32_t)(unsigned char)c; //
}

void uart_puts(const char *s){ //null terminator
    while(*s){
        uart_putc(*s++); 
    }
}



void print_number(int value, int base) { //(accidentally given by gemini as well, thanks?)
    char buff[36];
    int i = 0;
    int neg = 0;
    unsigned int n;

    if(value==0){ //zero case
        uart_putc('0');
        return;
    }

    if (value < 0 && base ==10){ //sign handling
        neg =1;
        n = (unsigned int)(-value);
    }else{
        n = (unsigned int)value;
    }
    while(n>0){ //generate string backwards
        int remainder = n % base;
        if (remainder <10 ){
            buff[i++] = remainder + '0';
        }else{
            buff[i++] = (remainder - 10) + 'a';
        }
    }
    n = n/base;
    if(neg){ //add minus sign to bacl
        buff[i++] = '-'; 
    }
    //to print backwards:
    while (i--){
        uart_putc(buff[i]);
    }
    
}

void uart_printf(const char *fmt, ...) { //Gemini 3.1's printf function
    va_list args;           // A pointer to the list of arguments
    va_start(args, fmt);    // Initialize the pointer right after the 'fmt' string

    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++; // Move past the '%' to see what the specifier is
            
            switch (*fmt) {
                case 'c':
                    // Chars are promoted to ints when passed through variadic arguments
                    uart_putc((char)va_arg(args, int)); 
                    break;
                case 's':
                    uart_puts(va_arg(args, char *));
                    break;
                case 'd':
                    print_number(va_arg(args, int), 10);
                    break;
                case 'x':
                    print_number(va_arg(args, int), 16);
                    break;
                case '%':
                    uart_putc('%'); // Handle an escaped percent sign (%%)
                    break;
                default:
                    // If they put a garbage specifier, just print the raw text
                    uart_putc('%');
                    uart_putc(*fmt);
                    break;
            }
        } else {
            // Normal character, pass it straight to the hardware
            uart_putc(*fmt);
        }
        fmt++; // Move to the next character in the format string
    }
    
    va_end(args); // Clean up
}