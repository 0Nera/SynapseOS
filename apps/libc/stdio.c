#include <stdio.h>


/*
    Стандартная библиотека SynapseOS

    stdio - модуль ввода-вывода
*/

/*
    Регистры сисфункций
    eax - номер
    ebx - параметр 1
    edx - параметр 2
    ecx - параметр 3
    esi - параметр 4
    edi - параметр 5
    ebp - параметр 6

    Символьный код | Регистры
    a                eax, ax, al
    b                ebx, bx, bl
    c                ecx, cx, cl
    d                edx, dx, dl
    S                esi, si
    D                edi, di
*/

#define SC_CODE_puts            0
#define SC_CODE_getscancode     1
#define SC_CODE_getchar         2
#define SC_CODE_gets            3
#define SC_CODE_malloc          4
#define SC_CODE_free            5
#define SC_CODE_setdev          10
#define SC_CODE_readfile        11
#define SC_CODE_writefile       12
#define SC_CODE_exists          13
#define SC_CODE_filesize        14
#define SC_CODE_rand            20
#define SC_CODE_srand           21
#define SC_CODE_putpixel        32
#define SC_CODE_drawline        33 
#define SC_CODE_version         40
#define SC_CODE_ticks           42



int getscancode(){
    int result = 0;

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_getscancode)
                );

    return result;
}


char getchar(){
    char result = '\0';

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_getchar)
                );

    return result;
} 

char *gets() {
    char *result = "";

    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_gets)
                );

    return result;
} 


int getversion(){
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_version)
                );

    return result;
}


uint32_t timer_get_ticks() {
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_ticks)
                );

    return result;
}

int print_str(char str[]) {
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)                  // result = eax (после выполнения)
                : "a"(SC_CODE_puts),            // eax = SC_CODE_puts(0)
                  "b"(str)                      // ebx = str
                );

    return result;
}


uint64_t rand(){
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)
                : "a"(SC_CODE_rand)
                );

    return result;
}


uint64_t srand(uint32_t seed) {
    uint32_t result = 0;
 
    asm volatile("int $0x80" 
                : "=a"(result)                   // result = eax (после выполнения)
                : "a"(SC_CODE_srand),            // eax = SC_CODE_puts(0)
                  "b"(seed)                      // ebx = str
                );

    return result;
}


/*
    putint - вывод числа
*/
void putint(const int i) {
    char res[32];

    if (i < 0) {
        print_str("-");
    }

    itoa(i, res);
    print_str(res);
    
}



void puthex(unsigned int i) {
    char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned int n, d = 0x10000000;

    print_str("0x");

    while((i / d == 0) && (d >= 0x10)) d /= 0x10;

    n = i;

    while( d >= 0xF ) {
        print_str(&hex[n/d]);
        n = n % d;
        d /= 0x10;
    }

    print_str(&hex[n]);
}

void putchar(unsigned char ch) {
	printf("%c", ch);
}

void print(char *format, va_list args) {
    int i = 0;
    char temp[2] = {0};

    while (format[i]) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's':
                    print_str(va_arg(args, char*));
                    break;
                case 'c':
                    temp[0] = (char) va_arg(args, char*)[0];
                    print_str(temp);
                    break;
                case 'd':
                    putint(va_arg(args, int));
                    break;
                case 'i':
                    putint(va_arg(args, int));
                    break;
                case 'u':
                    putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    puthex(va_arg(args, uint32_t));
                    break;
                default:
                    temp[0] = format[i];
                    print_str(temp);
            }
        } else {
            temp[0] = format[i];
            print_str(temp);
        }
        i++;
    }
}


/*
    printf - форматированный вывод
*/
void printf(char *text, ...) {
    va_list args;
    va_start(args, text);
    print(text, args);
}

int vfs_read(const char *filename, int32_t offset, int32_t size, void *buf) {
	int ok = 0;
        asm volatile("int $0x80" 
                : "=a"(ok)
                : "a"(SC_CODE_readfile),
                  "b"(filename),
                  "c"(offset),
                  "d"(size),
                  "S"(buf)
                );
     return ok;
}

