#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 512
#define PAGES 1024
#define TAM 40
// Tratamendo de erros.
#define ERRO_NOME_TABELA -1
#define ERRO_ABRIR_ARQUIVO -2
#define ERRO_DE_PARAMETRO -3
#define ERRO_ABRIR_ESQUEMA NULL

struct fs_objects { // Estrutura usada para carregar fs_objects.dat
	char nome[20];
	int cod;
	char nArquivo[20];
	int qtdCampos;
};

typedef struct table_bd{ // Estrutura usada para carregar fs_schema.dat
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

struct fs_objects leObjeto(char *nTabela);
tp_table *leSchema (struct fs_objects objeto);
int tamTupla(tp_table *esquema, struct fs_objects objeto);
void cpystr(char *, char *, int, int);
tp_buffer * initbuffer();
void cria_campo(int tam, int header, char *val, int x);
int drawline(tp_buffer *buffpoll, tp_table *s, struct fs_objects objeto, int p, int num_page);
int cabecalho(tp_table *, int);
int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page);
int load_metadata(FILE *, tp_table *, int);
void load_data( struct fs_objects objeto, tp_buffer *bufferpool, int tam_registro);
int page_avalible;