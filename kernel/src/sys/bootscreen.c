/**
 * @file kernel.c
 * @author Никита Пиминов (github.com/pimnik98)
 * @brief BootScreen - Красивая загрузка ядра с сообщениями
 * @version 0.1.0
 * @date 2022-08-27
 * @copyright Copyright Piminoff Nikita (c) 2022
 */
#include <kernel.h>

uint32_t theme = 0; // 0 - Dark / 1 - Light
uint32_t bgColorDark = 0x333333;
uint32_t txColorDark = 0x43ACE8;
uint32_t bgColorLight = 0xD6D2D0;
uint32_t txColorLight = 0x2D3C5D;
uint32_t maxStrLine = 0;
uint32_t maxHeightLine = 0;
bool lazy = false;
uint32_t curElem = 0;
uint32_t maxElem = 10;
uint32_t mode = 0;  // 0 - Classic | 1 - Only Log

/**
 * @brief Включить ленивую загрузку для BootScreen
 *
 * @param bool l - true/false - Вкл/Выкл.
 */
void bootScreenLazy(bool l){
    lazy = l;
}

/**
 * @brief Сменить тему BootScreen
 *
 * @param uint32_t th - 0 - Dark | 1 - Light
 */
void bootScreenChangeTheme(uint32_t th){
    theme = th;
}

/**
 * @brief Возращает цвет оформления
 *
 * @return uint32_t - код цвета
 */
uint32_t bootScreenTheme(uint32_t type){
    if (theme == 0 && type == 0){
        return txColorDark;
    } else if (theme == 0 && type == 1){
        return bgColorDark;
    } else if (theme == 0 && type == 2){
        return 0x262626;
    } else if (theme == 1 && type == 0){
        return txColorLight;
    } else if (theme == 1 && type == 1){
        return bgColorLight;
    } else if (theme == 1 && type == 2){
        return txColorLight;
    } else {
        return txColorDark;
    }
}

/**
 * @brief Завершает работу BootScreen
 *
 * @param uint32_t bg - Отчистить указаным цветом экран
 * @param uint32_t tx - Установить цвет для вывода текста
 */
void bootScreenClose(uint32_t bg, uint32_t tx){
    tty_setcolor(tx);
    drawRect(0,0,getWidthScreen(),getHeightScreen(),bg);
    setPosX(0);
    setPosY(0);
    tty_changeState(true);
}

void bootScreenChangeMode(uint32_t m){
    mode = m;
}

/**
 * @brief Выводит во время загрузки служебную информацию BootScreen
 *
 */
void bootScreenInfo(){
    setPosX(0);
    setPosY(0);
    if (!lazy){
        tty_printf("SynapseOS v%d.%d.%d\nBuilt: %s\n",
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,    // Версия ядра
        __TIMESTAMP__                                   // Время окончания компиляции ядра
        );

        char* about = "bootScreen by Piminoff Nikita";
        uint32_t centerAbout = (maxStrLine/2)-(strlen(about)/2);

        tty_setcolor(bootScreenTheme(2));
        setPosX(((1+centerAbout)*8));
        setPosY(getHeightScreen()-32);
        tty_printf(about);
    }
    setPosX(0);
    setPosY(16*5);


}

/**
 * @brief Рисует прогресс-бар для BootScreen
 *
 */
void bootScreenProcentPaint(){
    curElem++;
    if (curElem > maxElem){
        curElem = maxElem;
    }
    uint32_t padding_h = maxHeightLine/4;
    uint32_t proc = (curElem*100)/maxElem;
    //setPosX(8*8);

    drawRect(8*8,(16*((maxHeightLine-padding_h+2))),(proc)*8,16,bootScreenTheme(0));
}

/**
 * @brief Обновить информацию для BootScreen
 *
 * @param char* title - Вывести данное сообщение
 */
void bootScreenPaint(char* title){
    qemu_log("[BOOT] %s",title);
    if (mode == 1){
        tty_changeState(true);
        tty_setcolor(bootScreenTheme(0));
        tty_printf("%s\n",title);
        tty_changeState(false);
        punch();
        return;
    }
    maxStrLine = (getWidthScreen()/8)-2;
    maxHeightLine = getHeightScreen()/16;
    tty_setcolor(bootScreenTheme(0));
    tty_changeState(true);
    uint32_t centerTitle = (maxStrLine/2)-(strlen(title)/2);
    uint32_t padding_h = maxHeightLine/4;
    if (lazy){
        drawRect(0,16*((maxHeightLine-padding_h)),getWidthScreen(),16,bootScreenTheme(1));
    } else {
        drawRect(0,0,getWidthScreen(),getHeightScreen(),bootScreenTheme(1));
    }
    setPosX(((1+centerTitle)*8));
    setPosY(16*((maxHeightLine-padding_h)));
    tty_printf(title);
    bootScreenInfo();
    bootScreenProcentPaint();
    tty_changeState(false);
    punch();

}

/**
 * @brief Инициализирует BootScreen
 *
 * @param uint32_t count - Кол-во этапов
 */
void bootScreenInit(uint32_t count){
    // Предварительная настройка BootScreen
    maxElem = count;
    qemu_log("Init...");
    tty_changeState(false);
    maxStrLine = (getWidthScreen()/8)-2;
    maxHeightLine = getHeightScreen()/16;
    bootScreenPaint("Starting...");

}
