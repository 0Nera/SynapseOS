/**
 * @file tui.c
 * @brief TUI для SynapseOS
 * @author Пиминов Никита (@piminov_remont) (github.com/pimnik98)
 * piminoff.ru
 */
#include <kernel.h>

#define CLOCK_FORMAT 2

int32_t bgColor = VESA_BLUE;        // Фон на экране
int32_t txColor = VESA_WHITE;       // Основной текст для печати на экране
int32_t TUIMode = TUI_DEFAULT;      // Режим TUI
int32_t typeDisplay = 0;            // Тип дисплея (?)
int32_t lastUpdate = 0;             // Последнее обновление экрана
int32_t w = 0;                      // Длина экрана
int32_t h = 0;                      // Высота экрана
int32_t ww = 0;                     // Длина рабочего места
int32_t wh = 0;                     // Высота рабочего места
int32_t maxStrLine = 0;             // Максимальное количество символов на линии
int32_t maxHeightLine = 0;          // Максимальное количество линий
int32_t oldPosX = 0;                // Последние местоположение символа по X (место печати)
int32_t oldPosY = 0;                // Последние местоположение символа по Y (место печати)
char* Display;                      // Название расширения монитора
int32_t currentMenu = 0;            // Текущие меню
int32_t currentList = 0;            // Текущая позиция на экране
char* listMenu[128];                 // Сам список меню
int32_t maxListMenu = 0;            // Максимальное количество элементов
uint32_t maxItemScreen = 0;         // Максимальное количество объектов на экране
uint32_t pageMenuCurrent = 0;        // Текущая страница
uint32_t pageMenuMax = 0;           // Максимальная страница

