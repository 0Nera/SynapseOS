/**
 * @file apps/apps/pimnik98/tshell/main.c
 * @authors Пиминов Никита (github.com/pimnik98 | VK: @piminov_remont)
 * @brief TShell
 * @version 0.0.3
 * @date 2022-08-24
 *
 * @copyright Copyright Пиминов Никита (с) 2022
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vesa.h>
#include <tui.h>
#include <time.h>

int32_t oldPosX     = 0;                // Последние местоположение символа по X (место печати)
int32_t oldPosY     = 0;                // Последние местоположение символа по Y (место печати)
int32_t curMenu     = 0;                // Текущие меню
uint32_t bakMenu    = -1;

void headBar(){
    drawLine(2,TUI_BASE_COLOR_HEAD);
    char* OSNAME = " SynapseOS v0.2.12 (Dev)";
    setPosX(0);
    setPosY(16*1);
    puts_color(OSNAME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);

    setColor(getColorsTUI(false));
    char* TIME = "10/10/2022 12:23";
    setPosX((getMaxStrLineTUI()-16)*8);
    setPosY(16*1);
    drawRect((getMaxStrLineTUI()-16)*8,16,18*8, 16,TUI_BASE_COLOR_HEAD);
    puts_color(TIME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);
}

void headBarOld(){
    setPosX(16);
    setPosY(16*4);
    char infoCPU[512];
    substr(infoCPU, format_string("CPU: %s","getNameBrand()"), 0, (getMaxListMenuTUI()/2)-2);
    puts_color(infoCPU,getColorsTUI(false), getColorsTUI(true));

    setPosX(16);
    setPosY(16*5);
    char infoRAM[512];
    //substr(infoRAM, strcat(format_string("RAM: %d",(getInstalledRam())), kb), 0, (getMaxListMenuTUI()/2)-2);
    puts_color(infoRAM,getColorsTUI(false), getColorsTUI(true));

    setPosX(16);
    setPosY(16*6);
    char infoVideo[512];
    substr(infoVideo, "Video: Basic video adapter (Unknown)", 0, (getMaxListMenuTUI()/2)-2);
    puts_color(infoVideo,getColorsTUI(false), getColorsTUI(true));

    setPosX(16);
    setPosY(16*7);
    char infoDisplay[512];
    substr(infoDisplay, (format_string("Display: %s",getDisplayName(),getWidthScreen(),getHeightScreen())), 0, (getMaxListMenuTUI()/2)-2);
    puts_color(infoDisplay,getColorsTUI(false), getColorsTUI(true));
}

void footBar(char* text){
    drawRect(0,getHeightScreen()-16,1024,getHeightScreen(),VESA_LIGHT_GREY);
    setPosX(0);
    setPosY(getHeightScreen()-16);
    puts_color(text,VESA_BLACK, VESA_LIGHT_GREY);
}

void updateLoop(){
    // Получаем последнию позицию курсора
    oldPosX = getPosX();
    oldPosY = getPosY();

    // Выводим нажатую кнопку во второй колонке в шапке
    //char* TIME = "10/10/2022 12:23";
    setPosX((getMaxStrLineTUI()-20)*8);
    setPosY(0);
    setColor(getColorsTUI(false));
    drawRect((getMaxStrLineTUI()-16)*8,0,20*8, 15, TUI_BASE_COLOR_HEAD);

    /* CAUSES PageFault
     * Оставлю до лучших времен, крашиться, причина не известна
     struct synapse_time* TIME = get_time();
    char hrw[3];
    char mnw[3];
    char scw[3];

    char dw[3];
    char mw[3];
    char yw[5];
    
    itoa(TIME->hours, hrw);
    itoa(TIME->minutes, mnw);
    itoa(TIME->seconds, scw);

    itoa(TIME->day, dw);
    itoa(TIME->month, mw);
    itoa(TIME->year, yw);

    char totaltime[48] = {0};
    strcpy(totaltime, dw);
    strcat(totaltime, "/");
    strcat(totaltime, mw);
    strcat(totaltime, "/");
    strcat(totaltime, yw);
    strcat(totaltime, " ");
    if(TIME->hours<10) strcat(totaltime, "0");
    strcat(totaltime, hrw);
    strcat(totaltime, ":");
    if(TIME->minutes<10) strcat(totaltime, "0");
    strcat(totaltime, mnw);
    strcat(totaltime, ":");
    if(TIME->seconds<10) strcat(totaltime, "0");
    strcat(totaltime, scw);

    puts_color(totaltime, getColorsTUI(false), TUI_BASE_COLOR_HEAD);

    */
    if (keyLastInset() != 0){
        qemu_printf("Last key ID: %d",keyLastInset());
    }
    // Возращаем указатель обратно
    setPosX(oldPosX);
    setPosY(oldPosY);
}

void menu0(bool items){
    if (items){
        setCurrentItemTUI(0);
        cleanItems();
        addItem("Info PC",true,"","");              // 0
        addItem("List PCI-Devices",true,"","");     // 1
        addItem("Application List",true,"","");     // 2
        addItem("Exit to Console",false,"","");     // 3
        addItem("Reboot",false,"","");              // 4
        addItem("Shutdown",false,"","");            // 5
    }
    curMenu = 0;
    bakMenu = -1;
    createMenuBox("Base menu:");

}

