#include <stdint.h>
#pragma once
#include <stdio.h>
#include <string.h>

#define TUI_DEFAULT     0
#define TUI_ERROR_BOX   1
#define TUI_MENU_BOX    2

#define TUI_BASE_COLOR_HEAD     0xBABABA
#define TUI_BASE_COLOR_BODY     0xD6D2D0
#define TUI_BASE_COLOR_MAIN     0xFFFFFF
#define TUI_BASE_COLOR_ITEM     0x43ACE8
#define TUI_BASE_COLOR_FOOT     0xBABABA
#define TUI_BASE_COLOR_ERROR    0x8B4444

#define TUI_TEXT_COLOR_HEAD     0x000000
#define TUI_TEXT_COLOR_BODY     0x000000
#define TUI_TEXT_COLOR_ITEM     0xFFFFFF
#define TUI_TEXT_COLOR_FOOT     0x000000
#define TUI_TEXT_COLOR_ERROR    0xEBE7E7

typedef struct s_tui_theme {
    uint32_t BASE_COLOR_HEAD;
    uint32_t BASE_COLOR_BODY;
    uint32_t BASE_COLOR_MAIN;
    uint32_t BASE_COLOR_ITEM;
    uint32_t BASE_COLOR_FOOT;
    uint32_t BASE_COLOR_ERROR;
    uint32_t TEXT_COLOR_HEAD;
    uint32_t TEXT_COLOR_BODY;
    uint32_t TEXT_COLOR_ITEM;
    uint32_t TEXT_COLOR_FOOT;
    uint32_t TEXT_COLOR_ERROR;
} ThemeTUI;

typedef struct s_tui_item {
    uint32_t id;        // Позиция
    char* name;         // Наименование пункта
    bool disabled;      // Выключен ли?
    char* key;          // Ключ, если требуется
    char* value;        // Данные, если требуется
} ItemTUI;