void testDisplay(int w, int h){
    int32_t pixels = w*h;
    int32_t tDsize = w/h;
    // Типы дисплеев:
    // Unknown  - 0
    // 4:3      - 1
    // 16:9     - 2
    // 16:10    - 3
    // 5:3      - 4
    // 5:4      - 5
    // 5:6      - 6
    // 22:15    - 7
    // 11:9     - 8
    // 8:3      - 9
    // 15:9     - 10
    // 2:1      - 11
    // 3:2      - 12
    // 25:16    - 13
    // 19:10    - 14
    // 21:9     - 15
    // 25:16    - 16
    // 43:18    - 17


    switch(pixels){
        case 76800:{
            // 320×240 (4:3)
            Display = "QVGA";
            typeDisplay = 1;
            break;
        }
        case 84480: {
            // 352×240 (22:15)
            Display = "SIF(MPEG1 SIF)";
            typeDisplay = 7;
            break;
        }
        case 101376: {
            // 352×288 (11:9)
            Display = "CIF(MPEG1 VideoCD)";
            typeDisplay = 8;
            break;
        }
        case 96000: {
            // 400×240 (5:3)
            Display = "WQVGA";
            typeDisplay = 4;
            break;
        }
        case 276480: {
            // 480×576 (5:6 - 12:10)
            Display = "[MPEG2 SV-CD]";
            typeDisplay = 6;
            break;
        }
        case 153600: {
            // 640×240 (8:3) или 320×480 (2:3)
            Display = "HVGA";
            typeDisplay = 9;
            break;
        }
        case 230400: {
            // 640×360 (16:9)
            Display = "nHD";
            typeDisplay = 2;
            break;
        }
        case 307200: {
            // 640×480 (4:3 - 12:9)
            Display = "VGA";
            typeDisplay = 1;
            break;
        }
        case 384000: {
            // 800×480 (5:3)
            Display = "WVGA";
            typeDisplay = 4;
            break;
        }
        case 480000: {
            // 800×600 (4:3)
            Display = "SVGA";
            typeDisplay = 1;
            break;
        }
        case 409920: {
            // 854×480 (16:9)
            Display = "FWVGA";
            typeDisplay = 2;
            break;
        }
        case 518400: {
            // 960×540 (16:9)
            Display = "qHD";
            typeDisplay = 2;
            break;
        }
        case 614400: {
            // 1024×600 (128:75 - 15:9)
            Display = "WSVGA";
            typeDisplay = 10;
            break;
        }
        case 786432: {
            // 1024×768 (4:3)
            Display = "XGA";
            typeDisplay = 1;
            break;
        }
        case 995328: {
            // 1152×864 (4:3)
            Display = "XGA+";
            typeDisplay = 1;
            break;
        }
        case 720000: {
            // 1200×600 (2:1)
            Display = "WXVGA";
            typeDisplay = 11;
            break;
        }
        case 921600: {
            // 1280×720 (16:9)
            Display = "HDV 720p (HD ready) (720p)";
            typeDisplay = 2;
            break;
        }
        case 983040: {
            // 1280×768 (5:3)
            Display = "WXGA";
            typeDisplay = 4;
            break;
        }
        case 1049088: {
            // 1366×768 (16:9)
            Display = "HD";
            typeDisplay = 2;
            break;
        }
        case 1310720: {
            // 1280×1024 (5:4)
            Display = "SXGA";
            typeDisplay = 5;
            break;
        }
        case 1622016: {
            // 1408×1152 (11:9)
            Display = "16CIF";
            typeDisplay = 8;
            break;
        }
        case 1296000: {
            // 1440×900 (8:5 - 16:10)
            Display = "WXGA+";
            typeDisplay = 3;
            break;
        }
        case 1470000: {
            // 1400×1050 (4:3)
            Display = "SXGA+";
            typeDisplay = 1;
            break;
        }
        case 15120000: {
            // 1400×10800 (4:3)
            Display = "HDV 1080i ";
            typeDisplay = 1;
            break;
        }
        case 1474560: {
            // 1536×960 (8:5 - 16:10)
            Display = "XJXGA";
            typeDisplay = 3;
            break;
        }
        case 1572864: {
            // 1536×1024 (3:2)
            Display = "WSXGA (x)";
            typeDisplay = 12;
            break;
        }
        case 1440000: {
            // 1600×900 (16:9)
            Display = "WXGA++";
            typeDisplay = 2;
            break;
        }
        case 1638400: {
            // 1600×1024 (25:16)
            Display = "WSXGA";
            typeDisplay = 13;
            break;
        }
        case 1920000: {
            // 1600×1200 (4:3)
            Display = "UXGA";
            typeDisplay = 1;
            break;
        }
        case 1764000: {
            // 1680×1050 (16:10)
            Display = "WSXGA+";
            typeDisplay = 3;
            break;
        }
        case 2073600: {
            // 1920×1080 (16:9)
            Display = "Full HD (FHD)";
            typeDisplay = 2;
            break;
        }
        case 2304000: {
            // 1920×1200 (8:5 - 16:10)
            Display = "WUXGA";
            typeDisplay = 3;
            break;
        }
        case 2111840: {
            // 2048×1080 (19:10)
            Display = "2K DCI (Cinema 2K)";
            typeDisplay = 14;
            break;
        }
        case 2359296: {
            // 2048×1152 (16:9)
            Display = "QWXGA";
            typeDisplay = 2;
            break;
        }
        case 2777088: {
            // 2048×1536 (4:3)
            Display = "QXGA";
            typeDisplay = 1;
            break;
        }
        case 2764800: {
            // 2560×1080 (64:27 (21:9))
            Display = "UWHD";
            typeDisplay = 15;
            break;
        }
        case 3686400: {
            // 2560×1440 (16:9)
            Display = "WQXGA (WQHD) (QHD)";
            typeDisplay = 2;
            break;
        }
        case 4096000: {
            // 2560×1600 (8:5 - 16:10)
            Display = "WQXGA";
            typeDisplay = 3;
            break;
        }
        case 5242880: {
            // 2560×2048 (5:4)
            Display = "QSXGA";
            typeDisplay = 5;
            break;
        }
        case 5760000: {
            // 3200×1800 (16:9)
            Display = "WXGA+";
            typeDisplay = 2;
            break;
        }
        case 6553600: {
            // 3200×2048 (25:16)
            Display = "WQSXGA";

            typeDisplay = 16;
            break;
        }
        case 7680000: {
            // 3200×2400 (4:3)
            Display = "QUXGA";
            typeDisplay = 1;
            break;
        }
        case 4953600: {
            // 3440×1440 (43:18)
            Display = "Ultra WQHD";
            typeDisplay = 17;
            break;
        }
        case 4838400: {
            // 3840×2400 (8:5 - 16:10)
            Display = "WQUXGA";
            typeDisplay = 3;
            break;
        }
        case 829440: {
            // 3840×2160 (16:9)
            Display = "4K Ultra HD 1 (4K UHD-1)";
            typeDisplay = 2;
            break;
        }
        case 8847360: {
            // 4096×2160 (19:10)
            Display = "4K DCI (Cinema 4K)";
            typeDisplay = 14;
            break;
        }
        case 14745600: {
            // 5120×2880 (16:9)
            Display = "5K / UHD +";
            typeDisplay = 2;
            break;
        }
        case 20971520: {
            // 5120×4096 (5:4)
            Display = "HSXGA";
            typeDisplay = 5;
            break;
        }
        case 26214400: {
            // 6400×4096 (25:16)
            Display = "WHSXGA";
            typeDisplay = 13;
            break;
        }
        case 30720000: {
            // 6400×4800 (4:3)
            Display = "HUXGA";
            typeDisplay = 1;
            break;
        }
        case 33177600: {
            // 7680×4320 (16:9)
            Display = "8K Ultra HD 2 (8K UHD-2)";
            typeDisplay = 2;
            break;
        }
        case 36864000: {
            // 7680×4800 (8:5, 16:10)
            Display = "WHUXGA";
            typeDisplay = 3;
            break;
        }
        case 58982400: {
            // 10240×5760 (16:9)
            Display = "10k";
            typeDisplay = 2;
            break;
        }
        case 74649600: {
            // 11520×6480 (16:9)
            Display = "12K";
            typeDisplay = 2;
            break;
        }
        default: {
            Display = "Unknown";
            typeDisplay = 0;
            break;
        }
    }
    //tty_printf("%d",framebuffer_width);
    qemu_log("[testDisplay] %dx%d - %d pixels | %d | %s",w,h,pixels,typeDisplay,Display);
    tty_printf("[testDisplay] %dx%d - %d pixels | %d | %s \n",w,h,pixels,typeDisplay,Display);
}