void menu1(bool items){
    if (items){
        setCurrentItemTUI(0);
        cleanItems();
        addItem("OS: SynapseOS v0.12.2 (Dev)",true,"","");
        addItem(format_string("CPU: %s","Test CPU"),true,"","");
        addItem(format_string("RAM: %d kb",12346789),true,"","");
        addItem((format_string("Display: %s",getDisplayName(),getWidthScreen(),getHeightScreen())),true,"","");     // 2
        addItem("Video: Basic video adapter (Unknown)",true,"","");
        addItem("Back",false,"","");
    }
    curMenu = 1;
    bakMenu = 0;
    createMenuBox("System Info");
}

void menuExit(){
    cleanItems();
    addItem("Yes, close TShell",false,"","");
    addItem("No, I don't want to log out of TShell",false,"","");
    curMenu = -2;
    bakMenu = -1;
    createMenuBox("Are you sure you want to quit TShell?");
}

void menuGen(bool items){
    uint32_t h = curMenu;
    if (h == 0){
        menu0(items);
    } else if (h == 1){
        menu1(items);
    } else if (h == -2){
        menuExit(items);
    }
}

uint32_t menuHandler(){
    uint32_t h = curMenu;
    uint32_t i = getCurrentItemTUI();
    qemu_printf("[menuHandler] Handler: %d | Item: %d\n",h,i);
    if (h == 0 && i == 0){
        menu1(true);
        return 0;
    } else if (h == 0 && i == 1){
        createErrorBox("Error in TShell. ","This functionality has not yet been implemented.");
        sleep_ms(3000);
        cleanWorkSpace(TUI_BASE_COLOR_BODY);
        curMenu = 0;
        menuGen(true);
        return 0;
    } else if (h == 0 && i == 2){
        createErrorBox("Error in TShell. ","This functionality has not yet been implemented.");
        sleep_ms(3000);
        cleanWorkSpace(TUI_BASE_COLOR_BODY);
        curMenu = 0;
        menuGen(true);
        return 0;
    } else if ((h == 0 && i == 3) || (h == -2 && i == 0)){
        return -1;
    } else if (h == -2 && i == 1){
        cleanWorkSpace(TUI_BASE_COLOR_BODY);
        setModeTUI(TUI_DEFAULT);
        return 0;
    } else if (h == 0 && i == 4){
        reboot();
        return 0;
    } else if (h == 0 && i == 5){
        shutdown();
        return 0;
    } else if (h == 1 && i == 5){
        menu0(true);
        return 0;
    }
    return 0;
}

int main(){
    tui_configurate();
	int32_t i = 0;
    setLastUpdateTUI(timer_get_ticks()+5);
    while(1){
        if (timer_get_ticks() < getLastUpdateTUI()){
            // Просто игнорируем работу цикла
            continue;
        }
        // Выполняем цикл
        updateLoop();
        if (getModeTUI() == TUI_ERROR_BOX){

        } else if (getModeTUI() == TUI_MENU_BOX){
             //changeStageKeyboard(0);
            if (keyLastInset() == KEY_ENTER){
                setLastUpdateTUI(timer_get_ticks()+3);
                uint32_t handler = menuHandler();
                if (handler == -1){
                    cleanScreen();
                    tui_destroy();
                    break;
                }
                continue;
            } else if (keyLastInset() == KEY_UP){
                setCurrentItemTUI(getCurrentItemTUI()-1);
                // Нажата клавиша вверх
                if (getCurrentItemTUI() < 0){
                    setCurrentItemTUI(getMaxListMenuTUI()-1);
                }
                menuGen(false);
            } else if (keyLastInset() == KEY_DOWN){
                // Нажата клавиша вниз
                setCurrentItemTUI(getCurrentItemTUI()+1);
                if (getCurrentItemTUI() >= getMaxListMenuTUI()){
                    setCurrentItemTUI(0);
                }
                menuGen(false);
            } else if (keyLastInset() == KEY_ESC){
                if (bakMenu != -1){
                    curMenu = bakMenu;
                    menuGen(true);
                } else {
                    cleanWorkSpace(TUI_BASE_COLOR_BODY);
                    setModeTUI(TUI_DEFAULT);
                }
                setLastUpdateTUI(timer_get_ticks()+5);
                continue;
            }
            setLastUpdateTUI(timer_get_ticks()+5);
            continue;

        } else if (keyLastInset() == KEY_ESC){
            curMenu = -2;
            menuGen(true);
            setLastUpdateTUI(timer_get_ticks()+5);
            continue;
        } else if (keyLastInset() == KEY_START || keyLastInset()== KEY_F1){
            //exec("/initrd/apps/hi",0,"");
            curMenu = 0;
            menuGen(true);
            continue;
            //break;
        } else if (keyLastInset() == KEY_F12){
            // Нажата клавиша F12 - закроем TUI и вернем управление shell()
            createErrorBox("Error in TUI module. ","You will be returned to the console in 1 seconds.");
            sleep_ms(1000);
            //bgColor = VESA_BLACK;
            cleanScreen();
            tui_destroy();
            break;
        } else {
            if (i == 0){
                i = 1;
                // Чистим экран
                cleanScreen();
                // Рисуем обводку
                drawRectLine(0,16*2,getWidthScreen(),getWorkSpaceHeightTUI()+32,VESA_WHITE,VESA_LIGHT_GREY,178);
                // Выводим шапку
                headBar();
                cleanWorkSpace(TUI_BASE_COLOR_BODY);

                // Выводим ноги
                footBar(" Press 'Start' to open menu");
                // Установка позиции для печати
                setPosX(8);
                setPosY(16*8);
                //changeStageKeyboard(1);
            }
            // Обновляем таймер, для переотображения
            setLastUpdateTUI(timer_get_ticks()+3);
        }
    }
	//

	return 0;
}
