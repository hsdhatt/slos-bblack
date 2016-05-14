#include <stdio.h>
#include <stdlib.h>
//#include "lz4.h"

#define COMPRESSION_MAGIC 0xFEEDFEED
#define BLOCK_SIZE 16*1024
struct compression_header {
	unsigned int magic;
	unsigned int compressed_size;
	unsigned int size;
};


void compress(FILE *fpin, FILE *fpout)
{
	char *src, *dest;
	struct compression_header hdr;
	int bytes_read, result;

	src = malloc(BLOCK_SIZE);
	dest = malloc(BLOCK_SIZE);

	result = 0;
	do {
		bytes_read = fread(src, sizeof(char), BLOCK_SIZE, fpin);
		
		hdr.size = bytes_read;
	
		result = LZ4_compress(src, dest, bytes_read);
		hdr.compressed_size = result;

		hdr.magic = COMPRESSION_MAGIC;
		fwrite(&hdr, 1, sizeof(hdr), fpout);
		fwrite(dest, 1, hdr.compressed_size, fpout);

	}while(bytes_read == BLOCK_SIZE);

	return;
	
} 
int main(int argc, char *argv[])
{
	FILE *fpin, *fpout;

	fpin = fopen("../image/slos.bin", "r");
	fpout = fopen("../image/slos-compressed", "w");

	compress(fpin, fpout);
	fclose(fpin);
	fclose(fpout);

	return 0;
}
