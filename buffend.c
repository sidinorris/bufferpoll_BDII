#include "buffend.h"
struct fs_objects leObjeto(char *nTabela){
	
	FILE *dicionario;
	char *tupla = (char *)malloc(sizeof(char)*20);
	int cod;
	dicionario = fopen("fs_object.dat", "r"); // Abre o dicionario de dados.

	struct fs_objects objeto;
	objeto.qtdCampos = ERRO_NOME_TABELA; // Operacao para tratamento de erro.

	if (dicionario == NULL){
		objeto.qtdCampos = ERRO_ABRIR_ARQUIVO;
		return objeto;
	}

	while(fgetc (dicionario) != EOF){
        fseek(dicionario, -1, 1);

        fread(tupla, sizeof(char), 20, dicionario); //Lê somente o nome da tabela

        if(strcmp(tupla, nTabela) == 0){ // Verifica se o nome dado pelo usuario existe no dicionario de dados.
      		strcpy(objeto.nome, tupla);
      		fread(&cod,sizeof(int),1,dicionario);	// Copia valores referentes a tabela pesquisada para a estrutura.
      		objeto.cod=cod;
      		fread(tupla,sizeof(char),20,dicionario);
      		strcpy(objeto.nArquivo, tupla);
      		fread(&cod,sizeof(int),1,dicionario);
      		objeto.qtdCampos = cod;
      		
        	return objeto;
        }
        fseek(dicionario, 28, 1); // Pula a quantidade de caracteres para a proxima verificacao(4B do codigo, 20B do nome do arquivo e 4B da quantidade de campos).
	}
	return objeto;
}
tp_table *leSchema (struct fs_objects objeto){
	FILE *schema;
	int i = 0, cod;
	char *tupla = (char *)malloc(sizeof(char)*40);  
	tp_table *esquema = (tp_table *)malloc(sizeof(tp_table)*objeto.qtdCampos); // Aloca esquema com a quantidade de campos necessarios.

	schema = fopen("fs_schema.dat", "r"); // Abre o arquivo de esquemas de tabelas.

	if (schema == ERRO_ABRIR_ESQUEMA)
		return ERRO_ABRIR_ESQUEMA;

	while((fgetc (schema) != EOF) && (i < objeto.qtdCampos)){ // Varre o arquivo ate encontrar todos os campos com o codigo da tabela.
        fseek(schema, -1, 1);

        if(fread(&cod, sizeof(int), 1, schema)){ // Le o codigo da tabela.
        	if(cod == objeto.cod){ // Verifica se o campo a ser copiado e da tabela que esta na estrutura fs_objects.
        		
        		fread(tupla, sizeof(char), 40, schema);
        		strcpy(esquema[i].nome,tupla);					// Copia dados do campo para o esquema.
        		fread(&esquema[i].tipo, sizeof(char),1,schema);      
        		fread(&esquema[i].tam, sizeof(int),1,schema);   
        		i++;    		
        	}
        	else
        		fseek(schema, 45, 1); // Pula a quantidade de caracteres para a proxima verificacao (40B do nome, 1B do tipo e 4B do tamanho).
        }
        
    }
    return esquema;
}
void cpystr(char *tg, char *sc, int st, int len){ //SC = Ponteiro para string que carregada do arquivo; tg = Ponteiro para uma página do buffer
	st = st * len;
	
	int i=st,j=0;
	for (;i<len+st;i++)
	  tg[i]=sc[j++];
}
tp_buffer * initbuffer(){
	
	tp_buffer *bp = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
	int i;

	for (i = 0;i < PAGES; i++){
		bp->db=0;
		bp->pc=0;
		bp->nrec=0;
		bp++;
	}
	page_avalible = 0; // Variavel global controlando as páginas utilizadas
	return bp;
}
void cria_campo(int tam, int header, char *val, int x){
	int i;
	char aux[45];

	if(header){
		for(i = 0; i <= TAM && val[i] != '\0'; i++){
			aux[i] = val[i];
		}
		for(;i<TAM;i++)
			aux[i] = ' ';
		aux[i] ='\0';
		printf("%s", aux);

	}

	else{
		for(i = 0; i < x; i++)
			printf(" ");
	}
}
int drawline(tp_buffer *buffpoll, tp_table *s, struct fs_objects objeto, int p, int num_page){
	 
	if (num_page > PAGES || p > SIZE){
		return ERRO_DE_PARAMETRO;
	}
	int *pos_ini, aux = (p * tamTupla(s,objeto)) , num_reg = objeto.qtdCampos;
	pos_ini = &aux;
	int count, pos_aux, bit_pos;
	union c_double cd;
	union c_int ci;
	int x = 0;
	
	count = pos_aux = bit_pos = 0;
	
	for(count = 0; count < num_reg; count++){
		pos_aux = *(pos_ini);
		bit_pos = 0;
		

		switch(s[count].tipo){
			
			case 'S':
				x = 0;
				while(buffpoll[num_page].data[pos_aux] != '\0'){
			
					printf("%c", buffpoll[num_page].data[pos_aux]);
					if ((buffpoll[num_page].data[pos_aux++] & 0xc0) != 0x80) bit_pos++; //Conta apenas bits que possam ser impressos (UTF8)
				x++;
				}
				
				cria_campo((TAM - (bit_pos)), 0, (char*)' ', (40 - x));
				break;
			
			case 'I':
				while(pos_aux < *(pos_ini) + s[count].tam){
					ci.cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++];
				}
				printf("%d", ci.num); //Controla o número de casas até a centena
				cria_campo((TAM - (bit_pos)), 0, (char*)' ', 38);
				break;
				
			case 'D':
				while(pos_aux < *(pos_ini) + s[count].tam){
					cd.double_cnum[bit_pos++] = buffpoll[num_page].data[pos_aux++]; // Cópias os bytes do double para área de memória da union
				}
				printf("%.3lf", cd.dnum);
				cria_campo((TAM - (bit_pos)), 0, (char*)' ', 34);
				break;
			
			case 'C': 
				printf("%c", buffpoll[num_page].data[pos_aux]);
				if(s[count].tam < strlen(s[count].nome)){
					bit_pos = strlen(s[count].nome);
				}
				else{
					bit_pos = s[count].tam;
				}
				cria_campo((bit_pos - 1), 0, (char*)' ', 39);	
				break;
			
			default: printf("Erro de Impressão\n\n\n");
				break;
		}
		*(pos_ini) += s[count].tam;		
	}
	printf("\n");
	return 0;
}
int cabecalho(tp_table *s, int num_reg){
	int count, aux;
	aux = 0;
	
	for(count = 0; count < num_reg; count++){
		cria_campo(s[count].tam, 1, s[count].nome, 0); // O segundo parâmetro significa se = 1 Cabecalho se = 0 é valor daquele campo
		aux += s[count].tam;
	}
	printf("\n");
	return aux;
}
int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page){
	
	int aux, i, num_reg = objeto.qtdCampos;
	
	
	if(buffpoll[num_page].nrec == 0){
		return 0;	
	}
	
	i = aux = 0;
	
	aux = cabecalho(s, num_reg);
	
	
	while(i < buffpoll[num_page].nrec){ // Enquanto i < numero de registros * tamanho de uma instancia da tabela
		drawline(buffpoll, s, objeto, i, num_page);
		i++;
	}
	return 1;
}
int load_metadata(FILE *arq_meta, tp_table *s, int valor_reg){
	
	int count, pos;
	int tam_registro;
	pos = tam_registro = 0;
	char c;
	
	for(count = 0; count < valor_reg; count++){	 //Passa pelo numero de ocorrencias da tabela
		pos = 0;
	
		while(fread(&c, sizeof(char), 1, arq_meta) && c != '\0'){ //le o nome do campo
			s[count].nome[pos] = c;
			pos++;
		}
	
		s[count].nome[pos] = c;	//acrescenta o \0 no final da string		
		fread(&c, sizeof(char), 1, arq_meta); //le o tipo (S, I, D, C)
		s[count].tipo = c;
		fread(&s[count].tam, sizeof(int), 1, arq_meta);
		
		tam_registro += s[count].tam;
		//printf("%s %c %d\n", s[count].nome, s[count].tipo, s[count].tam);
		//printf("TAMANHO DA TUPLA: %d\n", tam_registro);
		
	}
	return tam_registro;
}
int tamTupla(tp_table *esquema, struct fs_objects objeto){// Retorna o tamanho total da tupla da tabela.

    int qtdCampos = objeto.qtdCampos, i, tamanhoGeral = 0;
   
    if(qtdCampos){ // Lê o primeiro inteiro que representa a quantidade de campos da tabela.
		for(i = 0; i < qtdCampos; i++)
			tamanhoGeral += esquema[i].tam; // Soma os tamanhos de cada campo da tabela.
	}

	return tamanhoGeral;
}
void load_data( struct fs_objects objeto, tp_buffer *bufferpool, int tam_registro){

	FILE *arq_data;
	arq_data = fopen(objeto.nArquivo, "r");
	
	if(arq_data == NULL){
		return;
	}
	
	char *lei_aux;
	int page_count;
	page_count = 0;
	
	lei_aux = (char*)malloc(sizeof(char)*tam_registro);
		
	while(fgetc(arq_data) != EOF && page_avalible < PAGES){ //le os dados do arquivo data
	
		fseek(arq_data, -1, 1);		
		fread(lei_aux, sizeof(char)*tam_registro, 1, arq_data);
		
		if(page_count + tam_registro < SIZE){
			cpystr(bufferpool[page_avalible].data, lei_aux, bufferpool[page_avalible].nrec, tam_registro);
			bufferpool[page_avalible].nrec++;
			page_count += tam_registro;
		}
		else if ((++page_avalible) < PAGES){ // Se ainda tiver páginas vai para a próxima, caso substitui segundo política
			page_count = 0;
			
		}
		else{ // Vai para à política de substituição
			page_avalible = 0;	
		}
	}
	free(lei_aux);
	fclose(arq_data);	
}
