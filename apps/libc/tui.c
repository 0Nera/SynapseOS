/**
 * @file apps/tui.c
 * @authors Пиминов Никита (github.com/pimnik98 | VK: @piminov_remont)
 * @brief Прослойка для работы с текстовым графическим редактором
 * @version 0.0.3
 * @date 2022-08-24
 *
 * @copyright Copyright Пиминов Никита (с) 2022
 *
 */

#include <tui.h>
int32_t bgColor = VESA_BLUE;        // Фон на экране
int32_t txColor = VESA_WHITE;       // Основной текст для печати на экране
int32_t TUIMode = TUI_DEFAULT;      // Режим TUI
int32_t typeDisplay = 0;            // Тип дисплея (?)
int32_t lastUpdate = 0;             // Последнее обновление экрана
int32_t w = 0;                      // Длина рабочего места
int32_t h = 0;                      // Длина рабочего места
int32_t ww = 0;                     // Длина рабочего места
int32_t wh = 0;                     // Высота рабочего места
int32_t maxStrLine = 0;             // Максимальное количество символов на линии
int32_t maxHeightLine = 0;          // Максимальное количество линий
char* Display;                      // Название расширения монитора
int32_t currentMenu = 0;            // Текущие меню
int32_t currentList = 0;            // Текущая позиция на экране
char *listMenu[128];                // Сам список меню
int32_t maxListMenu = 0;            // Максимальное количество элементов
uint32_t maxItemScreen = 0;         // Максимальное количество объектов на экране
uint32_t pageMenuCurrent = 0;       // Текущая страница
uint32_t pageMenuMax = 0;           // Максимальная страница
uint32_t ItemsMax   = 256;
uint32_t Items[256];
/**
 * @brief Получить цвет TUI
 *
 * @params bool bg - Если true, то фон, в противном случае основной цвет
 */
int32_t getColorsTUI(bool bg){
    return (bg?bgColor:txColor);
}

/**
 * @brief Получить текущий режим меню
 *
 * @return int32_t - Выводит ID статуса
 */
int32_t getModeTUI(){
    return TUIMode;
}

/**
 * @brief Получить длину рабочего пространства
 *
 * @return int32_t - Длина пространства
 */
int32_t getWorkSpaceWidthTUI(){
    return ww;
}

/**
 * @brief Получить высоту рабочего пространств
 *
 * @return int32_t - Высота пространства
 */
int32_t getWorkSpaceHeightTUI(){
    return wh;
}

/**
 * @brief Установить режим работы TUI
 *
 * @param int32_t mode - Режим TUI
 */
int32_t setModeTUI(int32_t mode){
    TUIMode = mode;
}

/**
 * @brief Получить последнее время обновления TUI
 *
 * @return int32_t - Выводит последнее время обновления
 */
int32_t getLastUpdateTUI(){
    return lastUpdate;
}

/**
 * @brief Установить время обновления TUI
 *
 * @param int32_t time - Время (в тиках)
 */
int32_t setLastUpdateTUI(int32_t time){
    lastUpdate = time;
}

/**
 * @brief Получить текущию позицию в меню
 *
 * @return int32_t - Выводит позицию в меню
 */
int32_t getCurrentItemTUI(){
    return currentList;
}

/**
 * @brief Установить позицию в меню
 *
 * @param int32_t item - Позиция в меню
 */
int32_t setCurrentItemTUI(int32_t item){
    currentList = item;
}

/**
 * @brief Установить максимальное количество элементов в меню
 *
 * @param int32_t item - Максимальное кол-во элементов
 */
int32_t setMaxItemTUI(int32_t item){
    maxListMenu = item;
}

/**
 * @brief Получить максимальное количество элементов в меню
 *
 * @return int32_t - Выводит кол-во элементов
 */
int32_t getMaxListMenuTUI(){
    return maxListMenu;
}

/**
 * @brief Получить максимальное символов в одной строке
 *
 * @return int32_t - Выводит кол-во символов на одну строку экрана
 */
int32_t getMaxStrLineTUI(){
    return maxStrLine;
}

/**
 * @brief Выводит фатальный красный блок
 *
 * @return char* - Выводит название текущего расширения экрана
 */
char* getDisplayName(){
    return Display;
}

/**
 * @brief Инициализация параметров дисплея
 */
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
    //qemu_log("[testDisplay] %dx%d - %d pixels | %d | %s",w,h,pixels,typeDisplay,Display);
    //tty_printf("[testDisplay] %dx%d - %d pixels | %d | %s \n",w,h,pixels,typeDisplay,Display);
}

/**
 * @brief Отчищает список элементов
 */
void cleanItems(){
    char listMenu[128];
    maxListMenu = 0;
}

uint32_t getMaxStrLineBoxTUI(){
    return (((ww-(((maxStrLine/4)*8)*2))/8)-4);
}

/**
 * @brief Добавляет позицию в список элементов
 *
 * @param char* name - Название позиции
 */

