#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kbhit.c"
#include "gconio.h"
#include <unistd.h>

#define N_LINHAS 20
#define N_COLUNAS 3*N_LINHAS
#define S_FOOD 'o'
#define S_BODY 'x'
#define __ESPACO ' '
#define CR_INICIAL 10
#define LR_INICIAL CR_INICIAL
#define COMP_INICIAL 4
#define CC_INICIAL CR_INICIAL+COMP_INICIAL
#define LC_INICIAL LR_INICIAL

#define DIREITA 'd'
#define ESQUERDA 'e'
#define PCIMA 'c'
#define PBAIXO 'b'

typedef struct{
	int x;
	int y;
}posicao;

void Sleep(time_t delay);
void limpatela();
void vaipara(int x, int y);
//void init();
void init(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], int comprimento);
//void set_snake(char tela[N_LINHAS][N_COLUNAS], int pos[4], char direcao_atual);
int set_snake(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], char direcao_atual, int comprimento);

int main(){
	char tela[N_LINHAS][N_COLUNAS];
	int l, c, comprimento = COMP_INICIAL, qVidas = 3, pontos = 0;
	clock_t timer;
	char direcao_atual = DIREITA;
	char direcao_anterior = 0, dir;
	int pos[4] = {LR_INICIAL, CR_INICIAL, LC_INICIAL, CC_INICIAL}, i = 0;
	posicao alimento;
	posicao cobra[100];

	for(l=0;l<100;l++){
		cobra[l].x = -1;
		cobra[l].y = -1;
	}
	srand(time(NULL));
	/*for(l=0;l<N_LINHAS;l++){
		for(c=0;c<N_COLUNAS;c++){
			if(l==0 || l==N_LINHAS-1){
				tela[l][c] = '_';
			}else if(c==0 || c==N_COLUNAS-1){
				tela[l][c] = '|';
			}else{
				tela[l][c] = __ESPACO;
			}
		}
	}*/
	init(tela, cobra, comprimento);
	//tela[alimento.x][alimento.y] = __ESPACO;
	alimento.x = CC_INICIAL+1;//1+rand()%N_LINHAS-2;
	alimento.y = 1+rand()%N_COLUNAS-3;
	tela[alimento.x][alimento.y] = S_FOOD;
	do{
		/*l = set_snake(tela, cobra, direcao_atual, comprimento);
		if(l){
			direcao_atual = DIREITA;
			init(tela, cobra, comprimento);
			qVidas--;
			if(qVidas<0){
				printf("GAME OVER\n");
				printf("COMPRIMENTO FINAL: %d\n", comprimento);
				break;
			}
		}*/
		limpatela();
		printf("| Vidas: %d\t Comprimento: %d\t Pontos: %d |\n", qVidas, comprimento, pontos);
		for(l=0;l<N_LINHAS;l++){
			for(c=0;c<N_COLUNAS;c++){
				printf("%c", tela[l][c]);
			}
			printf("\n");
		}
		//for(l=0;l<100;l++) printf("%d,%d\n", cobra[l].x, cobra[l].y);
		if(kbhit()){
			getch();
			getch();
			dir = getch();
			//printf("%d\t\n\n", dir);
			switch(dir){
				case 65:
					direcao_atual = PCIMA;
				break;
				case 66:
					direcao_atual = PBAIXO;
				break;
				case 67:
					direcao_atual = DIREITA;
				break;
				case 68:
					direcao_atual = ESQUERDA;
				break;
			}
		}
		i = set_snake(tela, cobra, direcao_atual, comprimento);
		if(i){
			if(i==1){
				direcao_atual = DIREITA;
				init(tela, cobra, comprimento);
				qVidas--;
				alimento.x = 1+rand()%N_LINHAS-2;
				alimento.y = 1+rand()%N_COLUNAS-3;
				tela[alimento.x][alimento.y] = S_FOOD;
				if(qVidas<0){
					printf("GAME OVER\n");
					printf("PONTUAÇÃO: %d\n", pontos);
					printf("COMPRIMENTO FINAL: %d\n", comprimento);
					break;
				}
			}else if(i==2){
				//exit(0);
				comprimento++;
				pontos += (comprimento * 3) - 1;
				alimento.x = 1+rand()%N_LINHAS-2;
				alimento.y = 1+rand()%N_COLUNAS-3;
				tela[alimento.x][alimento.y] = S_FOOD;
			}
		}
		//scanf("1");
		timer = clock();
		//getchar();
		//Sleep(1);
		//comprimento++;
		usleep(200000);
	}while(1);
	//printf("%c\n", direcao_atual);
	return 0;
}

