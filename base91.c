#include <stdio.h>
#include <stdlib.h>

#define BLOCO_SIZE 13

void leBloco(FILE *fp) {
  char temp;
  int i= 0;
  fseek(fp, 0, SEEK_SET);

  while(!feof(fp)){
    fread(&temp, sizeof(char), 1, fp);
    if(i==2)
      printf("leu: %02x\n", temp);
    i++;
  }
}

FILE* open_file(char* fileName) {
  FILE *fp;
  fp = fopen(fileName, "a+b");
  if(fp == NULL) {
    printf("Arquivo %s nao encontrado", fileName);
    exit(0);
  }
  return fp;
}

void escreveBloco(FILE *fp) {
  char c = '0';
  putc(c, fp);
}

int main(int argc, char *argv[]) {
  FILE *fp;

  if(argc != 2) {
    printf("O primeiro argumento deve ser D, para decode, ou E para encode\n");
    printf("O segundo argumento deve ser o nome do arquivo de entrada\n");
    return -1;
  }

  //fp = open_file(argv[1]);
  fp = fopen(argv[1], "a+b");

  leBloco(fp);
  //leBloco(fp);
  fclose(fp);


  return 0;
}
