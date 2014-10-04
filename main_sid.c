#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "buffend.h"



int main(){

	int erro;
	
	table *filme   = NULL;
	table *genero  = NULL;	
	table *cliente = NULL;
	table *locacao = NULL;

	filme = iniciaTabela("Filme");
	filme = adicionaCampo(filme, "Codigo_Filme", 'I', (sizeof(int)));
	filme = adicionaCampo(filme, "Nome", 'S', 20);
	filme = adicionaCampo(filme, "Autor", 'S', 20);
	filme = adicionaCampo(filme, "Codigo_Genero", 'I', (sizeof(int)));
	filme = adicionaCampo(filme, "Ano_Lancamento",'I', (sizeof(int)));
	erro = finalizaTabela(filme);
	
	//printf("%c",filme->esquema[0].tipo);
	
	if(erro != SUCCESS){
		printf("Erro ..%d: na função finalizaTabela().\n", erro);
		return 0;
	}	


// o resto é igual e isso
