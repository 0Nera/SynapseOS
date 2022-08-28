#include <kernel.h>

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
			tty_printf("%s: %s\n",s, "File no found");
			break;
		}
		case STDIO_ERR_MODE_ERROR:{
			tty_printf("%s: %s\n",s, "Unknown operating mode");
			break;
		}
		case STDIO_ERR_SIZE:{
			tty_printf("%s: %s\n",s, "The file size has a non-standard value.");
			break;
		}
		case STDIO_ERR_NO_OPEN:{
			tty_printf("%s: %s\n",s, "The file has not been opened for work.");
			break;
		}
		default: {
			tty_printf("%s: %s\n",s, "Unknown");
			break;
		}
	}
}

/*
 * Открыть файл для работы с ним
 */
FILE* fopen(const char* filename, const char* mode){
	FILE* file = NULL;
	// Получаем тип открытого файла
	int32_t fmode = fmodecheck(mode);
	if (!vfs_exists(filename) || fmode == 0){
		// Тип файла не определен или файл не найден
		file->err = (fmode == 0?2:1);
		file->path = (char*)filename;
		file->size = 0;
		file->open = 0;
		file->pos = -1;
		return file;
	}
	file->open = 1;										// Файл успешно открыт
	file->fmode = fmode;								// Режим работы с файлом
	file->size = vfs_get_size(filename);				// Размер файла
	file->path = (char*)filename;								// Полный путь к файлу
	file->bufSize = sizeof(char) * file->size;			// Размер буфера
	file->buf = (char*) kheap_malloc(file->bufSize);	// Сам буфер
	file->pos = 0;										// Установка указателя в самое начало
	file->err = 0;										// Ошибок в работе нет
	return file;
}

/*
 * Закрыть файл после работы с ним
 */
void fclose(FILE* stream){
	if (stream->open){
		kheap_free(stream->buf);
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
	tty_printf("[fDebugInfo] Path: %s\n\tIsOpen: %d\n\tMode: %d\n\tSize: %d\n\tBuffer: %d\n\tPosition: %d\n\tError code: %d\n",stream->path,stream->open,stream->fmode,stream->size,stream->bufSize,stream->pos,stream->err);
}


size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream ){
	// Реализуем как будет готовы ATA-драйвера и FS
	return -1;
}


char* fgets (uint32_t n, FILE *stream){
	return NULL;
}

