#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 512
#define PAGES 1024
#define TAM 40

#define ERRO_ABRIR_ARQUIVO -1
#define ERRO_NOME_TABELA -2

struct fs_objects {
	char nome[20];
	int cod;
	char nArquivo[20];
	int qtdCampos;
};

typedef struct table_bd{
	char nome[TAM];
	char tipo;
	int tam;
}tp_table;

typedef struct buffer{
   unsigned char db; //Dirty bit
   unsigned char pc; //Pin counter
   unsigned int nrec; //numero de registros armazenados na pagina
   char data[SIZE];
}tp_buffer;

union c_double{
	
	double dnum;
	char double_cnum[sizeof(double)];	
};

union c_int{
	
	int  num;
	char cnum[sizeof(int)];
};

struct fs_objects leObjeto(char * nTabela);
tp_table *leSchema (struct fs_objects objeto);
char *strcop(char *data, int pos, int tam);
int tamTupla(tp_table *campos, struct fs_objects meta);

void cpystr(char *, char *, int, int);
void initbuffer(tp_buffer *);
void cria_campo(int, int, char *);
void drawline(tp_buffer *, tp_table *, int, int *, int);
int cabecalho(tp_table *, int);
int printbufferpoll(tp_buffer *, tp_table *, int, int);
int load_metadata(FILE *, tp_table *, int);
void load_data(FILE *, tp_buffer *, int);

int page_avalible;