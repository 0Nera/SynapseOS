#include "../../../include/vesa.h"
#include "../../../include/stdlib.h"
#include "../../../include/stdio.h"
#include "../../../include/stdfile.h"

struct ImageViewerMetadata {
	short width;
	short height;
};

int main() {
	char metadata[4];

	printf("File > ");
	char *fname = gets();
	printf("Opening %s...\n", fname);

	fread(fname, 0, 4, metadata);

	struct ImageViewerMetadata* meta = (struct ImageViewerMetadata*)metadata;
	printf("Image Width: %d\nImage Height: %d", meta->width, meta->height);
	
	return 0;
}