/**
 * Рисуем прямоугольник
 * x - Начальная координата X
 * y - Начальная координата y
 * w - Длина
 * h - Высота
 * color - цвет заливки
 */
void drawRect(int x,int y,int w, int h,int color){
    for (int _x = x; _x < x+w ; _x++){
        for (int _y = y; _y < y+h; _y++){
            set_pixel(_x, _y, color);
        }
    }
}

void drawRectLine(int x,int y,int w, int h,int color,int color2, int c){
    for (int _x = x; _x < x+w ; _x += 8){
        draw_vga_character(c, _x, y, color, color2, true);
        draw_vga_character(c, _x, y+h-16, color, color2, true);
    }
    for (int _y = y; _y < y+h; _y += 16){
        draw_vga_character(c, x, _y, color, color2, true);
        draw_vga_character(c, x+w-8, _y, color, color2, true);
    }
}

void drawLine(int y,int color){
    for (int _x = 0; _x < w ; _x += 8){
        draw_vga_character(0, _x, y*16, color, color, true);
    }
}

void cleanScreen(bool fast){
    if (fast){
        drawRect(0,0,w,h,bgColor);
    } else {
        for (int x = 0; x != w; x++){
            for (int y = 0; y != h; y++){
                set_pixel(x, y, bgColor);
            }
        }
    }
}

