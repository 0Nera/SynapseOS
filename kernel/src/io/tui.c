#include <kernel.h>


int32_t DV_W = 1024;
int32_t DV_H = 768;
int32_t bgColor = VESA_BLUE;
int32_t txColor = VESA_WHITE;
int32_t TUIMode = 0; // Режим коррекции отображения на разных экранах
int32_t typeDisplay = 0;
char* Display;

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
    log("[testDisplay] %dx%d - %d pixels | %d | %s",w,h,pixels,typeDisplay,Display);
    tty_printf("[testDisplay] %dx%d - %d pixels | %d | %s \n",w,h,pixels,typeDisplay,Display);
}

void cleanScreen(){
    for (int x = 0; x != 1024; x++){ // Вывод синего цвета по середине
        for (int y = 0; y != 768; y++){
            set_pixel(x, y, bgColor);
        }
    }
}
/*
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

void drawRectLine(int x,int y,int w, int h,int color, int c){
    for (int _x = x; _x < x+w ; _x += 8){
        draw_vga_character(c, _x, y, txColor, color, true);
        draw_vga_character(c, _x, y+h-16, txColor, color, true);
    }
    for (int _y = y; _y < y+h; _y += 16){
        draw_vga_character(c, x, _y, txColor, color, true);
        draw_vga_character(c, x+w-8, _y, txColor, color, true);
    }
}

void footBar(int32_t y){
    drawRect(0,y-16,1024,y,VESA_LIGHT_GREY);
    setPosX(0);
    setPosY(y-16);
    tty_puts_color("Press 'Start' to open menu",VESA_BLACK, VESA_LIGHT_GREY);
}

bool tui(){
    int32_t w = getWidthScreen();
    int32_t h = getHeightScreen();
    testDisplay(w,h);
    if (typeDisplay != 1){
        tty_printf("Sorry, your screen extension is not supported by TUI.\nYou are redirected immediately to the console application of the operating system.\n");
        return false;
    }
    tty_printf("TUI Kerner Test...\n");
    sleep(30);
    int32_t i = 0;

    char* OSNAME = "SynapseOS v0.2.12 (Dev)";
    int32_t l_OSNAME = (w/2)-(strlen(OSNAME)*4);

    int32_t maxStrLine = (w/8)-2;
    while(1){
        if (i == 0){
            i = 1;
            // Чистим экран

            cleanScreen();
            // Рисуем обводку
            drawRectLine(0,0,w,h,VESA_LIGHT_GREY,178);
            // Выводим ноги
            footBar(h);
            // Выводим заголовок
            setPosX(l_OSNAME);
            setPosY(0);
            tty_puts_color(OSNAME,VESA_WHITE, VESA_LIGHT_GREY);
            // Рисуем треугольник для параметров
            drawRectLine(8,16,w/2,112,VESA_LIGHT_GREY,176);
            drawRectLine(w/2,16,w-32,112,VESA_LIGHT_GREY,176);

            setPosX(16);
            setPosY(32);
            char infoOS[512];
            substr(infoOS, "OS: SynOSx86DevBuild", 0, (maxStrLine/2)-2);
            tty_puts_color(infoOS,txColor, bgColor);

            setPosX(16);
            setPosY(48);
            char infoCPU[512];
            substr(infoCPU, "CPU: TEST CPU", 0, (maxStrLine/2)-2);
            tty_puts_color(infoCPU,txColor, bgColor);

            setPosX(16);
            setPosY(64);
            char infoRAM[512];
            substr(infoRAM, "RAM: 32 MB", 0, (maxStrLine/2)-2);
            tty_puts_color(infoRAM,txColor, bgColor);

            setPosX(16);
            setPosY(80);
            char infoVideo[512];
            substr(infoVideo, "Video: Unknown", 0, (maxStrLine/2)-2);
            tty_puts_color(infoVideo,txColor, bgColor);

            setPosX(16);
            setPosY(96);
            char infoDisplay[512] = "";
            strncpy(infoDisplay,"Display: ",512);
            strcat(infoDisplay,Display);
            tty_puts_color(infoDisplay,txColor, bgColor);

            setPosX(16);
            setPosY(128);
            tty_puts_color("Error in TUI module. You will be returned to the console in 10 seconds.",txColor, bgColor);
            sleep(1000);
            break;

        }




        //drawRect(0,0,1024,768,VESA_BLUE);

    }
    return true;
}
