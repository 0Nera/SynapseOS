#include <stdio.h>

int main(int argc, char** argv) {
	printf("We have %d arguments\n", argc);
	for(int i=0; i<argc; i++) {
		printf("|--- %s\n", argv[argc]);
	}
	return 0;
}
