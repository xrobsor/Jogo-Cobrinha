#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kbhit.c"
#include "gconio.h"
#include <unistd.h>

#define N_LINHAS 20
#define N_COLUNAS 3*N_LINHAS
#define S_FOOD 'o'
#define S_BODY '*'
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

void sortFood(char tela[N_LINHAS][N_COLUNAS]);
void limpatela();
void vaipara(int x, int y);
void init(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], int comprimento);
int set_snake(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], char direcao_atual, int comprimento);

int main(){
	char tela[N_LINHAS][N_COLUNAS];
	int l, c, comprimento = COMP_INICIAL, qVidas = 3, pontos = 0, maiorComp = 0;
	clock_t timer;
	char direcao_atual = DIREITA;
	char direcao_anterior = 0, dir;
	int pos[4] = {LR_INICIAL, CR_INICIAL, LC_INICIAL, CC_INICIAL}, i = 0;
	posicao alimento;
	posicao cobra[100]; // esse vetor de struct posicao armazena a posiçao da matriz onde esta
			// cada 'pedaco' do corpo da cobra para q o codigo saiba onde apagar a cobra

	for(l=0;l<100;l++){
		cobra[l].x = -1;
		cobra[l].y = -1;
	}
	srand(time(NULL));
	init(tela, cobra, comprimento);
	sortFood(tela);
	do{
		limpatela();
		printf("| Vidas: %d\t Comprimento: %d\t Pontos: %d |\n", qVidas, comprimento, pontos);
		for(l=0;l<N_LINHAS;l++){ // esse laço mostra a matriz onde esta ocorrendo o jogo
			for(c=0;c<N_COLUNAS;c++){
				printf("%c", tela[l][c]);
			}
			printf("\n");
		}
		if(kbhit()){ //  a função kbhit retorna verdadeiro se o usuario tiver pressionado alguma tecla		
			getch();
			getch();
			dir = getch();
			switch(dir){
				case 65:
					if(direcao_atual != PBAIXO) direcao_atual = PCIMA;
				break;
				case 66:
					if(direcao_atual != PCIMA) direcao_atual = PBAIXO;
				break;
				case 67:
					if(direcao_atual != ESQUERDA) direcao_atual = DIREITA;
				break;
				case 68:
					if(direcao_atual != DIREITA) direcao_atual = ESQUERDA;
				break;
			}
		}
		i = set_snake(tela, cobra, direcao_atual, comprimento);
		if(i){
			if(i==1){ // cobra acerta a parede ou ela mesma
				direcao_atual = DIREITA;
				comprimento = COMP_INICIAL;
				init(tela, cobra, comprimento);
				qVidas--;
				pontos -= pontos/3;
				sortFood(tela);
				if(qVidas<0){
					printf("GAME OVER\n");
					printf("PONTUAÇÃO: %d\n", pontos);
					printf("MAIOR COMPRIMENTO : %d\n", maiorComp);
					break;
				}
			}else if(i==2){ // cobra acerta a comida
				comprimento++;
				if(comprimento > maiorComp) maiorComp = comprimento;
				if(comprimento == 20) qVidas++;
				pontos += (comprimento * 3) - 1;
				sortFood(tela);
			}
		}
		timer = clock();
		usleep(200000);
	}while(1);

	return 0;
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

	for(c=0; c<=COMP_INICIAL; c++){ // inicio as posicoes da cobra
		cobra[c].y = c+CR_INICIAL;
		cobra[c].x = CR_INICIAL;
	}
	for(l=0;l<comprimento;l++){ // inicio a posicao da cobra na matriz
		tela[cobra[l].x][cobra[l].y] = S_BODY;
	}

}
int set_snake(char tela[N_LINHAS][N_COLUNAS], posicao cobra[], char direcao_atual, int comprimento){
	int i;
	tela[cobra[0].x][cobra[0].y] = __ESPACO; // aqui eu apago o ultimo pedaco da cobra que sera o rabo dela

	switch(direcao_atual){ // verifico para onde a cobra esta indo
		case DIREITA:
			cobra[comprimento].y = cobra[comprimento-1].y+1;
			cobra[comprimento].x = cobra[comprimento-1].x;
		break;
		case ESQUERDA:
			cobra[comprimento].y = cobra[comprimento-1].y-1;
			cobra[comprimento].x = cobra[comprimento-1].x;
		break;
		case PCIMA:
			cobra[comprimento].x = cobra[comprimento-1].x-1;
			cobra[comprimento].y = cobra[comprimento-1].y;
		break;
		case PBAIXO:
			cobra[comprimento].x = cobra[comprimento-1].x+1;
			cobra[comprimento].y = cobra[comprimento-1].y;
		break;
	}
	if(tela[cobra[comprimento].x][cobra[comprimento].y] != __ESPACO){ // verifica se a cobra acertou alguma coisa
		if(tela[cobra[comprimento].x][cobra[comprimento].y] == S_FOOD){ // acertou a comida
			tela[cobra[comprimento].x][cobra[comprimento].y] = S_BODY;
			return 2; // retorna 2 qnd acerta a comida, a funcao apenas aumenta o tamanho da cobra o restante
				// devera ser tratado na funcao main
		}else return 1;// retorna 1 se tiver acertado a parede ou ela mesma
	}
	for(i=0;i<comprimento;i++){// cobra[0] sera sempre o rabo da cobra, como no inicio desta funcao  nos apagamos
				// esta posicao aqui arrastaremos o restante do vetor
		cobra[i].x = cobra[i+1].x;
		cobra[i].y = cobra[i+1].y;
	}
	tela[cobra[comprimento-1].x][cobra[comprimento-1].y] = S_BODY; // aqui eu adiciono mais um pedaco do corpo da cobra
									// como no inicio da funcao apagamos um pedaco 
									// isso nos da a sensacao de q a cobra esta se movendo
	return 0; // a funcao retorna 0 quando a cobra nao acerta nada, portanda nada de especial acontece apenas a cobra se move
}
void sortFood(char tela[N_LINHAS][N_COLUNAS]){
	posicao alimento;
	do{
		alimento.x = 1+rand()%N_LINHAS;
		alimento.y = 1+rand()%N_COLUNAS;
	}while(tela[alimento.x][alimento.y] != __ESPACO);
	tela[alimento.x][alimento.y] = S_FOOD;
}