ItemTUI* addItem(char* name,bool disabled, char* key, char* value){
    if (maxListMenu > ItemsMax){
        //return false;
    }
    uint32_t i = maxListMenu;

    /**
    Items[i]->id = i;
    Items[i]->name = (char*) malloc(sizeof(char)*strlen(name));
    Items[i]->disabled = disabled;
    Items[i]->key = (char*) malloc(strlen(key));
    Items[i]->value = (char*) malloc(strlen(value));
    */
    if (strlen(name) > getMaxStrLineBoxTUI()){
        qemu_printf("[Name: %s | size: %d | msl: %d]",name,strlen(name),getMaxStrLineBoxTUI());
        substr(name, name, 0, getMaxStrLineBoxTUI());
    }
    listMenu[i] = name;
    maxListMenu++;
    return true;
}

ItemTUI* getItem(uint32_t id){
    return Items[id];
}

/**
 * @brief Отчистить пользовательское пространство
 *
 * @param int - Цвет для фона
 */
void cleanWorkSpace(int color){
    drawRect(8,16*(3),ww,wh,color);
}
/**
 * @brief Выводит список меню
 *
 * @param title - Заголовок
 */
void createMenuBox(char* title){
    TUIMode = TUI_MENU_BOX;
    // Установка отступа экрана в длину (left/right)
    uint32_t padding_w = maxStrLine/4; // 320 - 10 символа; 1024 - 32 символа
    // Установка отступа экрана в высоту (up)
    uint32_t padding_h = maxHeightLine/4;
    // Получаем размеры коробки
    uint32_t boxWidth = ww-((padding_w*8)*2);
    uint32_t boxHeight = wh-((padding_h*16));
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
    if (strlen(title) > maxStrLineBox){
        substr(title, title, 0, maxStrLineBox);
    }
    // Рисуем бокс и узорчики
    drawRect(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+maxItemScreen),TUI_BASE_COLOR_MAIN);
    drawRectLine(8+(padding_w*8),16*(8+padding_h),boxWidth,16*(6+maxItemScreen),TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN,15);

    // Отображаем титл
    setPosX(((3+padding_w)*8));
    setPosY(16*((10)+padding_h));
    puts_color(title,TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN);

    // Рисуем пункты меню
    for (int x = pageMenuCurrent*maxItemScreen; x < maxItemScreen; x++){
        setPosX(((3+padding_w)*8));
        setPosY(16*((12+x)+padding_h));
        if (currentList == x){
            drawRect(((3+padding_w)*8),16*((12+x)+padding_h),maxStrLineBox*8, 16,TUI_BASE_COLOR_ITEM);
            puts_color(listMenu[x],TUI_TEXT_COLOR_ITEM,TUI_BASE_COLOR_ITEM);
            // Помечаем что, он является текущим
        } else {
            puts_color(listMenu[x],TUI_TEXT_COLOR_BODY,TUI_BASE_COLOR_MAIN);
        }
    }
}

/**
 * @brief Выводит фатальный красный блок
 *
 * @param title - Заголовок
 * @param text  - Текст ошибки.
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
    puts_color(title,TUI_TEXT_COLOR_ERROR,TUI_BASE_COLOR_ERROR);

    // Рисуем буковки
    if (strlen(text) > maxStrLineBox){
        for (int i = 0;i < lineHeight;i++){
            //substr(title, title, 0+(i*maxStrLineBox), ((i+1)*maxStrLineBox));

        }
    } else {
        setPosX(((3+padding_w)*8));
        setPosY(16*((12)+padding_h));
        puts_color(text,TUI_TEXT_COLOR_ERROR,TUI_BASE_COLOR_ERROR);
    }
}



/**
 * @brief Инициализация и сброс TUI на стандартное значение
 */
void tui_configurate(){
    qemu_printf("%s","TUI Configure");
    w = getWidthScreen();
    h = getHeightScreen();
    ww = w-16;
    wh = h-(80);
    maxHeightLine = wh/16;
    maxStrLine = (w/8)-2;
    changeStageKeyboard(0); // Блокируем нажатие и отображение кнопок
    testDisplay(w,h);       // Тестируем монитор на валидность (Скорее всего останеться только для детекта, когда резина будет готова)
    bgColor = TUI_BASE_COLOR_BODY;
    txColor = TUI_TEXT_COLOR_BODY;
    TUIMode = TUI_DEFAULT;
    lastUpdate = timer_get_ticks()+3;

    /*
    th.BASE_COLOR_HEAD  = TUI_BASE_COLOR_HEAD;
    th.BASE_COLOR_BODY  = TUI_BASE_COLOR_BODY;
    th.BASE_COLOR_MAIN  = TUI_BASE_COLOR_MAIN;
    th.BASE_COLOR_ITEM  = TUI_BASE_COLOR_ITEM;
    th.BASE_COLOR_FOOT  = TUI_BASE_COLOR_FOOT;
    th.BASE_COLOR_ERROR = TUI_BASE_COLOR_ERROR;
    th.TEXT_COLOR_HEAD  = TUI_TEXT_COLOR_HEAD;
    th.TEXT_COLOR_BODY  = TUI_TEXT_COLOR_BODY;
    th.TEXT_COLOR_ITEM  = TUI_TEXT_COLOR_ITEM;
    th.TEXT_COLOR_FOOT  = TUI_TEXT_COLOR_FOOT;
    th.TEXT_COLOR_ERROR = TUI_BASE_COLOR_ERROR;
    */
}

void tui_destroy(){
    changeStageKeyboard(1);
    setPosX(0);
    setPosY(0);
}
