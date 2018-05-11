#include <stdio.h>
#include <stdlib.h>

#define BLOCO_SIZE 13
#define DESLOCAMENTO_ASCII_MAIUSCULO 65
#define DESLOCAMENTO_ASCII_MINUSCULO 71
#define DESLOCAMENTO_ASCII_ALGARISMOS -4

char decode(int16_t numero) {
  //letras maiusculas
  char caracter;
  if(numero >= 0 && numero <= 25)
    caracter = numero+DESLOCAMENTO_ASCII_MAIUSCULO;
  //letras minusclas
  else if(numero >= 26 && numero <= 51)
    caracter = numero+DESLOCAMENTO_ASCII_MINUSCULO;
  //algarismos
  else if(numero >= 52 && numero <= 61)
    caracter = numero+DESLOCAMENTO_ASCII_ALGARISMOS;
    //Simbolos
  else if(numero >= 53 && numero <= 90)
    //por enquanto printar qualquer simbolo, visto que a ordem da base 91
    //não é a mesma da tabela ASCII
    caracter = '!';
  else
    caracter = '?';
    printf("numero fora da tabela de base 91\n");

  return caracter;
}

void leBloco(FILE *fp) {
  // unsigned char temp[2];
  int16_t x, temp, temp2, y1, y2;
  int i= 0;
  fseek(fp, 0, SEEK_SET);

  while(!feof(fp)){
    //lendo 8bits
    fread(&x, sizeof(int16_t), 1, fp);
    //Na primeira vez 13 bits serão o X os 3 outros lidos farão parte do proximo bloco
    if(i == 0) {
      printf("%02x\n", x);
      //exclui os 13 primeiros bits
      temp = x << 13;
      //exclui os 3 ultimos bits
      x = x >> 3;
      printf("%02x\n", x);

      //Achando o Y1 e Y2
      y1 = x/91;
      y2 = x%91;
      printf("%c\n", decode(y1));
      printf("%c\n", decode(y2));

    }
    //depois da primeira leitura X
    else {
      //armazena os 3 ultimos bits
      temp2 = x >> 3;
      //EXCLUI os 3 ultimos bits
      x = x >> 3;
      //concatena x e temp da ultima iteração
      x = temp & x;
      //
      temp = temp2;

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
