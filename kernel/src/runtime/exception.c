#include "minemu/irq.h"
#include "minemu/syscall.h"
#include "minemu/platform.h"
#include "minemu/uart.h"

void minemu_fail_stop(void) {
    for (;;) {
        __asm__ volatile("nop");
    }
}

void minemu_panic(const char *message) {
    (void)message;
    minemu_fail_stop();
}

__attribute__((weak, noreturn)) void minemu_svc_trampoline(void) {
    minemu_fail_stop();
}


__attribute__((weak)) struct minemu_trap_frame *minemu_svc_dispatch(
    struct minemu_trap_frame *frame) {
    (void)frame;
    minemu_fail_stop();
}

__attribute__((weak))struct minemu_trap_frame *minemu_irq_dispatch(
    struct minemu_trap_frame *frame) {
        uint32_t s = frame->exception_id;
        if (s == MINEMU_IRQ_UART0){
            uart_irq_handle();
        }
        MINEMU_INTERRUPT->eoi = s; //controller done
        return frame;
}

__attribute__((weak)) void minemu_undefined_dispatch(struct minemu_trap_frame *frame) {
    (void)frame;
    minemu_fail_stop();
}

__attribute__((weak)) void minemu_abort_dispatch(struct minemu_trap_frame *frame) {
    (void)frame;
    minemu_fail_stop();
}