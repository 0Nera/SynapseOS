#pragma once
#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

typedef struct _FILE
{

    char name[32];
    uint32_t flags;
    uint32_t fileLength;
    uint32_t id;
    uint32_t eof;
    uint32_t position;
    uint32_t currentCluster;
    uint32_t deviceID;
    uint32_t fstream_ptr;
    uint32_t fsize;
} FILE, *PFILE;

FILE *fopen(const char *filename, const char *mode);
void fclose(FILE *stream);
int fseek(FILE *stream, int offset, int whence);
int ftell(FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
int fsize(FILE *stream);
void rewind(FILE *stream);
