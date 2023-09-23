#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "tela.h"

typedef struct{
    int joga;
    cor c_user;
    cor c_pc;
    int cor;
    int barra;
    float pontos;
    double inicio;
    double tempo;
} dados;

typedef struct{
  float pontos[3];
  char nome[3][15];
}rank;

void corf(int r, int g, int b){printf("\e[48;2;%d;%d;%dm", r, g, b);}
void corn(void){printf("\e[m");}
void pos(int lin, int col){printf("\e[%d;%dH", lin, col);}
void corl(int r, int g, int b){printf("\e[38;2;%d;%d;%dm", r, g, b);}
void limpa(void){printf("\e[2J");}

float pior_possivel_r(dados dados_jogo){
  float pior;
  if(dados_jogo.c_pc.vermelho >= 127){
    pior = dados_jogo.c_pc.vermelho;
  }else if(dados_jogo.c_pc.vermelho < 127){
    pior = 255 - dados_jogo.c_pc.vermelho;
  }
  return pior;
}

float pior_possivel_g(dados dados_jogo){
  float pior;
  if(dados_jogo.c_pc.verde >= 127){
    pior = dados_jogo.c_pc.verde;
  }else if(dados_jogo.c_pc.verde < 127){
    pior = 255 - dados_jogo.c_pc.verde;
  }
  return pior;
}

float pior_possivel_b(dados dados_jogo){
  float pior;
  if(dados_jogo.c_pc.azul >= 127){ 
    pior = dados_jogo.c_pc.azul;
  }else if(dados_jogo.c_pc.azul < 127){
    pior = 255 - dados_jogo.c_pc.azul;
  }
  return pior;
}
float minha_dist(float me, float pc){
  float necessario;
  if( pc >= me){
    necessario = pc - me;
  }else if(me > pc){
    necessario = me - pc;
  }
  return necessario;
} 

float calc_final(float pc, float me){
  float fim;
  fim = (((me*100)/pc)-100)*(-1);
  return fim;
}

dados calc_bonus(dados dados_jogo){ //achei 5% de bonus muita coisa, entao dei 1% só
  float seg_bonus, percent_bonus, ganhou_bonus;
  if(dados_jogo.tempo < 30){
    seg_bonus = 30 - dados_jogo.tempo;
  }
  percent_bonus = seg_bonus*0.01;
  ganhou_bonus = seg_bonus*1;
  dados_jogo.pontos = dados_jogo.pontos*percent_bonus;
  pos(2, 24);
  printf("Voce ganhou %%%.2f de bonus por encerrar antes do tempo", ganhou_bonus);
  return dados_jogo;
}

float calc_pontos(dados dados_jogo){
  float pontuacao;
  float pior_r, pior_g, pior_b, me_r, me_g, me_b;
  float p_r, p_g, p_b;
  pior_r = pior_possivel_r(dados_jogo);
  pior_g = pior_possivel_g(dados_jogo);
  pior_b = pior_possivel_b(dados_jogo);
  me_r = minha_dist(dados_jogo.c_user.vermelho, dados_jogo.c_pc.vermelho);
  me_g = minha_dist(dados_jogo.c_user.verde, dados_jogo.c_pc.verde);
  me_b = minha_dist(dados_jogo.c_user.azul, dados_jogo.c_pc.azul);
  p_r = calc_final(pior_r, me_r);
  p_g = calc_final(pior_g, me_g);
  p_b = calc_final(pior_b, me_b);
  pontuacao = (p_r+p_g+p_b)/3;
  dados_jogo.pontos = pontuacao;
  dados_jogo = calc_bonus(dados_jogo);
  dados_jogo.pontos = pontuacao + dados_jogo.pontos;
  return dados_jogo.pontos;
}

dados desenha_quadrado_pc(dados dados_jogo){
  pos(1, 1);
  dados_jogo.c_pc.vermelho = rand()%255 ;
  dados_jogo.c_pc.verde = rand()%255;
  dados_jogo.c_pc.azul = rand()%255;
  tela_cor_fundo(dados_jogo.c_pc);
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 10; j++){
      printf(" ");
    }
    printf("\n");
  }
  tela_cor_normal();
  return dados_jogo;
} 

dados desenha_quadrado_pc_final(dados dados_jogo){
  pos(1, 1);
  tela_cor_fundo(dados_jogo.c_pc);
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 10; j++){
      printf(" ");
    }
    printf("\n");
  }
  tela_cor_normal();
  return dados_jogo;
} 

void desenha_quadrado_player(dados cor_player){
  int aux =1;
  pos (aux, 12);
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 10; j++){
      tela_cor_fundo(cor_player.c_user);
      printf(" ");
    }
    aux++;
    printf("\n");
    pos (aux, 12);
  }
  tela_cor_normal();
}

