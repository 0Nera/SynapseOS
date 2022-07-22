 /*
    Автор: Пиминов Никита. Распространяется по лицензии GNU GPL 3.0 (pimnik98)
    Имя файла:  tui.c
    Описание:   Программа для теста TUI
*/
#include <kernel.h>
#include <io/tui.h>

void print_char(char ch)
{
  vga_buffer[vga_index_db] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index_db++;
}
void print_string(char *str)
{
  uint32_t index = 0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

uint32_t digit_count(uint8_t num)
{
    uint32_t count = 0;
    if(num == 0)
        return 1;
    while(num > 0){
        count++;
        num = num/10;
    }
    return count;
}

void print_color_string(char *str, uint8_t fore_color, uint8_t back_color)
{
  uint32_t index = 0;
  uint8_t fc, bc;
  fc = g_fore_color;
  bc = g_back_color;
  g_fore_color = fore_color;
  g_back_color = back_color;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
  g_fore_color = fc;
  g_back_color = bc;
}

void print_int(uint8_t num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_string(str_num);
}

uint16_t get_box_draw_char(uint8_t chn, uint8_t fore_color, uint8_t back_color)
{
  uint16_t ax = 0;
  uint8_t ah = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  ax |= chn;

  return ax;
}

void clear_vga_buffer(uint16_t **buffer, uint8_t fore_color, uint8_t back_color)
{
  uint32_t i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index_db = 0;
}

void gotoxy(uint16_t x, uint16_t y)
{
  vga_index_db = x_end*y;
  vga_index_db += x;
}

void draw_generic_box(uint16_t x, uint16_t y,
                      uint16_t width, uint16_t height,
                      uint8_t fore_color, uint8_t back_color,
                      uint8_t topleft_ch,
                      uint8_t topbottom_ch,
                      uint8_t topright_ch,
                      uint8_t leftrightside_ch,
                      uint8_t bottomleft_ch,
                      uint8_t bottomright_ch)
{
  uint32_t i;

  //increase vga_index_db to x & y location
  vga_index_db = x_end*y;
  vga_index_db += x;

  //draw top-left box character
  vga_buffer[vga_index_db] = get_box_draw_char(topleft_ch, fore_color, back_color);

  vga_index_db++;
  //draw box top characters, -
  for(i = 0; i < width; i++){
    vga_buffer[vga_index_db] = get_box_draw_char(topbottom_ch, fore_color, back_color);
    vga_index_db++;
  }

  //draw top-right box character
  vga_buffer[vga_index_db] = get_box_draw_char(topright_ch, fore_color, back_color);

  // increase y, for drawing next line
  y++;
  // goto next line
  vga_index_db = x_end*y;
  vga_index_db += x;

  //draw left and right sides of box
  for(i = 0; i < height; i++){
    //draw left side character
    vga_buffer[vga_index_db] = get_box_draw_char(leftrightside_ch, fore_color, back_color);
    vga_index_db++;
    //increase vga_index_db to the width of box
    vga_index_db += width;
    //draw right side character
    vga_buffer[vga_index_db] = get_box_draw_char(leftrightside_ch, fore_color, back_color);
    //goto next line
    y++;
    vga_index_db = x_end*y;
    vga_index_db += x;
  }
  //draw bottom-left box character
  vga_buffer[vga_index_db] = get_box_draw_char(bottomleft_ch, fore_color, back_color);
  vga_index_db++;
  //draw box bottom characters, -
  for(i = 0; i < width; i++){
    vga_buffer[vga_index_db] = get_box_draw_char(topbottom_ch, fore_color, back_color);
    vga_index_db++;
  }
  //draw bottom-right box character
  vga_buffer[vga_index_db] = get_box_draw_char(bottomright_ch, fore_color, back_color);

  vga_index_db = 0;
}

void draw_box(uint8_t boxtype,
              uint16_t x, uint16_t y,
              uint16_t width, uint16_t height,
              uint8_t fore_color, uint8_t back_color)
{
  switch(boxtype){
    case BOX_SINGLELINE :
      draw_generic_box(x, y, width, height,
                      fore_color, back_color,
                      218, 196, 191, 179, 192, 217);
      break;

    case BOX_DOUBLELINE :
      draw_generic_box(x, y, width, height,
                      fore_color, back_color,
                      201, 205, 187, 186, 200, 188);
      break;
  }
}

void fill_box(uint8_t ch, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
  uint32_t i,j;

  for(i = 0; i < height; i++){
    //increase vga_index_db_db to x & y location
    vga_index_db = x_end*y;
    vga_index_db += x;

    for(j = 0; j < width; j++){
      vga_buffer[vga_index_db] = get_box_draw_char(ch, 0, color);
      vga_index_db++;
    }
    y++;
  }
}

void init_vga(uint8_t fore_color, uint8_t back_color)
{
  vga_buffer = (uint16_t*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}


int tui(){

    const char*str = "Box Demo";


    gotoxy((x_end/2)-strlen(str), 1);
    print_color_string("Box Demo", VESA_WHITE, VESA_BLACK);

    draw_box(BOX_DOUBLELINE, 0, 0, 320, 240, VESA_LIGHT_GREEN, VESA_BLACK);

    draw_box(BOX_SINGLELINE, 5, 3, 20, 5, VESA_LIGHT_YELLOW, VESA_BLACK);
    gotoxy(10, 6);
    print_color_string("Hello World", VESA_LIGHT_RED, VESA_BLACK);

    // NULL for only to fill colors, or provide any other character
    fill_box(0, 36, 5, 30, 10, VESA_RED);

    fill_box(1, 6, 16, 30, 4, VESA_GREEN);
    draw_box(BOX_DOUBLELINE, 6, 16, 28, 3, VESA_BLUE, VESA_GREEN);
    return 0;                            // Программа завершена, надо вернуть 0 - все хорошо выполнено
}
