#include <stdio.h>
#include <stdlib.h>

#define COMPRESSION_MAGIC 0xFEEDFEED
#define BLOCK_SIZE 16*1024

struct compression_header {
	unsigned int magic;
	unsigned int compressed_size;
	unsigned int size;
};


void decompress(FILE *fpin, FILE *fpout)
{
	char *src, *dest;
	struct compression_header hdr;
	int bytes_read, result;

	result = 0;
	while(1) {
		fread(&hdr, sizeof(char), sizeof(hdr), fpin);

		if (hdr.magic != COMPRESSION_MAGIC)
			break;
	
		src = malloc(hdr.compressed_size);
		dest = malloc(hdr.size);

		bytes_read = fread(src, sizeof(char), hdr.compressed_size, fpin);
		if (bytes_read == 0) {
			free(src);
			free(dest);
			break;
		}
		printf("bytes read %d\n", bytes_read);
		
	
		result = LZ4_decompress_safe(src, dest, hdr.compressed_size, hdr.size);

		printf("result is %d\n", result);
		fwrite(dest, 1, hdr.size, fpout);
		free(src);
		free(dest);
	}

	return;
	
} 
int main(int argc, char *argv[])
{
	FILE *fpin, *fpout;

	fpin = fopen("../image/slos-compressed", "r");
	fpout = fopen("../image/slos-decompressed", "w");

	decompress(fpin, fpout);
	fclose(fpin);
	fclose(fpout);

	return 0;
}
