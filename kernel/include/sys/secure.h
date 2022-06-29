#pragma once

void secure_init(uint8_t level);         // Функция инициализации модуля защиты
bool secure_check_elf(const char *name); // Функция проверки исполняемого файла