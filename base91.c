#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BLOCO_SIZE 13
#define DESLOCAMENTO_ASCII_MAIUSCULO 65
#define DESLOCAMENTO_ASCII_MINUSCULO 71
#define DESLOCAMENTO_ASCII_ALGARISMOS -4
#define DESLOCAMENTO_ARRAY_CARACTER  62

#define A 65
#define Z 90
#define a 97
#define z 122
#define ZERO 48
#define NOVE 57
#define LENGTH_ARR_CHAR 29
//10 por linha
//codigos entre 62 e 90
char array_caracter[] = {'!', '#', '$', '>', '?', '@', '[', ']', '^', '_',
                           '`', '{', '|', '}', '~', '\'', '%', '&', '(', ')',
                           '*', '+', ',', '.', '/', ':' , ';', '<', '='};

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
    caracter = array_caracter[numero-DESLOCAMENTO_ARRAY_CARACTER];
  else {
    caracter = '?';
    printf("numero fora da tabela de base 91\n");
  }

  return caracter;
}

void leBloco(FILE *fp) {
  // unsigned char temp[2];
  int16_t x, temp, temp2, y1, y2;
  int i= 0;
  fseek(fp, 0, SEEK_SET);

  while(!feof(fp) && i < 4){
    //lendo 8bits
    fread(&x, sizeof(int16_t), 1, fp);
    //Na primeira vez 13 bits serão o X, os 3 outros lidos farão parte do proximo bloco
    if(i == 0) {
      printf("%02x\n", x);
      //exclui os 13 primeiros bits, sobra os 3 ultimos
      temp = x << 13;
      //exclui os 3 ultimos bits, sobra os 13 primeiros
      x = x >> 3;
      printf("%02x\n", x);
    }
    //depois da primeira leitura X
    else {
      //armazena os 3 ultimos bits
      temp2 = x << 13;
      //EXCLUI os 3 ultimos bits
      x = x >> 3;
      printf("1- %02x\n", x);

      //concatena x e temp da ultima iteração
      x = temp | x;
      printf("2-%02x\n", x);

      //
      temp = temp2;

    }
    //Achando o Y1 e Y2
    y1 = x/91;
    y2 = x%91;
    printf("%c\n", decode(y1));
    printf("%c\n", decode(y2));

    i++;
  }
}

int charToInt(char s) {
  int num;
  /* Transforma ascii entre a-z para int da base91.
  Obs.: 71 = a(ascii) - a(base91) */
  if((s >= a) && (s <= z)) {
    printf("aqui\n");
    num = s - 71;
  }
  /* Transforma ascii entre A-Z para int da base91.*/
  else if((s >= A) && (s <= Z)) {
    num = s - A;
  }
  /* Transforma ascii entre 0-9 para int da base91.
  4-> 52(zero base91) - 48(zero ascii)*/
  else if((s >= ZERO) && (s <= NOVE)) {
    num = s + 4;
  }
  /*Transforma os caracteres restantes em base91. 62 eh o int
  correspondente a posicao 0 do vetor array_caracter*/
  else {
    for(int i = 0; i < LENGTH_ARR_CHAR; i++) {
      if(s == array_caracter[i]){
        return i + 62;
      }
    }
    return -1;
  }
  return num;
}

//void salvarBin(bin,File){}

/* parametro-> arquivo com arquivo texto em base91
Ler um char de cada vez, deslocar o ponteiro para que na proxima
vez que executar essa funcao possa ler um char diferente*/
char leChar(FILE *fp) {
char lido;
fscanf(fp,"%c", &lido);
printf("Charlido: %c\n",lido);
  return lido;
}

/*
1-pego duas letras de um arquivo base91 de texto-> 1letra e 2letra
2-y1 = 1letra, y2 = 2letra
3-encontro o valor de x multiplicando y1 por 91 e somando com y2
4-transformo o inteiro x em um binario
5-Salvo o binario em um arquivo texto
*/
void base91tobinary(FILE *fp) {
  int y1, y2, x,vetorx[1000],i=0;
  char letra1, letra2;

 while(!feof(fp)) {

    letra1 = leChar(fp);
    letra2 = leChar(fp);

    y1 = charToInt(letra1);
    y2 = charToInt(letra2);

    x = (y1 * 91) + y2;
    printf("int base91: %d \n",x);
    vetorx[i]= x;
i=i+1;
 }
 fclose(fp);
 FILE* fpescrita = fopen("SAIDA_BASE91", "wb");
fwrite(vetorx,sizeof(vetorx[i]),1,fpescrita);
fclose(fpescrita);
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

  if(argv[1][0] == 'D'){
    leBloco(fp);
    fclose(fp);
  }
  else if(argv[1][0] == 'E'){
    base91tobinary(fp);
  }




  return 0;
}
