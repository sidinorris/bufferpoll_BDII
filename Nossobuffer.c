#include "buffend.h"



int main(){

	struct fs_objects objeto = leObjeto("Cliente");
	tp_table *esquema = leSchema(objeto);
	tp_buffer *bufferpoll = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
	FILE *arq2; //Ponteiros para arquivos
	
	int pos_inicial_regis = 0, tam_tupla = tamTupla(esquema,objeto);
	
	initbuffer(bufferpoll);
	
	/*arq = fopen("meta.dat", "r");
	if(arq == NULL){	
		printf("Read Error\n");
		return 0;
	}
	fread(&valor_reg, sizeof(int), 1, arq);
	tp_table *s = (tp_table*)malloc(sizeof(tp_table)*valor_reg); //Aloca um vetor com o numero de colunas da tupla
	tam_registro = load_metadata(arq, s, valor_reg);
	fclose(arq);
	*/
	arq2 = fopen(objeto.nArquivo, "r");
	
	if(arq2 == NULL){
		printf("Read Error\n");
		return 0;
	}
	load_data(arq2, bufferpoll, tam_tupla);
	fclose(arq2);
	
	//para imprimir
	
	//printbufferpoll(bufferpoll, s, 0, valor_reg); //Parametros são: buffer, estrutura dos meta dados, página a ser impressa e quantos campos tem a "tabela"
	
	cabecalho(esquema, objeto.qtdCampos);
	pos_inicial_regis = 1 * tamTupla(esquema,objeto);
	drawline(bufferpoll, esquema, objeto.qtdCampos, &pos_inicial_regis, 0); // (buffer, meta, numero de campos, posicao inicial, pagina)
	free(esquema);
	free(bufferpoll);

	return 0;
}
