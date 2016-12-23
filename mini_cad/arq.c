#include <stdio.h>

const double direcoes_x[16] = {1, 1, 1, 0.5, 0, -0.5, -1, -1, -1, -1, -1, -0.5, 0, 0.5, 1, 1};
const double direcoes_y[16] = {0, 0.5, 1, 1, 1, 1, 1, 0.5, 0, -0.5, -1, -1, -1, -1, -1, -0.5};
const double octantes_x[8] = {1, 0.707106781, 0, -0.707106781, -1, -0.707106781, 0, 0.707106781};	
const double octantes_y[8] = {0, 0.707106781, 1, 0.707106781, 0, -0.707106781, -1, -0.707106781};

int main (){
	FILE *arq;
	int c;
	int n = 0;

	arq = fopen("file.txt","r");
	if(arq == NULL) {
		perror("Erro, nao foi possivel abrir o arquivo\n");
		return(-1);
	}
	
	while ((c = fgetc(arq)) != EOF){
		printf("%c", c);
		n++;
	}
	
	printf ("bytes: %d", n);
	fclose(arq);
	return(0);
	
	// combinando dois bytes (o menos significativo primeiro)
	// int number = buf[0] | buf[1] << 8;
	// ou
	
	// #define bytes_to_u16(MSB,LSB) (((unsigned int) ((unsigned char) MSB)) & 255)<<8 | (((unsigned char) LSB)&255)
	// unsigned int port=bytes_to_u16(buf[1],buf[0]);
}