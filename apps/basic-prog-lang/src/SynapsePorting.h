#ifndef SYNAPSE
#define SYNAPSE

#define strnicmp(str1, str2, count) strncmp(str1, str2, count)
#define stricmp(str1, str2) strcmp(str1, str2)
// WARNING: This should be a temorary solution, meanwhile we porting libc funcs ;)
//                                                               NDRAEY
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#endif
