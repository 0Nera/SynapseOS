#include <kernel.h>
FILE* fopen(const char* filename, const char* mode)
{

}

void fclose(FILE* stream){

}

int fseek(FILE* stream, int offset, int whence)
{
	switch(whence)
	{
		case SEEK_CUR:
				stream->fstream_ptr = offset;
			break;
		case SEEK_END:
				stream->fstream_ptr = stream->fsize - offset;
			break;
		case SEEK_SET:
				stream->fstream_ptr += offset;
			break;
		default:;
	}
	return 0;//stream->fstream_ptr;
}

int ftell(FILE* stream)
{
	return stream->fstream_ptr;
}

void rewind(FILE* stream)
{
	stream->fstream_ptr = 0;
}

int fsize(FILE* stream)
{
	return stream->fsize;
}

size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream){
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream ){
}
