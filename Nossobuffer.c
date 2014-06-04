#include "buffend.h"
int main(int rg, char *nomeTabela[]){

	struct fs_objects objeto = leObjeto(nomeTabela[1]);

	if(objeto.qtdCampos == ERRO_NOME_TABELA){
		printf("Nome de tabela inválido!\n");
		return 0;
	}else if(objeto.qtdCampos == ERRO_ABRIR_ARQUIVO){
		printf("Erro de leitura em arquivo!\n");
		return 0;
	}

	tp_table *esquema = leSchema(objeto);

	if(esquema == ERRO_ABRIR_ESQUEMA){
		printf("Erro de leitura em arquivo!\n");
		return 0;
	}

	tp_buffer *bufferpoll = initbuffer();
	load_data(objeto, bufferpoll, tamTupla(esquema,objeto));
	cabecalho(esquema, objeto.qtdCampos);
	int erro = drawline(bufferpoll, esquema, objeto, 2,0);

	if(erro == ERRO_DE_PARAMETRO)
	{
		printf("Erro de parametro função drawline()!\n");
		return 0;
	}

	printbufferpoll(bufferpoll, esquema, objeto, 0);

	return 0;
}
