#include "../../../include/vesa.h"
#include "../../../include/stdlib.h"
#include "../../../include/stdio.h"
#include "../../../include/stdfile.h"

int main() {
	char metadata[4];

	printf("File > ");
	char fname = getchar(); // Doesn't work
	printf("Opening %s...\n", fname);
	
	return 0;
}
