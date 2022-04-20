#include <stdio.h>
#include <ports.h>


int main(){
    unsigned char c;
	unsigned char a, b;
	char *drive_type[5] = {
		"no floppy drive",
		"360kb 5.25in floppy drive",
		"1.2mb 5.25in floppy drive",
		"720kb 3.5in",
		"1.44mb 3.5in",
		"2.88mb 3.5in" };

	outb(0x70, 0x10);
	c = inb(0x71);

	a = c >> 4; // get the high nibble
	b = c & 0xF; // get the low nibble by ANDing out the high nibble

	printf("\nFloppy drive A is an: %s", drive_type[a]);
	printf("\nFloppy drive B is an: %s\n", drive_type[b]);

    return 0;
}