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
	
} TG;

typedef struct pilha
{
	int id;
	struct pilha *prox;
} TP;



#endif // MAIN_H