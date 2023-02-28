/**
 * @file pit.c
 * @author Арен Елчинян (a2.dev@yandex.com)
 * @brief
 * @version 0.1.0
 * @date 13-11-2022
 *
 * @license This work is licensed under the Creative Commons  Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)  License.
 * @copyright Арен Елчинян (c) 2022 - 2023
 *
 */

#include <arch.h>
#include <libk.h>

static uint64_t tick = 0;
static uint64_t nanosec = 0;
bool pit_lock = false;
extern process_t *current_process;
extern thread_t *current_thread;

/**
 * @brief
 *
 * @param regs
 */
static void timer_callback(register_t *regs) {
    UNUSED(regs);
    tick++;

    if (!pit_lock) {
        nanosec = rdtsc();
#if RELEASE

        debug_log("ID задачи: %u, ID процесса[%s]: %u, тик: %u, приоритет: %u, наносекунд с прошлой задачи: %u",
            current_thread->id, current_process->name, current_process->pid,
            tick, current_thread->priority, nanosec - last_nanosec);
        last_nanosec = nanosec;

#endif

        scheduler_switch();
    }
}

/**
 * @brief
 *
 * @param frequency
 * @return true
 * @return false
 */
bool pit_init(uint32_t frequency) {
    // Установка вектора прерываний таймера (ЗАРАНЕЕ!!)
    int_set_handler(IRQ0, &timer_callback);

    // Значение, которое мы отправляем в PIT, - это значение,
    // на которое нужно разделить его входные тактовые частоты (1193180 Гц),
    // чтобы получить нашу требуемую частоту.
    // Важно отметить, что делитель должен быть достаточно маленьким,
    // чтобы поместиться в 16 бит.
    uint16_t divisor = 1193180 / frequency;

    nanosec = rdtsc();

    // Отправка командного байта
    ports_outb(0x43, 0x36);

    // Делитель должен быть отправлен по байтам,
    // поэтому делим здесь на верхние / нижние байты.
    uint8_t l = divisor & 0xFF;
    uint8_t h = divisor >> 8;

    // Отправляем делитель частоты
    ports_outb(0x40, l);
    ports_outb(0x40, h);

    debug_log("Частота: %u, делитель %u", frequency, divisor);
    return true;
}
