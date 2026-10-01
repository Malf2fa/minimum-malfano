#include "minemu/boot.h"
#include "minemu/trap.h"
#include "minemu/trace.h"
#include "minemu/uart.h"
#include "minemu/irq.h"

static int starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str != *prefix) return 0;
        str++;
        prefix++;
    }
    return 1;
}

static void msh(void){
    char cmd_buffer[21]; //20+1 null
    while(1){
        uart_puts("msh> ");
        int i = 0;
        while(1){
            char c = uart_getc();
            if (c=='\n'){
                cmd_buffer[i] = '\0';
                break;
            }else if (c == 0x08 || c == 0x7f){
                if(i>0){
                    i--;
                }
            }else{
                if(i<20){ //(after it becomes full ignore)
                    cmd_buffer[i++] = c;
                }
            }
        }
        int pos =0;
        while(cmd_buffer[pos] == ' '){
            pos++;
        }
        if (cmd_buffer[pos] == '\0'){
            continue;
        }
        if(starts_with(cmd_buffer+pos, "echo") && (cmd_buffer[pos+4] == '\0' || cmd_buffer[pos+4] == ' ')){  //checks for valid echo
            pos += 4;
            while(cmd_buffer[pos] == ' '){ //parse through whitespace
                pos++;
            }
            uart_puts(cmd_buffer + pos); 
            uart_putc('\n');
        }else{
            uart_puts("command not found: "); //command err
            while(cmd_buffer[pos] != ' ' && cmd_buffer[pos] != '\0'){
                uart_putc(cmd_buffer[pos++]);
            } 
            uart_putc('\n');
        }
        
    }
}
void minemu_kernel_main(const struct minemu_boot_info *boot_info) {
    if ((uintptr_t)boot_info != MINEMU_BOOT_INFO_VADDR ||
        boot_info->magic != MINEMU_BOOT_INFO_MAGIC ||
        boot_info->version != MINEMU_ABI_VERSION ||
        boot_info->size != sizeof(*boot_info) ||
        boot_info->system_rom_base != UINT32_C(0x08000000) ||
        boot_info->direct_map_vaddr != UINT32_C(0xc0000000) ||
        boot_info->direct_map_paddr != UINT32_C(0x40000000) ||
        boot_info->direct_map_size != UINT32_C(0x04000000)) {
        minemu_trace_event(UINT32_C(0xb007bad0));
        minemu_fail_stop();
    }

    uart_puts("hello world\n");

    uart_init();
    minemu_irq_enable();
    msh();
    minemu_fail_stop();
}