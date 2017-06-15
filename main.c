#include <stdio.h> /* I/O */
#include <stdlib.h> /* malloc */
#include "main.h" /* structs */


/*
	Cria um grafo
*/
TG* create()
{
	TG * g = (TG*)malloc(sizeof(TG));
	g->prim_no = NULL;

	return g;
}

/*
	Libera um grafo	
*/
void release(TG *g)
{
	TNo *p = g->prim_no;

	while(p)
	{
		TViz *v = p->prim_viz;

		while(v)
		{
			TViz *t = v;
			v = v->prox_viz;
			free(t);
		}

		TNo *q = p;

		p = p->prox_no;

		free(q);
	}
}

TNo *buscaNo(TG *g, int id)
{
	TNo *p = g->prim_no;

	while(p)
	{
		if(p->id_no == id) 
			break;

		p = p->prox_no;
	}

	return p;
}

void insereNo(TG *g, int id)
{  
	/* Verifica se o vértice já existe no grafo. Se existir, não inserir novamente */
	if(buscaNo(g, id))
		return;

	TNo *vertice = (TNo*)malloc(sizeof(TNo));

	vertice->id_no = id;
	vertice->prim_viz = NULL;
	vertice->prox_no = NULL;

	TNo *p = g->prim_no;
	
	if(p)
	{	
		while(p->prox_no)
			p = p->prox_no;

		p->prox_no = vertice;
	}
	else
		g->prim_no = vertice;
}



void retiraNo(TG *g, int id)
{
	TNo *p = g->prim_no;

	TViz *anterior = NULL;

	TNo *anterior_no = NULL;

	while(p && (p->id_no != id))
	{
		anterior_no = p;
		p = p->prox_no;
	}

	/*
	 	Se p é NULL, não existe o que ser retirado
	*/
	if(p)
	{
		/*
			Vamos excluir os caminhos de p para outro nos
		*/
		TViz *v = p->prim_viz;             

		while(v)
		{
			TViz *aux = v;
			v = v->prox_viz;
			free(aux);
		}

		TNo *q = g->prim_no;

		while(q)
		{   			//agora vamos excluir caminhos de outros nos ate p
			if(p != q)
			{                 //se q nao for o proprio vertice a ser retirado
				TViz *v = q->prim_viz; //, buscamos se ele tem aresta para chegar em p

				while(v)
				{
					if(v->id_viz == id)
					{
						TViz *aux = v;
						v = v->prox_viz;

						if(anterior)
						{
							anterior->prox_viz = v;
						}
						else
						{
						 	q->prim_viz = v;
						}

						free(aux);     //so existe um caminho de q para p,

						break;		//logo podemos parar de procurar
					}
					else 
					{
						anterior = v; v = v->prox_viz;
					}
				}
			}

			anterior = NULL;

			q = q->prox_no;			//e agora olhamos se outro nó tem aresta para p
		}

		//agora vamos enfim retirar o no
		if(!anterior_no)
		{		//se p é o primeiro no do grafo
			g->prim_no = p->prox_no;	//o primeiro no do grafo passa a ser o proximo de p

			free(p);

			return;
		}
		anterior_no->prox_no = p->prox_no; //o no anterior a p aponta para o proximo no após p

		free(p);
	}
}

void insereAresta(TG *g, int id1, int id2, int orientado)
{//id1 é origem, id2 é destino 

	TNo *p = g->prim_no;

	while((p) && (p->id_no != id2))
	{ //acha o nó de destino
		p = p->prox_no;

	}

	if(!p) 
		return;  //se o no de destino não existe, não faz nada

	p = g->prim_no;

	while((p) && (p->id_no != id1))
	{ //acha o nó de origem
		p = p->prox_no;
	}

	if(!p) 
		return;  //se o no de origem não existe, não faz nada

	TViz *novo = (TViz*)malloc(sizeof(TViz));
	novo->id_viz = id2;

	TViz *v = p->prim_viz;

	if(!v)
	{ 		//se não tem primeiro vizinho, o novo é o primeiro agora 
		p->prim_viz = novo;

		if(!orientado)
			 insereAresta(g, id2, id1, 1); //se o grafo for nao for orientado

		return;                                  // acrescenta a volta
	}

	printf("v é %d\n", v->id_viz);

	while(v->prox_viz)
	{
		if(v->id_viz == id2)
			return; //se o caminho ja existe, não faz nada. 
									 //se não for orientado ja existe o caminho id2->id1 também

		v = v->prox_viz;  //acha o ultimo vizinho
	}

	v->prox_viz = novo;   //acrescenta o novo caminho no final da lista de vizinhos

	if(!orientado)
		insereAresta(g, id2, id1, 1); //se não for orientado, faz o caminho id2->id1
}

TViz *buscaAresta(TG *g, int id1, int id2)
{
	TNo *p = buscaNo(g, id1);

	TViz *v = p->prim_viz;

	while(v)
	{
		if(v->id_viz == id2)
			break;

		v = v->prox_viz;
	}

	return v;
}

void retiraAresta(TG *g, int id1, int id2, int orientado)
{
}

/*
	Exibe o grafo na tela
*/
void print(TG *g)
{
	TNo *p = g->prim_no;

	while(p)
	{
		printf("%d -> ", p->id_no);

		TViz *v = p->prim_viz;

		while(v)
		{
			printf("%d ->", v->id_viz);
			v = v->prox_viz;
		}

		printf("null \n");
		p = p->prox_no;
	}
}

/*
	Verifica se o grafo é orientado ou não.	[Função booleana]
*/
int orientado(TG *g)
{
	TNo *p = g->prim_no;

	while(p)
	{
		TViz *v = p->prim_viz;

		while(v)
		{
			TViz *teste = buscaAresta(g, v->id_viz, p->id_no);

			if(!teste)
				return 1;

			v = v->prox_viz;
		}

		p = p->prox_no;
	}

	return 0;
}

int menu()
{
	printf("OPERACOES COM GRAFO:\n");
	printf("1. Criar\n");
	printf("2. Inserir vertice\n");
	printf("3. Inserir arco\n");
	printf("4. Exibir\n");
}

int main(int argc, char* argv[])
{
	int option = 0;
	int run = 1;

	while(run)
	{


		TG *grafo = create();

		int i = 1;

		while(i < 8)
		{
			insereNo(grafo, i);
			i++;
		}

		insereAresta(grafo, 2, 4, 1);
		//TNo *n=buscaNo(grafo,3);
		//printf("%d\n",n->id_no);
		//insereAresta(grafo,1,3,1)
		//TViz *v = buscaAresta(grafo, 2, 4);
		//printf("%d\n",v->id_viz );
		//TNo *p = buscaNo(grafo, 8);
		//int n = orientado(grafo);
		//printf("%d\n", n);
		print(grafo);

		release(grafo);

	}



	return 0;
}
