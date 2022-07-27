#include "../../../include/vesa.h"
#include "../../../include/stdlib.h"
#include "../../../include/stdio.h"

int main() {
	char metadata[4];

	printf("Warning! Keyboard doesn't work!\nFile > ");
	while(1) {
		char *fname = gets();
		printf("%s\n", fname);
	}
	//printf("Opening %s...\n", fname);
	
	return 0;
}
