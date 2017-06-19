#ifndef MAIN_H
#define MAIN_H


typedef struct TViz
{
	int id_viz;
	struct TViz *prox_viz;

} TViz;

typedef struct TNo
{
	int id_no;
	TViz *prim_viz;
	struct TNo *prox_no;

} TNo;

typedef struct grafo
{
	TNo *prim_no;
	int V; /* Qtd de vertices */

} TG;

typedef struct pilha
{
	int id;
	struct pilha *prox;
} TP;

#define TRUE 1
#define FALSE 0

void pilhaIni(TP* pilha);
void push(TP *pilha, int val);
void insertStack(TP * pilha, int val);
void showStack(TP *pilha);
void libera(TP* pilha);
int buscaVisitado(TP *pilha, int val);
void insereVisitado(TP *pilha, int val);
void caminho(TG *g, int id1, TP *pilha);
int encontraCaminho(TG *g, int id1, int id2);
int findLastId(TViz *v);
void achaPontes(TG *g);
void achaArticulacao(TG *g);
TG* create();
void release(TG *g);
TNo *findVertex(TG *g, int id);
int insertVertex(TG *g, int id);
int removeVertex(TG *g, int id);
int insertEdge(TG *g, int id1, int id2, int orientado);
TViz *findEdge(TG *g, int id1, int id2);
TViz *_remove(TViz *vertex, int id, int *flag);
int removeEdge(TG *G, int id1, int id2, int isDirected);
void print(TG *g);
int directed(TG *G);
int digito_em_comum(int primeiro, int segundo);
int junta_resultado(int primeiro, int segundo);
int sair_chegar(TG *g, int id1, int id2, int caminho);
int fortemente_conexos(TG *g);
int* conexo(TG* G);
int componentes_conexos(TG* G);
void information(TG* G);
int menu(TG *G);

#endif // MAIN_H