void headBar(){
    drawLine(1,TUI_BASE_COLOR_HEAD);
    char* OSNAME = " SynapseOS v0.2.12 (Dev)";
    setPosX(0);
    setPosY(16*1);
    tty_puts_color(OSNAME,txColor, TUI_BASE_COLOR_HEAD);

    tty_setcolor(txColor);
    struct synapse_time TIME = get_time();
    //char* TIME = "10/10/2022 12:23";
    setPosX((maxStrLine-18)*8);
    setPosY(16*1);
    drawRect((maxStrLine-18)*8,16,18*8, 16,TUI_BASE_COLOR_HEAD);
    tty_printf("%d/%d/%d %d:%d:%d", TIME.day, TIME.month, TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    //tty_puts_color(TIME,txColor, TUI_BASE_COLOR_HEAD);
}

void headBarOld(){
    drawLine(1,TUI_BASE_COLOR_HEAD);
    char* OSNAME = " SynapseOS v0.2.12 (Dev)";
    setPosX(0);
    setPosY(16*1);
    tty_puts_color(OSNAME,txColor, TUI_BASE_COLOR_HEAD);

    char* TIME = "10/10/2022 12:23";
    setPosX((maxStrLine-15)*8);
    setPosY(16*1);
    tty_puts_color(TIME,txColor, TUI_BASE_COLOR_HEAD);
    // Рисуем треугольник для параметров
    drawRectLine(8,32,w/2,112,txColor,VESA_LIGHT_GREY,176);
    drawRectLine(w/2,32,(w/2)-8,112,txColor,VESA_LIGHT_GREY,176);


    setPosX(16);
    setPosY(16*3);
    char infoOS[512];
    substr(infoOS, format_string("OS: %s",OSNAME), 0, (maxStrLine/2)-2);
    tty_puts_color(infoOS,txColor, bgColor);

    setPosX(16);
    setPosY(16*4);
    char infoCPU[512];
    substr(infoCPU, format_string("CPU: %s",getNameBrand()), 0, (maxStrLine/2)-2);
    tty_puts_color(infoCPU,txColor, bgColor);

    setPosX(16);
    setPosY(16*5);
    char infoRAM[512];
    substr(infoRAM, strcat(format_string("RAM: %d",(getInstalledRam()))," kb"), 0, (maxStrLine/2)-2);
    tty_puts_color(infoRAM,txColor, bgColor);

    setPosX(16);
    setPosY(16*6);
    char infoVideo[512];
    substr(infoVideo, "Video: Basic video adapter (Unknown)", 0, (maxStrLine/2)-2);
    tty_puts_color(infoVideo,txColor, bgColor);

    setPosX(16);
    setPosY(16*7);
    char infoDisplay[512];
    substr(infoDisplay, (format_string("Display: %s",Display,w,h)), 0, (maxStrLine/2)-2);
    tty_puts_color(infoDisplay,txColor, bgColor);
}

void footBar(char* text){
    drawRect(0,h-16,1024,h,VESA_LIGHT_GREY);
    setPosX(0);
    setPosY(h-16);
    tty_puts_color(text,VESA_BLACK, VESA_LIGHT_GREY);
}

void cleanWorkSpace(int color){
    drawRect(8,16*(3),ww,wh,color);
}

void createMenuBox(char* title){
    TUIMode = TUI_MENU_BOX;
    // Установка отступа экрана в длину (left/right)
    uint32_t padding_w = maxStrLine/4; // 320 - 10 символа; 1024 - 32 символа
    // Установка отступа экрана в высоту (up)
    uint32_t padding_h = maxHeightLine/4;
    // Получаем размеры коробки
    uint32_t boxWidth = ww-((padding_w*8)*2);
    uint32_t boxHeight = wh-((padding_h*16));      // ? Реализовал а зачем, забыл :)
    uint32_t maxListBox = boxHeight;
    // Получаем максимальное количество символов на строку в коробке
    uint32_t maxStrLineBox = (boxWidth/8)-4; // 60 - символов при 1024

    // Высота бокса для меню
    maxItemScreen = (maxListMenu >10?10:maxListMenu);

    // Установка максимальных страниц навигаций
    if (maxListMenu > 0){
        pageMenuMax = (maxListMenu/maxItemScreen)+1;
    } else {
        pageMenuMax = 1;
    }

    // Обрезаем заголовок
    substr(title, title, 0, maxStrLineBox);

    // Рисуем бокс и узорчики
    drawRect(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+maxItemScreen),TUI_BASE_COLOR_MAIN);
    drawRectLine(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+maxItemScreen),TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN,15);

    // Отображаем титл
    setPosX(((3+padding_w)*8));
    setPosY(16*((10)+padding_h));
    tty_puts_color(title,TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN);

    // Рисуем пункты меню
    for (int x = pageMenuCurrent*maxItemScreen; x < maxItemScreen; x++){
        setPosX(((3+padding_w)*8));
        setPosY(16*((12+x)+padding_h));
        substr(listMenu[x], listMenu[x], 0, maxStrLineBox);
        if (currentList == x){
            drawRect(((3+padding_w)*8),16*((12+x)+padding_h),maxStrLineBox*8, 16,TUI_BASE_COLOR_ITEM);
            tty_puts_color(listMenu[x],TUI_TEXT_COLOR_ITEM,TUI_BASE_COLOR_ITEM);
            // Помечаем что, он является текущим
        } else {
            tty_puts_color(listMenu[x],TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN);
        }
    }
}

