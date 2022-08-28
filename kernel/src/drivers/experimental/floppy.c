/**
 * @brief Драйвер контроллера гибких дисков
 * @author Арен Елчинян
 * @license GNU GPL 3.0
 * @version 0.0.1
 * @date 26.07.2022
 * @warning Я не даю никаких гарантий и не несу ответственность. Используйте на свой страх и риск.
*/
#include <kernel.h>

static volatile bool done = false;
static bool dchange = false;
static bool motor = false;
static int mtick = 0;
static volatile int tmout = 0;
static uint8_t status[7] = { 0 };
static uint8_t statsz = 0;
static uint8_t sr0 = 0;
static uint8_t fdc_track = 0xff;

typedef struct floppy_info{
    char* type_name;
    int8_t heads;
    int8_t tracks;
    int8_t sectors;
    uint16_t sector_size;
    int8_t gap_len;
} floppy_info_t;


static floppy_info_t floppy_drive_types[] = {
    {"NONE",            0,  0,  0,   0,    0},
    {"360KB 5.25\"",    2, 40,  9, 512, 0x2A},
    {"1.2MB 5.25\"",    2, 80, 15, 512, 0x2A},
    {"720KB 3.5\"",     2, 80,  9, 512, 0x1B},
    {"1.44MB 3.5\"",    2, 80, 18, 512, 0x1B},
    {"2.88MB 3.5\"",    2, 80, 36, 512, 0x1B}
};


/**
 * @brief Функция приема прерываний от контроллера гибких дисков
 * @param r Регистры переданные обработчиком прерываний
 */
void floppy_handler(regs_t *r) {
    tty_printf("\nInterrupt!\n");
    done = true;
}


/**
 * @brief 
 * 
 * @param sensei 
 * @return true 
 * @return false 
 */
bool floppy_wait(bool sensei) {
    while (!done) {
        asm volatile("hlt");
    }
}



/**
 * @brief Возвращение контроллера в обычное состояние
 * 
 */
void floppy_reset() {
    qemu_log("FDC_DOR 0");
    // Останавливаем мотор и выключаем IRQ/DMA
    outb(FDC_DOR,0);
   
    mtick = 0;
    motor = false;

    qemu_log("FDC_DRS 0");
    // Cкорость передачи данных программы (500K/s) 
    outb(FDC_DRS,0);

    qemu_log("FDC_DOR 0x0c");
    // Снова разрешаем прерывания
    //outb(FDC_DOR, 0x0C);

    // Ждем пока получим прерывание
    done = false;
    qemu_log("floppy_wait 1");
    floppy_wait(true);
}


/**
 * @brief Функция инициализации драйвера контроллера гибких дисков
 * @return true В случае успешной установки драйвера
 * @return false В случае ошибки, подробности в логах
 */
bool floppy_install() {
    qemu_log("Installing Floppy driver...");

    /* 
        Получаем информацию о контроллерах Floppy через CMOS, 
        Номер порта = 0x70, 0x71, 
        Регистр = 0x10 
    */
	outb(0x70, 0x10);
    uint8_t drives = inb(0x71);

    tty_printf("Floppy drives:\n");
    if ((drives >> 4) > 0) {
        tty_printf("\t-A: [%s]\n", floppy_drive_types[drives >> 4].type_name);
    }

    if ((drives & 0xf) > 0) {
        tty_printf("\t-B: [%s]\n", floppy_drive_types[drives & 0xf].type_name);
    }
    qemu_log("Setup interrupts");
    // Так как floppy имеет IRQ - 6, а IRQ начинаются с 33(клавиатура), то прерывание будет 32 + 6 = 38
    register_interrupt_handler(32 + 6, &floppy_handler);

    qemu_log("IRQ_clear_mask");
    // Чтобы принимать прерывания контроллера, очистим маску IRQ
    IRQ_clear_mask(6);

    qemu_log("floppy_reset");
    //floppy_reset();

    return true;
}