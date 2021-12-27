#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    if (argc != 2 ) {
        printf("Usage: milliVOLT *.vol\n");
        return(1);
    }

    FILE *f0 = fopen(argv[1], "rb");

    if (f0 == NULL) {
        printf("Unable to open VOLT archive!\n");
        return(1);
    }

    unsigned int magic;
    fread(&magic, sizeof(unsigned int), 1, f0);

    if (magic != 0x544C4F56 ) {
        printf("Missing VOLT header!\n");
        return(1);
    }

    fseek(f0, 0x04, SEEK_CUR);
    unsigned int numOfFiles;
    fread(&numOfFiles, sizeof(unsigned int), 1, f0);
    printf("Number of files: 0x%x\n", numOfFiles);
    unsigned int nameSecSize;
    fread(&nameSecSize, sizeof(unsigned int), 1, f0);
    printf("Name section size: 0x%x\n", nameSecSize);

    for (int i = 0; i < numOfFiles; i++) {
        fseek(f0, 0x08, SEEK_CUR);
        unsigned int nameOffset;
        fread(&nameOffset, sizeof(unsigned int), 1, f0);
        printf("Name offset: 0x%x\n", nameOffset);
        unsigned int getBack = ftell(f0);
        nameOffset += numOfFiles * 0x0C + 0x10;
        fseek(f0, nameOffset, SEEK_SET);
        unsigned int fileAddress;
        fread(&fileAddress, sizeof(unsigned int), 1, f0);
        printf("File address: 0x%x\n", fileAddress);
        fseek(f0, 0x04, SEEK_CUR);
        unsigned int fileSize;
        fread(&fileSize, sizeof(unsigned int), 1, f0);
        printf("File size: 0x%x\n", fileSize);
        fseek(f0, 0x04, SEEK_CUR);
        unsigned char nameChar = ' ';
        int nameSize = 0;

        while (nameChar != 0x00) {
            fread(&nameChar, sizeof(unsigned char), 1, f0);
            nameSize++;
        }

        char *nameBuffer = (char *) malloc(sizeof(char) * nameSize);
        fseek(f0, -nameSize, SEEK_CUR);
        fread(nameBuffer, sizeof(char), nameSize, f0);
        printf("Name: %s\n", nameBuffer);
        FILE *f1 = fopen(nameBuffer, "wb");
        free(nameBuffer);
        fseek(f0, fileAddress, SEEK_SET);
        char *fileBuffer = (char *) malloc(sizeof(char) * fileSize);
        fread(fileBuffer, sizeof(char), fileSize, f0);
        fwrite(fileBuffer, sizeof(char), fileSize, f1);
        free(fileBuffer);
        fclose(f1);
        fseek(f0, getBack, SEEK_SET);
    }

    fclose(f0);
    return(0);
}