int vfs_exists(const char* filename) {
	int ok = 0;
	asm volatile("int $0x80"
			: "=a"(ok)
			: "a"(SC_CODE_exists),
			  "b"(filename)
			);
	return ok;
}

int vfs_get_size(const char* filename) {
	int ok = 0;
	asm volatile("int $0x80"
			: "=a"(ok)
			: "a"(SC_CODE_exists),
			  "b"(filename)
			);
	return ok;
}

uint32_t fmodecheck(const char* restrict mode){
	uint32_t fmode = 0;
	if (mode[0] == 'w') {
		if (mode[1] == 'x') { // wx
			fmode = O_WRITE | O_CREATE;
		} else if (mode[1] == 'b') {
			if (mode[2] == 'x') { // wbx
				fmode = O_WRITE | O_CREATE;
			} else if (mode[2] == '+') {
				if (mode[3] == 'x') { // wb+x
					fmode = O_WRITE | O_READ | O_CREATE;
				} else { // wb+
					fmode = O_WRITE | O_READ | O_CREATE | O_TRUNC;
				}
			} else { // wb
				fmode = O_WRITE | O_CREATE | O_TRUNC;
			}
		} else if (mode[1] == '+') {
			if (mode[2] == 'x') { // w+x
				fmode = O_WRITE | O_READ | O_CREATE;
			} else if (mode[2] == 'b') {
				if (mode[3] == 'x') { // w+bx
					fmode = O_WRITE | O_READ | O_CREATE;
				} else { // w+b
					fmode = O_WRITE | O_READ | O_CREATE | O_TRUNC;
				}
			} else { // w+
				fmode = O_WRITE | O_READ | O_CREATE | O_TRUNC;
			}
		} else { // w
			fmode = O_WRITE | O_CREATE | O_TRUNC;
		}
	} else if (mode[0] == 'r') {
		if (mode[1] == 'b') {
			if (mode[2] == '+') { // rb+
				fmode = O_READ | O_WRITE;
			} else { // rb
				fmode = O_READ;
			}
		} else if (mode[1] == '+') { // r+ r+b
			fmode = O_READ | O_WRITE;
		} else { // r
			fmode = O_READ;
		}
	} else if (mode[0] == 'a') {
		if (mode[1] == 'b') {
			if (mode[2] == '+') { // ab+
				fmode = O_WRITE | O_READ | O_APPEND | O_CREATE;
			} else { // ab
				fmode = O_WRITE | O_APPEND | O_CREATE;
			}
		} else if (mode[1] == '+') { // a+ a+b
			fmode = O_WRITE | O_READ | O_APPEND | O_CREATE;
		} else { // a
			fmode = O_WRITE | O_APPEND | O_CREATE;
		}
	}
	return fmode;
}

void fcheckerror(FILE* stream){
	if (!vfs_exists(stream->path)){
		stream->err = STDIO_ERR_NO_FOUND;
	} else if (stream->fmode == 0){
		stream->err = STDIO_ERR_MODE_ERROR;
	} else if (stream->size <= 0){
		stream->err = STDIO_ERR_SIZE;
	} else if (stream->open == 0){
		stream->err = STDIO_ERR_NO_OPEN;
	}
}

/*
 * Получить код ошибки
 * Если возращает 0, значит все ОК
 */
uint32_t ferror(FILE* stream){
	return stream->err;
}

/*
 * Вывести ошибку при обработки потока данных с пользовательским сообщением
 */
void perror(FILE* stream,char* s){
	switch(stream->err){
		case STDIO_ERR_NO_FOUND:{
			printf("%s: %s\n",s, "File no found");
			break;
		}
		case STDIO_ERR_MODE_ERROR:{
			printf("%s: %s\n",s, "Unknown operating mode");
			break;
		}
		case STDIO_ERR_SIZE:{
			printf("%s: %s\n",s, "The file size has a non-standard value.");
			break;
		}
		case STDIO_ERR_NO_OPEN:{
			printf("%s: %s\n",s, "The file has not been opened for work.");
			break;
		}
		default: {
			printf("%s: %s\n",s, "Unknown");
			break;
		}
	}
}

