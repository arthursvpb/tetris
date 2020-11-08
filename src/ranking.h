/*
* Centro Universitário de João Pessoa, 2018.
* Projeto de programação: TETRIS USANDO A LINGUAGEM C E A BIBLIOTECA ALLEGRO.
*
* Arthur Vasconcellos <contatoarthursilveira@gmail.com>
* 	github.com/arthursvpb
* 
* Lucca Barbosa <luccab73@gmail.com>
* 	github.com/LuccaBM73
*/

/***
 *    _________ _______ _________ _______ _________ _______ 
 *    \__   __/(  ____ \\__   __/(  ____ )\__   __/(  ____ \
 *       ) (   | (    \/   ) (   | (    )|   ) (   | (    \/
 *       | |   | (__       | |   | (____)|   | |   | (_____ 
 *       | |   |  __)      | |   |     __)   | |   (_____  )
 *       | |   | (         | |   | (\ (      | |         ) |
 *       | |   | (____/\   | |   | ) \ \_____) (___/\____) |
 *       )_(   (_______/   )_(   |/   \__/\_______/\_______)
 *                                                          
 ***/

/* CABEÇALHO .H: BIBLIOTECA PARA FUNÇÕES E TIPOS */

#ifndef _ranking_funcao_h
#define _ranking_funcao_h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TIPOS SIMPLES
typedef struct tipo {
    char nome [50];
    int vitorias;
    struct tipo * prox;
} PLAYER;

// FUNÇÃO PARA ALOCAR DINAMICAMENTE O RANKING
void alocacao_dinamica_ranking(PLAYER * lista) {
    FILE * arquivo = fopen("ranking.txt", "w");
    
    //Itera na lista para salvar cada linha:
    PLAYER * aux = lista;
    while(aux) {
        fprintf(arquivo, "%s,%d\n", aux->nome, aux->vitorias);
        
        // Iterando
        aux = aux->prox;
    }
    
 	// Fechamento do arquivo
    fclose(arquivo);
}

PLAYER * carregaRanking() {
    FILE * arquivo = fopen("ranking.txt", "r");
	// Verifica abertura do arquivo:
	if(!arquivo) return 0;
	
    // Iniciando a lista
    PLAYER * lista = (PLAYER*) malloc (sizeof(PLAYER));
    if(!lista) return 0;

	char linha [60];
	fgets(linha, 60, arquivo);
	
	// Em caso de erro
	if(feof(arquivo)) {
        printf("Lista de ranking vazia.");
        return 0;
	}
	char * nome;

    nome = strtok(linha,",");
 
    strcpy(lista->nome,nome);
  
    char * s_vitorias = strtok(NULL,",");
    
    lista->vitorias = atoi(s_vitorias);
    lista->prox = 0;

    fgets(linha, 60, arquivo);
  
    PLAYER * anterior = lista;
 
    while(!feof(arquivo)) {
        PLAYER * novo = (PLAYER*) malloc (sizeof(PLAYER));
            
			// Verifica
            if(!novo) return 0;

        strcpy(novo->nome,strtok(linha,","));
        novo->vitorias = atoi(strtok(NULL,","));
        novo->prox = 0;
        anterior->prox = novo;
		anterior = novo;
        fgets(linha, 60, arquivo);
    }
    
    fclose(arquivo);
    return lista;
}
// FUNÇÃO PARA SALVAR A PONTUAÇÃO DO JOGADOR
int salva_ranking(int pontuacao){
	
	FILE *f = fopen("ranking.txt", "a");
	if (f == NULL)
	{
	    printf("Erro ao abrir o arquivo!\n");
	    exit(1);
	}
	
	fprintf(f,"%d PONTOS\n", pontuacao); 	
	
	fclose(f);
}

#endif