void imprime_barra_vermelha(int r, int barra){
   if(barra == 1){
    corl(255, 255, 255);
    printf(">");
  }else{
    corn();
    printf(" ");
  }
  for(int i = 1; i <= 256; i++){
    if((r/5)*5 == i-1){
      corf(255, 255, 255);
      printf(" ");
      tela_cor_normal();
    }
    if(i == 256){
      corf(i-1, 0, 0);
    }else{
      corf(i, 0, 0);
    }
    printf(" ");
    i+=4;
  }
  tela_cor_normal();
  pos(7, 57);
  printf("%3i", r);
}
void imprime_barra_verde(int g, int barra){
    if(barra == 2){
    corl(255, 255, 255);
    printf(">");
    }else{
    corn();
    printf(" ");
  }
  for(int i = 1; i <= 256; i++){
    if((g/5)*5 == i-1){
      corf(255, 255, 255);
      printf(" ");
      tela_cor_normal();
    }
    if(i == 256){
      corf(0, i-1, 0);
    }else{
      corf(0, i, 0);
    }
    printf(" ");
    i+=4;
  }
  tela_cor_normal();
  pos(8, 57);
  printf("%3i", g);
}

void imprime_barra_azul(int b, int barra){
   if(barra == 3){
    corl(255, 255, 255);
    printf(">");
  }else{
    corn();
    printf(" ");
  }
  for(int i = 1; i <= 256; i++){
    if((b/5)*5 == i-1){
      corf(255, 255, 255);
      printf(" ");
      tela_cor_normal();
    }
    if(i == 256){
      corf(0, 0, i-1);
    }else{
      corf(0, 0, i);
    }
    printf(" ");
    i+=4;
  }
  tela_cor_normal();
  pos(9, 57);
  printf("%3i", b);
}
dados incremento_barra(dados cor_player){
  if(cor_player.barra < 3){
      cor_player.barra++;
  }
  return cor_player;
}
dados decremento_barra(dados cor_player){
  if( cor_player.barra > 1){
      cor_player.barra--;
  }
  return cor_player;
}
dados left(dados cor_player){
  switch (cor_player.barra){
      case 1:
        if(cor_player.c_user.vermelho > 0){
          cor_player.c_user.vermelho--;
          break;
        }
        case 2:
          if(cor_player.c_user.verde > 0){
          cor_player.c_user.verde--;
          break;
          }
        case 3:
          if(cor_player.c_user.azul > 0){
          cor_player.c_user.azul--;
          break;
        }
      }
  return cor_player;
}
dados right(dados cor_player){
  switch (cor_player.barra)
        {
        case 1:
        if(cor_player.c_user.vermelho < 255){
          cor_player.c_user.vermelho++;
          break;
        }
        case 2:
          if(cor_player.c_user.verde < 255){
          cor_player.c_user.verde++;
          break;
          }
        case 3:
          if(cor_player.c_user.azul < 255){
          cor_player.c_user.azul++;
          break;
        }
    }
  return cor_player;
}

dados continua_jogo(dados cor_player, bool *enter){
  pos(7, 1);
  int tecla = tela_le_char();
  switch (tecla){
    case  c_up:
      cor_player = decremento_barra(cor_player);
      break;
    case c_down:
      cor_player = incremento_barra(cor_player);
      break;
    case c_left:
      cor_player = left(cor_player);
      break;
    case c_right:
      cor_player = right(cor_player);
      break;
    case c_enter:
      *enter = true;
      break;
  }
  imprime_barra_vermelha(cor_player.c_user.vermelho, cor_player.barra);
  pos(8, 1);
  imprime_barra_verde(cor_player.c_user.verde, cor_player.barra);
  pos(9, 1);
  imprime_barra_azul(cor_player.c_user.azul, cor_player.barra);
  desenha_quadrado_player(cor_player);
  return cor_player;
}

dados imprime_dados_jogo(dados dados_jogo){
  int r_user, g_user, b_user, r_pc, g_pc, b_pc;
  r_user = dados_jogo.c_user.vermelho;
  g_user = dados_jogo.c_user.verde;
  b_user = dados_jogo.c_user.azul;
  r_pc = dados_jogo.c_pc.vermelho;
  g_pc = dados_jogo.c_pc.verde;
  b_pc = dados_jogo.c_pc.azul;
  dados_jogo.pontos = calc_pontos(dados_jogo);
  desenha_quadrado_pc_final(dados_jogo);
  desenha_quadrado_player(dados_jogo);
  pos(7, 1);
  printf("Voce Jogou: R%i G%i B%i", r_user,g_user,b_user);
  printf("\nAs cores eram:  R%i G%i B%i", r_pc, g_pc, b_pc);
  pos(1, 24);
  printf("A sua pontuacao final foi: %.2f", dados_jogo.pontos);
  return dados_jogo;
}