/*
 * Открыть файл для работы с ним
 */
FILE* fopen(const char* filename, const char* mode){
	FILE* file;
	// Получаем тип открытого файла
	int32_t fmode = fmodecheck(mode);
	if (!vfs_exists(filename) || fmode == 0){
		// Тип файла не определен или файл не найден
		file->err = (fmode == 0?2:1);
		file->path = filename;
		file->size = 0;
		file->open = 0;
		file->pos = -1;
		return file;
	}
	file->open = 1;										// Файл успешно открыт
	file->fmode = fmode;								// Режим работы с файлом
	file->size = vfs_get_size(filename);				// Размер файла
	file->path = filename;								// Полный путь к файлу
	file->bufSize = sizeof(char) * file->size;			// Размер буфера
	file->buf = (char*) malloc(file->bufSize);			// Сам буфер
	file->pos = 0;										// Установка указателя в самое начало
	file->err = 0;										// Ошибок в работе нет
	return file;
}

/*
 * Закрыть файл после работы с ним
 */
void fclose(FILE* stream){
	if (stream->open){
		free(stream->buf);
	}
}

/*
 * Получить размер файла в байтах
 */
int32_t fsize(FILE* stream){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
		return -1;
	} else {
		return stream->size;
	}
}

/*
 * Получить содержимое файла
 */
char* fread(FILE* stream){
	if (!stream->open || !vfs_exists(stream->path) || stream->size <= 0 || stream->fmode == 0){
		// Удалось ли открыть файл, существует ли файл, размер файла больше нуля и указан правильный режим для работы с файлом
		fcheckerror(stream);
		return "";
	}
	int32_t res = vfs_read(stream->path, 0, stream->size, stream->buf);
	(void)res;
	stream->buf[stream->size] = '\0';
	return stream->buf;
}

/*
 * Получить позицию в файле
 */
int64_t ftell(FILE* stream){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
		return -1;
	}
	return stream->pos;
}

/*
 * Установка позиции в потоке данных относительно текущей позиции
 * stream – указатель на управляющую таблицу потока данных.
 * offset – смещение позиции.
 * whence – точка отсчета смещения.
 */
int64_t fseek(FILE* stream, int64_t offset, uint32_t whence){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
		return -1;
	}
	int64_t lsk;
	if (whence == SEEK_CUR) {
		lsk = stream->pos;
	} else if (whence == SEEK_END) {
		lsk = stream->size;
	} else if (whence == SEEK_SET) {
		lsk = 0;
	} else {
		return -1;
	}
	if (lsk+offset > 0 && stream->size >=lsk+offset){
		stream->pos = lsk+offset;
	}
	return -1;
}

/*
 * Установка позиции в потоке данных
 */
void fsetpos(FILE* stream, int64_t pos){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
		stream->pos = 0;
	} else if (pos > 0 && stream->size >= pos){
		stream->pos = pos;
	}
	stream->pos = 0;
}

/*
 * Установка текущей позиции для чтения/записи в начало файла.
 */
void rewind(FILE* stream){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
	}
	stream->pos = 0;
}

/*
 * Получить позицию в файле
 */
int64_t fgetpos(FILE* stream){
	if (!stream->open || stream->size <= 0 || stream->fmode == 0){
		fcheckerror(stream);
		return -1;
	}
	return stream->pos;
}

void fdebuginfo(FILE* stream){
	printf("[fDebugInfo] Path: %s\n\tIsOpen: %d\n\tMode: %d\n\tSize: %d\n\tBuffer: %d\n\tPosition: %d\n\tError code: %d\n",stream->path,stream->open,stream->fmode,stream->size,stream->bufSize,stream->pos,stream->err);
}


size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream ){
	// Реализуем как будет готовы ATA-драйвера и FS
}


char* fgets (uint32_t n, FILE *stream){

}
