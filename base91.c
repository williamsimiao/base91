#include <stdio.h>
#include <stdlib.h>

#define BLOCO_SIZE 13

void leBloco(FILE *fp) {
  // unsigned char temp[2];
  int16_t temp;
  int i= 0;
  fseek(fp, 0, SEEK_SET);

  while(!feof(fp)){
    //lendo 8bits
    fread(&temp, sizeof(int16_t), 1, fp);
    if(i == 1) {
      printf("%02x\n", temp);
      //16 - 3 = 13
      temp = temp >> 3;
      printf("%02x\n", temp);
    }

    i++;
  }
}

void escreveBloco(FILE *fp) {
  //usar putc
  //putc(c, fp);
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

int main(int argc, char *argv[]) {
  FILE *fp;

  if(argc != 3) {
    printf("O primeiro argumento deve ser D, para decode, ou E para encode\n");
    printf("O segundo argumento deve ser o nome do arquivo de entrada\n");
    return 0;
  }

  fp = open_file(argv[2]);

  if(argv[1][0] == 'D')
    leBloco(fp);
  else if(argv[1][0] == 'E')
    escreveBloco(fp);

  fclose(fp);

  return 0;
}