double relogio(double inicio){
    double tempo;
    tempo = tela_relogio() - inicio;
    pos(1, 24);
    printf("%.2lf", tempo);
    return tempo;
}

dados reseta_cor_barra(dados qualquer){
    qualquer.c_user.vermelho = 127;
    qualquer.c_user.verde = 127;
    qualquer.c_user.azul = 127;
    return qualquer;
}


// FUNÇÃO COM MAIS DE 25 LINHAS ***ALTERAAAAAR****
void ranking(dados dados_principais, rank *p){
  pos(10, 1);
  p->pontos[0] = 0;
  p->pontos[1] = 0;
  p->pontos[2] = 0;
  FILE *arq;
  arq = fopen("./top3.txt", "r");
  if(arq == NULL){
    arq = fopen("./top3.txt", "w");
    fprintf(arq, "null 00\n null 00\n null 00");
    fclose(arq);
    arq = fopen("./top3.txt","r");
  }
  for(int i = 0; i < 3; i++){
    fscanf(arq, "%f %s", &(p->pontos[i]), p->nome[i]);
  }
  fclose(arq);
  if(dados_principais.pontos > p->pontos[0]){
    p->pontos[2] = p->pontos[1];
    p->pontos[1] = p->pontos[0];
    p->pontos[0] = dados_principais.pontos;
    for(int i = 0; i < 15; i++){
      p->nome[2][i] = p->nome[1][i];
      p->nome[1][i] = p->nome[0][i];
    }
    printf("Voce entrou no rank, Digite seu nome ");
    scanf("%15s", p->nome[0]);
    arq = fopen("./top3.txt","w");
    for(int k = 0; k < 3; k++){
      fprintf(arq, "%2.f %s \n", p->pontos[k], p->nome[k]);
    }
    fclose(arq);
  }else if(dados_principais.pontos > p->pontos[1]){
    p->pontos[2] = p->pontos[1];
    p->pontos[1] = dados_principais.pontos;
    for(int i = 0; i < 15; i++){
      p->nome[2][i] = p->nome[1][i];
    }
    printf("Voce entrou no rank, Digite seu nome ");
    scanf("%15s", p->nome[1]);
    arq = fopen("./top3.txt","w");
    for(int k = 0; k < 3; k++){
      fprintf(arq, "%.2f %s \n", p->pontos[k], p->nome[k]);
    }
    fclose(arq);
  }else if(dados_principais.pontos > p->pontos[2]){
    p->pontos[1] = dados_principais.pontos;
    printf("Voce entrou no rank, Digite seu nome ");
    scanf("%15s", p->nome[3]);
    arq = fopen("./top3.txt","w");
    for(int k = 0; k < 3; k++){
      fprintf(arq, "%.2f %s \n", p->pontos[k], p->nome[k]);
    }
    fclose(arq);
  }
  tela_limpa();
  imprime_dados_jogo(dados_principais);
  pos(10, 1);
  printf("RANKING TOP3 \n1- %s %.2f \n2- %s %.2f \n3- %s %.2f", p->nome[0], p->pontos[0], p->nome[1], p->pontos[1], p->nome[2], p->pontos[2]);
}

void inicia_jogo(dados dados_principais){
  int jogar;
  rank top3;
  do{
    bool enter = false;
    double inicio = tela_relogio();  // marca a hora de início do programa
    double tempo = 0;
    dados_principais = reseta_cor_barra(dados_principais);
    tela_limpa();
    tela_cria();
    tela_mostra_cursor(false);
    dados_principais = desenha_quadrado_pc(dados_principais);
    dados_principais.barra = 1;
    while(tempo < 30  && !enter){
      tempo = relogio(inicio);
      dados_principais = continua_jogo(dados_principais, &enter);
    }
      tela_destroi();
      dados_principais.tempo = tempo;
      dados_principais = imprime_dados_jogo(dados_principais);
      ranking(dados_principais, &top3);
      pos(15, 1);
      printf("Deseja jogar de novo? 1/0 ");
      scanf("%i", &jogar);
  }while(jogar == 1);
}

int main(){
  srand(time(0));
  dados dados_principais;
  dados_principais.c_user.vermelho = 0;
  dados_principais.c_user.verde = 0;
  dados_principais.c_user.azul = 0;
  inicia_jogo(dados_principais);
}