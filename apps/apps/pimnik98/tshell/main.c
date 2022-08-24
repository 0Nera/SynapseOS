#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vesa.h>
#include <tui.h>
#include <time.h>

int32_t oldPosX = 0;                // Последние местоположение символа по X (место печати)
int32_t oldPosY = 0;                // Последние местоположение символа по Y (место печати)

void headBar(){
    drawLine(2,TUI_BASE_COLOR_HEAD);
    char* OSNAME = " SynapseOS v0.2.12 (Dev)";
    setPosX(0);
    setPosY(16*1);
    puts_color(OSNAME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);

    setColor(getColorsTUI(false));
    //SynTime TIME = get_time();
    char* TIME = "10/10/2022 12:23";
    setPosX((getMaxStrLineTUI()-16)*8);
    setPosY(16*1);
    drawRect((getMaxStrLineTUI()-16)*8,16,18*8, 16,TUI_BASE_COLOR_HEAD);
    //printf("%d/%d/%d %d:%d:%d", TIME.day, TIME.month, TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    //printf("%d/%d/%d %d:%d:%d", get_time()->day, get_time()->month, get_time()->year, get_time()->hours, get_time()->minutes, get_time()->seconds);
    puts_color(TIME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);
}

void headBarOld(){
    drawLine(1,TUI_BASE_COLOR_HEAD);
    char* OSNAME = " SynapseOS v0.2.12 (Dev)";
    setPosX(0);
    setPosY(16*1);
    puts_color(OSNAME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);

    char* TIME = "10/10/2022 12:23";
    setPosX((getMaxListMenuTUI()-15)*8);
    setPosY(16*1);
    puts_color(TIME,getColorsTUI(false), TUI_BASE_COLOR_HEAD);
    // Рисуем треугольник для параметров
    drawRectLine(8,32,getWidthScreen()/2,112,getColorsTUI(false),VESA_LIGHT_GREY,176);
    drawRectLine(getWidthScreen()/2,32,(getWidthScreen()/2)-8,112,getColorsTUI(false),VESA_LIGHT_GREY,176);


    setPosX(16);
    setPosY(16*3);
    char infoOS[512];
    substr(infoOS, format_string("OS: %s",OSNAME), 0, (getMaxListMenuTUI()/2)-2);
    puts_color(infoOS,getColorsTUI(false), getColorsTUI(true));

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

    struct synapse_time* TIME = get_time();
    //char* TIME = "10/10/2022 12:23";
    setPosX((getMaxStrLineTUI()-20)*8);
    setPosY(0);
    setColor(getColorsTUI(false));
    drawRect((getMaxStrLineTUI()-16)*8,0,20*8, 15, TUI_BASE_COLOR_HEAD);

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

    //#if CLOCK_FORMAT==1
    //printf("%d/%d/%d %d:%d:%d", TIME.day, TIME.month, TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    //printf("%d/%d/%d %d:%d:%d", get_time()->day, get_time()->month, get_time()->year, get_time()->hours, get_time()->minutes, get_time()->seconds);
    /* CAUSES PageFault
    #elif CLOCK_FORMAT==2
    printf("%d %s %d %d:%d:%d", TIME.day, months_list[TIME.month-1], TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    #endif

    */
    if (keyLastInset() != 0){
        qemu_printf("Last key ID: %d",keyLastInset());
    }
    // Возращаем указатель обратно
    setPosX(oldPosX);
    setPosY(oldPosY);
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
            // Режим обработки фатального окна
        } else if (getModeTUI() == TUI_MENU_BOX){
             //changeStageKeyboard(0);
            if (keyLastInset() == 72){
                setCurrentItemTUI(getCurrentItemTUI()-1);
                // Нажата клавиша вверх
                if (getCurrentItemTUI() < 0){
                    setCurrentItemTUI(getMaxListMenuTUI()-1);
                }
                createMenuBox("Base menu:");
            } else if (keyLastInset() == 80){
                // Нажата клавиша вниз
                setCurrentItemTUI(getCurrentItemTUI()+1);
                if (getCurrentItemTUI() >= getMaxListMenuTUI()){
                    setCurrentItemTUI(0);
                }
                createMenuBox("Base menu:");
            } else if (keyLastInset() == 1){
                // Нажата клавиша ESC
                cleanWorkSpace(TUI_BASE_COLOR_BODY);
                setModeTUI(TUI_DEFAULT);
            }
            setLastUpdateTUI(timer_get_ticks()+5);
            continue;
        } else if (keyLastInset() == 91 || keyLastInset()==59){ // Win key!
            cleanWorkSpace(TUI_BASE_COLOR_BODY);
            char* listMenu;
            listMenu[0] = "Info PC";
            listMenu[1] = "List PCI-Devices";
            listMenu[2] = "Show ErrorBox";
            listMenu[3] = "Exit to Console";
            listMenu[4] = "Reboot";
            listMenu[5] = "Shutdown";
            listMenu[6] = "6Info PC";
            listMenu[7] = "7List PCI-Devices";
            listMenu[8] = "8Show ErrorBox";
            listMenu[9] = "9Exit to Console";
            listMenu[10] = "10Reboot";
            listMenu[11] = "11Shutdown";
            setMaxItemTUI(12);
            createMenuBox("Base menu:");
            //createErrorBox("Error","Test Fatal Screen");
            //sleep_ms(100);
            //cleanWorkSpace(TUI_BASE_COLOR_BODY);
            //break;
        } else if (keyLastInset() == 88){
            // Нажата клавиша F12 - закроем TUI и вернем управление shell()
            createErrorBox("Error in TUI module. ","You will be returned to the console in 1 seconds.");
            sleep_ms(1000);
            //bgColor = VESA_BLACK;
            cleanScreen();
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
