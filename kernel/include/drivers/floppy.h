#pragma once

// Порты ввода-вывода
#define FDC_DOR (0x3f2)     // Цифровой выходной регистр
#define FDC_MSR (0x3f4)     // Основной регистр статуса (ввод)
#define FDC_DRS (0x3f4)     // Регистр выбора скорости передачи данных (вывод)
#define FDC_DATA (0x3f5)    // Регистр данных
#define FDC_DIR (0x3f7)     // Регистр цифрового ввода (ввод)
#define FDC_CCR (0x3f7)     // Регистр управления конфигурацией (вывод)
#define CMD_SENSEI (0x08)   // Статус прерывания