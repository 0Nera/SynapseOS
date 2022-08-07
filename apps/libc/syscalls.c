#include <syscalls.h>

extern uint32_t __get_MSP(void);

#define STDOUT_USART void
#define STDERR_USART void
#define STDIN_USART void

// errno
#undef errno
extern int errno;

/*
 Переменные среды - пустой список.
*/
char *__env[1] = { 0 };
char **environ = __env;


// Выход из программы
_Noreturn void exit(){

}


// Открытие файла
int open(const char *name, int flags, ...){

}

/*
// Чтение файла
int read(int file, char *ptr, int len){

}


// Запись в файл
int write(int file, char *ptr, int len){

}


// Закрытие файла
int close(int file){

}
*/

// Передача управления новому процессу
int execve(char *name, char **argv, char **env){

}


// Создание нового процесса
int fork(){

}


// Cостояние открытого файла
int fstat(int file, struct stat *st){

}


// Получить ID текущего процесса
int getpid(){

}


// Является ли файл терминалом
int isatty(int file){

}


// Послать сигнал процессу
int kill(int pid, int sig){

}


// Устанвить новое имя для существующего файла
int link(char *old, char *new){

}


// Установить позицию в файле
int lseek(int file, int ptr, int dir){

}


// Увеличить размер области данных, использутся для malloc
caddr_t sbrk(int incr){

}


// Состояние открытого файла
int stat(const char *file, struct stat *st){

}


//  Временная информация о процессе (сколько тиков: системных, процессорных и т.д.)
clock_t times(struct tms *buf){
	
}


// Удалить имя файла
int unlink(char *name){
	return vfs_rm(name)==1?0:-1;
}


// Ожидания дочерних процессов
int wait(int *status){

}