/**
 * @brief Выводит фатальный красный блок
 */
void createErrorBox(char* title,char* text){
    // Переводим TUI в режим ERRORBOX
    //TUIMode = TUI_ERROR_BOX;
    // Установка отступа экрана в длину (left/right)
    uint32_t padding_w = maxStrLine/4; // 320 - 10 символа; 1024 - 32 символа
    // Установка отступа экрана в высоту (up)
    uint32_t padding_h = maxHeightLine/4;
    // Получаем размеры коробки
    uint32_t boxWidth = ww-((padding_w*8)*2);
    uint32_t boxHeight = wh-((padding_h));      // ? Реализовал а зачем, забыл :)
    // Получаем максимальное количество символов на строку в коробке
    uint32_t maxStrLineBox = (boxWidth/8)-4; // 60 - символов при 1024
    // maxHeightLine
    // Высота бокса для текса
    uint32_t lineHeight = 1;
    // Обрезаем строку до максимального лимита на строку
    substr(title, title, 0, maxStrLineBox);
    // Если текст длинее допустимой строки
    if (strlen(text) > maxStrLineBox){
        lineHeight = maxStrLineBox/strlen(text);
    }
    // Если на экран все не помещается
    if (maxHeightLine < lineHeight){
        lineHeight = maxHeightLine;
    }
    if (strlen(text) > maxStrLineBox){
        // Объявим переменную для деления текста
        char strings[lineHeight][maxStrLineBox+1];
        for (int i = 0;i < lineHeight;i++){
            substr(title, title, 0+(i*maxStrLineBox), ((i+1)*maxStrLineBox));
        }
    }
    // Рисуем коробку
    drawRect(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+lineHeight),TUI_BASE_COLOR_ERROR);
    drawRectLine(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+lineHeight),TUI_TEXT_COLOR_ERROR,TUI_BASE_COLOR_ERROR,19);
    // Показываем титл и считаем равнение по центру
    uint32_t centerTitle = (maxStrLineBox/2)-(strlen(title)/2);

    setPosX(((3+centerTitle+padding_w)*8));
    setPosY(16*((10)+padding_h));
    tty_puts_color(title,TUI_TEXT_COLOR_ERROR,TUI_BASE_COLOR_ERROR);

    // Рисуем буковки
    if (strlen(text) > maxStrLineBox){
        for (int i = 0;i < lineHeight;i++){
            //substr(title, title, 0+(i*maxStrLineBox), ((i+1)*maxStrLineBox));

        }
    } else {
        setPosX(((3+padding_w)*8));
        setPosY(16*((12)+padding_h));
        tty_puts_color(text,TUI_TEXT_COLOR_ERROR,TUI_BASE_COLOR_ERROR);
    }
}