void Sleep(time_t delay){
	time_t timer0, timer1;
	time(&timer0);
	do{
		time(&timer1);
	}while((timer1 - timer0) < delay);
}

void vaipara(int x, int y){
	printf("\e[%d;%dH", x, y);
}

void limpatela(){
	vaipara(0,0);
	printf("\e[0J");
}

void init(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], int comprimento){
	char pos_inicial[4] = {LR_INICIAL, CR_INICIAL, LC_INICIAL, CC_INICIAL};
	int l, c;
	for(l=0;l<N_LINHAS;l++){
		for(c=0;c<N_COLUNAS;c++){

			if(l==0 || l==N_LINHAS-1){

				tela[l][c] = '_';

			}else if(c==0 || c==N_COLUNAS-1){

				tela[l][c] = '|';

			}else{

				tela[l][c] = __ESPACO;

			}

		}

	}
	
	//cobra[0].x = LR_INICIAL;
	//cobra[0].y = CR_INICIAL;
	for(c=0; c<=COMP_INICIAL; c++){
		cobra[c].y = c+CR_INICIAL; 
		cobra[c].x = CR_INICIAL;
	}
	for(l=0;l<comprimento;l++){
		tela[cobra[l].x][cobra[l].y] = S_BODY;
	}
	/*for(l=LR_INICIAL; l<=LC_INICIAL; l++){
		for(c=CR_INICIAL; c<=CC_INICIAL; c++){
			tela[l][c] = S_BODY;
		}
	}*/
	
		
}
int set_snake(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], char direcao_atual, int comprimento){
	int i;
	//for(i=0;i<=3;i++) pos[i] = __ESPACO;
	tela[cobra[0].x][cobra[0].y] = __ESPACO;

//	cobra[comprimento].x
	switch(direcao_atual){
		case DIREITA:
			//pos[0] = pos[2];
			//pos[1] = pos[3];
			//pos[2]++;
//			pos[1]++;
//			pos[3]++;
			cobra[comprimento].y = cobra[comprimento-1].y+1;
		break;
		case ESQUERDA:
//			pos[1]--;
//			pos[3]--;
			cobra[comprimento].y = cobra[comprimento-1].y-1;
		break;
		case PCIMA:
			//pos[1]++;
//			pos[2]--;
			cobra[comprimento].x = cobra[comprimento-1].x-1;
		break;
		case PBAIXO:
			cobra[comprimento].x = cobra[comprimento-1].x+1;
		break;
	}
	if(tela[cobra[comprimento].x][cobra[comprimento].y] != __ESPACO){
		if(tela[cobra[comprimento].x][cobra[comprimento].y] == S_FOOD){
			//comprimento++;
			tela[cobra[comprimento-1].x][cobra[comprimento-1].y] = S_BODY;
			return 2;
		}else return 1;
	}
	for(i=0;i<comprimento;i++){
		cobra[i].x = cobra[i+1].x;
		cobra[i].y = cobra[i+1].y;
	}
	//cobra[comprimento].x = cobra[comprimento].y = -1;
//	tela[pos[0]][pos[1]] = S_BODY;
//	tela[pos[2]][pos[3]] = S_BODY;
	tela[cobra[comprimento-1].x][cobra[comprimento-1].y] = S_BODY;
	return 0;
}