void updateLoop(){
    // Получаем последнию позицию курсора
    oldPosX = getPosX();
    oldPosY = getPosY();

    // Выводим нажатую кнопку во второй колонке в шапке

     struct synapse_time TIME = get_time();
    //char* TIME = "10/10/2022 12:23";
    setPosX((maxStrLine-18)*8);
    setPosY(0);
    tty_setcolor(txColor);
    drawRect((maxStrLine-18)*8,0,20*8, 15,TUI_BASE_COLOR_HEAD);
    //#if CLOCK_FORMAT==1
    tty_printf("%d/%d/%d %d:%d:%d", TIME.day, TIME.month, TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    /* CAUSES PageFault
    #elif CLOCK_FORMAT==2
    tty_printf("%d %s %d %d:%d:%d", TIME.day, months_list[TIME.month-1], TIME.year, TIME.hours, TIME.minutes, TIME.seconds);
    #endif
    */
    //tty_printf("%d",keyLastInset());
    if (keyLastInset() != 0){
        qemu_log("Last key ID: %d",keyLastInset());
    }
    // Возращаем указатель обратно
    setPosX(oldPosX);
    setPosY(oldPosY);
}

bool tui(){
    int32_t i = 0;
    w = getWidthScreen();
    h = getHeightScreen();
    ww = w-16;
    wh = h-(80);
    maxHeightLine = wh/16;
    maxStrLine = (w/8)-2;
    changeStageKeyboard(2); // Блокируем нажатие и отображение кнопок
    testDisplay(w,h);       // Тестируем монитор на валидность (Скорее всего останеться только для детекта, когда резина будет готова)
    bgColor = TUI_BASE_COLOR_BODY;
    txColor = TUI_TEXT_COLOR_BODY;
    tty_printf("TUI Starting...\n");
    TUIMode = TUI_DEFAULT;
    lastUpdate = timer_get_ticks()+3;
    while(1){
        if (timer_get_ticks() < lastUpdate){
            // Просто игнорируем работу цикла
            continue;
        }
        // Выполняем цикл
        updateLoop();
        if (TUIMode == TUI_ERROR_BOX){
            // Режим обработки фатального окна
        } else if (TUIMode == TUI_MENU_BOX){
             changeStageKeyboard(0);
            if (keyLastInset() == 72){
                currentList--;
                // Нажата клавиша вверх
                if (currentList < 0){
                    currentList = maxListMenu-1;
                }
                createMenuBox("Base menu:");
            } else if (keyLastInset() == 80){
                // Нажата клавиша вниз
                currentList++;
                if (currentList >= maxListMenu){
                    currentList = 0;
                }
                createMenuBox("Base menu:");
            } else if (keyLastInset() == 1){
                // Нажата клавиша ESC
                cleanWorkSpace(TUI_BASE_COLOR_BODY);
                TUIMode = TUI_DEFAULT;
            }
            lastUpdate = timer_get_ticks()+5;
            continue;
        } else if (keyLastInset() == 91 || keyLastInset()==59){ // Win key!
            cleanWorkSpace(TUI_BASE_COLOR_BODY);
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
            maxListMenu = 12;
            createMenuBox("Base menu:");
            qemu_log("PAGE -> Items: %d/%d -> ScreenMax: %d",currentList,maxListMenu,maxItemScreen);
            //createErrorBox("Error","Test Fatal Screen");
            //sleep_ms(100);
            //cleanWorkSpace(TUI_BASE_COLOR_BODY);
            //break;
        } else if (keyLastInset() == 88){
            // Нажата клавиша F12 - закроем TUI и вернем управление shell()
            createErrorBox("Error in TUI module. ","You will be returned to the console in 1 seconds.");
            sleep_ms(1000);
            bgColor = VESA_BLACK;
            cleanScreen(true);
            break;
        } else {
            if (i == 0){
                i = 1;
                // Чистим экран
                cleanScreen(true);
                // Рисуем обводку
                //drawRectLine(0,0,w,h,txColor,VESA_LIGHT_GREY,178);
                // Выводим шапку
                headBar();
                cleanWorkSpace(TUI_BASE_COLOR_BODY);

                // Выводим ноги
                footBar(" Press 'Start' to open menu");
                // Установка позиции для печати
                setPosX(8);
                setPosY(16*8);
                changeStageKeyboard(1);
            }

            // Обновляем таймер, для переотображения
            lastUpdate = timer_get_ticks()+3;
        }
    }
    return true;
}
