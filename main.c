#include <stdio.h> /* I/O */
#include <stdlib.h> /* malloc */
#include "main.h" /* structs */


#define TRUE 1
#define FALSE 0

void pilhaIni(TP* pilha){

	pilha->prox = NULL;

}
void push(TP *pilha, int val){

	TP *novaPilha = (TP*)malloc(sizeof(TP));

	novaPilha->id = val;

	novaPilha->prox = NULL;

	if(pilha->prox == NULL)pilha->prox = novaPilha;

	else{

		TP *p = pilha->prox;

		while(p->prox){

			p = p->prox;

		}

		p->prox = novaPilha;

	}

}
void insertStack(TP * pilha, int val){

	TP *p = pilha->prox;

	while(p){

		if(p->id == val)return;

		p = p->prox;

	}

	push(pilha, val);

}
void showStack(TP *pilha){

	TP *p = pilha->prox;

	printf("\nArticulacoes: (");

	while(p){

		printf("%d", p->id);

		if(p->prox)printf(" ,");

		p = p->prox;

	}

	printf(")");


}
void libera(TP* pilha){

	TP *p = pilha->prox, *temp;

	if(p){

		while(p){

			temp = p;

			p = p->prox;

			free(temp);

		}

		free(pilha);

	}

}
int buscaVisitado(TP *pilha, int val){

	TP *p = pilha->prox;

	for(int i = 1; i <= val; ++i)
	{
		p = p->prox;
	}

	return  p->id;

}
void insereVisitado(TP *pilha, int val){

	TP *p = pilha->prox;

	for(int i = 0; i < val; ++i) {

		p = p->prox;

	}

	p->id = 1;

}
void caminho(TG *g, int id1, TP *pilha){

	insereVisitado(pilha, id1);

	TNo *p = findVertex(g, id1);

	if(p) {

		TViz *v = p->prim_viz;

		while(v) {

			if(buscaVisitado(pilha, v->id_viz) == 0)

				caminho(g, v->id_viz, pilha);

			v = v->prox_viz;

		}

	}

}
int encontraCaminho(TG *g, int id1, int id2){

	TNo *p = g->prim_no;

	int result = 0;

	TP *pilha = (TP*)malloc(sizeof(TP));

	pilhaIni(pilha);

	while(p){

		push(pilha, 0);

		p = p->prox_no;

	}

	caminho(g, id1, pilha);

	result = buscaVisitado(pilha, id2);

	libera(pilha);

	return  result;


}
int findLastId(TViz *v){

	while(v){

		if(!v->prox_viz)return v->id_viz;

		v = v->prox_viz;

	}

	return 0;

}
void achaPontes(TG *g){                     //METODO SIMPLES

	TNo *p = g->prim_no;

	int id1, id2, ultimo;

	while(p){

		id1 = p->id_no;

		TViz *v = p->prim_viz;

		ultimo = findLastId(v);

		while(v){

			TViz *proximo = v->prox_viz;

			id2 = v->id_viz;

			removeEdge(g, id1, id2, 0);

			if(!encontraCaminho(g, id1, id2)){

				if(id1 < id2)

					printf("(%d,%d) e ponte\n", id1, id2);

			}

			insertEdge(g, id1, id2, 0);

			v = proximo;

			if(ultimo == id2)break;

		}

		p = p->prox_no;

	}

}
void achaArticulacao(TG *g){                     //procura e mostras as articulacoes

	TNo *p = g->prim_no, *v1, *v2;

	int id1, id2, ultimo;


	TP *pilha = (TP*)malloc(sizeof(TP));

	pilhaIni(pilha);

	while(p){

		id1 = p->id_no;

		TViz *v = p->prim_viz;

		ultimo = findLastId(v);

		while(v){

			TViz *proximo = v->prox_viz;

			id2 = v->id_viz;

			removeEdge(g, id1, id2, 0);

			if(!encontraCaminho(g, id1, id2)){

				v1 = findVertex(g, id1);

				v2 = findVertex(g, id2);

				if(v1->prim_viz)insertStack(pilha, id1);

				if(v2->prim_viz)insertStack(pilha, id2);

			}

			insertEdge(g, id1, id2, 0);

			v = proximo;

			if(ultimo == id2)break;

		}

		p = p->prox_no;

	}

	showStack(pilha);

	libera(pilha);

}

/* Cria um grafo */
TG* create()
{
	TG * g = (TG*)malloc(sizeof(TG));
	g->prim_no = NULL;

	return g;
}

/* Libera um grafo */
void release(TG *g)
{
	if(g)
	{
		TNo *p = g->prim_no;

		/* Libera os vértices */
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

		/* Libera o grafo */
		free(g);
	}
}

/* Encontra um vertice no grafo */
TNo *findVertex(TG *g, int id)
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

/* Insere um vertice no grafo 
	
	Return:
	TRUE caso seja inserido com sucesso
	FALSE caso já exista esse vertice no grafo
*/
int insertVertex(TG *g, int id)
{
	/* Verifica se o vértice já existe no grafo. Se existir, não inserir novamente */
	if(findVertex(g, id))
		return FALSE;

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

	return TRUE;
}

/* Insere um vertice no grafo

	Return:
	TRUE caso tenha removido com sucesso
	FALSE caso o vertice nao exista no grafo
*/
int removeVertex(TG *g, int id)
{
	TNo *p = g->prim_no;

	TViz *anterior = NULL;

	TNo *anterior_no = NULL;

	while(p && p->id_no != id)
	{
		anterior_no = p;
		p = p->prox_no;
	}

	/* Se p é NULL, não existe o que ser retirado */
	if(p)
	{
		/* Vamos excluir os caminhos de p para outro nos */
		TViz *v = p->prim_viz;

		while(v)
		{
			TViz *aux = v;
			v = v->prox_viz;
			free(aux);
		}

		TNo *q = g->prim_no;

		while(q) // agora vamos excluir caminhos de outros nos ate p
		{
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

			return TRUE;
		}

		anterior_no->prox_no = p->prox_no; //o no anterior a p aponta para o proximo no após p

		free(p);

		return TRUE;
	}

	return FALSE;
}

/* Insere um arco no grafo

	Return:
	TRUE caso insira com sucesso o arco no grafo
	FALSE caso os vertices id1 ou id2 não existam no grafo
*/
int insertEdge(TG *g, int id1, int id2, int orientado)
{
	//id1 é origem, id2 é destino 

	TNo *p = g->prim_no;

	while((p) && (p->id_no != id2))
	{ //acha o nó de destino
		p = p->prox_no;

	}

	if(!p)
		return FALSE;  //se o no de destino não existe, não faz nada

	p = g->prim_no;

	while((p) && (p->id_no != id1))
	{ //acha o nó de origem
		p = p->prox_no;
	}

	if(!p)
		return FALSE;  //se o no de origem não existe, não faz nada

	TViz *novo = (TViz*)malloc(sizeof(TViz));
	novo->id_viz = id2;
	novo->prox_viz = NULL;

	TViz *v = p->prim_viz;

	if(!v)
	{ 		//se não tem primeiro vizinho, o novo é o primeiro agora 
		p->prim_viz = novo;

		if(!orientado)
			insertEdge(g, id2, id1, 1); //se o grafo for nao for orientado

		return TRUE;                                  // acrescenta a volta
	}

	while(v->prox_viz)
	{
		if(v->id_viz == id2)
			return FALSE; //se o caminho ja existe, não faz nada. 
						  //se não for orientado ja existe o caminho id2->id1 também

		v = v->prox_viz;  //acha o ultimo vizinho
	}

	v->prox_viz = novo;   //acrescenta o novo caminho no final da lista de vizinhos

	if(!orientado)
		insertEdge(g, id2, id1, 1); //se não for orientado, faz o caminho id2->id1

	return TRUE;
}

/* Busca um arco no grafo

	Return:
	Um ponteiro válido para o arco caso seja encontrado
	NULL caso o arco não exista
*/
TViz *findEdge(TG *g, int id1, int id2)
{
	TNo *p = findVertex(g, id1);

	TViz *v = p->prim_viz;

	while(v)
	{
		if(v->id_viz == id2)
			break;

		v = v->prox_viz;
	}

	return v;
}

/* Remover arco - Função auxiliar

	variável 'flag' recebe o valor TRUE caso algum arco tenha sido removido

	Return:
	Nova lista de adjacencias sem o vertice a ser removido
	
*/
TViz *_remove(TViz *vertex, int id, int *flag)
{
	if(!vertex)
		return vertex;

	if(vertex->id_viz == id)
	{
		TViz* next = vertex->prox_viz;
		free(vertex);

		*flag = TRUE;

		return next;
	}
	else
	{
		vertex->prox_viz = _remove(vertex->prox_viz, id, flag);

		return vertex;
	}
}

/* Remover arco

	Return:
	TRUE caso tenha removido o arco com sucesso
	FALSE caso o arco a ser removido não exista

*/
int removeEdge(TG *G, int id1, int id2, int isDirected)
{
	TNo *vertex = findVertex(G, id1);

	if(vertex)
	{
		int flag = FALSE;

		vertex->prim_viz = _remove(vertex->prim_viz, id2, &flag);

		if(flag)
		{
			/* Caso o grafo seja orientado, remover a volta também */
			if(isDirected)
			{
				return removeEdge(G, id2, id1, 0);
			}

			return TRUE;
		}
	}

	return FALSE;
}

/* Exibe o grafo na tela */
void print(TG *g)
{
	if(g)
	{
		TNo *p = g->prim_no;

		while(p)
		{
			printf("%d -> ", p->id_no);

			TViz *v = p->prim_viz;

			while(v)
			{
				printf("%d -> ", v->id_viz);
				v = v->prox_viz;
			}

			printf("NULL\n");
			p = p->prox_no;
		}
	}
}





/*  Verifica se o grafo é orientado ou não.

	Return:
	TRUE caso o grafo seja orientado
	FALSE caso não seja
*/

int directed(TG *G)
{
	TNo *p = G->prim_no;

	while(p)
	{
		TViz *v = p->prim_viz;

		while(v)
		{
			if(!findEdge(G, v->id_viz, p->id_no))
				return TRUE;

			v = v->prox_viz;
		}

		p = p->prox_no;
	}

	return FALSE;
}

/**/
int digito_em_comum(int primeiro, int segundo)
{
	int salva = primeiro;

	while((segundo / 10) || (segundo % 10))
	{
		int s = segundo % 10;

		while((primeiro / 10) || (primeiro % 10))
		{
			if(primeiro % 10 == s)
				return TRUE;

			primeiro = primeiro / 10;
		}
		segundo = segundo / 10;

		primeiro = salva;
	}

	return FALSE;
}
int junta_resultado(int primeiro, int segundo)
{
	if(!primeiro)
		return segundo;

	int resp = primeiro;

	while((segundo / 10) || (segundo % 10))
	{
		if(!digito_em_comum(primeiro, segundo % 10))
			resp = (resp * 10) + (segundo % 10);

		segundo = segundo / 10;
	}

	return resp;
}
int sair_chegar(TG *g, int id1, int id2, int caminho)
{
	TNo *p = findVertex(g, id1);

	TViz *v = p->prim_viz;

	while(v)
	{
		//printf("testando caminho de %d e indo para %d\n",id1,v->id_viz);
		if(v->id_viz == id2)
		{
			caminho = junta_resultado(caminho, id1); //a função evita repetições do mesmo nó 
			caminho = junta_resultado(caminho, id2); //na explicitação do caminho
		}
		else
		{
			if(!digito_em_comum(caminho, v->id_viz))
			{	  //se certificando de não entrar em um loop infinito
				int teste = digito_em_comum(caminho, id1);  //se certificando do caminho não ter duas vezes o mesmo nó

				int novo_caminho = 0;

				if(teste)
				{
					novo_caminho = junta_resultado(caminho, id1);
				}
				else
				{
					novo_caminho = caminho * 10 + id1;
				}

				int temp = sair_chegar(g, v->id_viz, id2, novo_caminho); //pega o resultado, se for 0 não existe caminho ate id2 

				if(temp)
				{											//partindo deste v
					caminho = junta_resultado(caminho, temp);
				}
			}
		}

		v = v->prox_viz;
	}

	if(!digito_em_comum(caminho, id1))
		return 0; // se não possui vertice ou nenhum dos vertices o leva ao destino, retorna 0

	return caminho;
}
int fortemente_conexos(TG *g)
{
	int* resp = (int*)malloc(sizeof(int));
	resp[0] = 0;

	int i = 1;

	TNo *no = g->prim_no;

	while(no)
	{
		int no_chegou_em_si_mesmo = sair_chegar(g, no->id_no, no->id_no, 0);  //se certifica que existe uma resposta

		if(no_chegou_em_si_mesmo)
		{
			int j = 1;

			while(j <= i)
			{
				if(digito_em_comum(no_chegou_em_si_mesmo, resp[j - 1]))
				{
					break;
				}

				j++;
			}

			resp = (int*)realloc(resp, sizeof(int) * j);

			resp[j - 1] = junta_resultado(resp[j - 1], sair_chegar(g, no->id_no, no->id_no, 0));

			if(j > i)
			{
				i++;
			}
		}

		no = no->prox_no;
	}

	if(!directed(g) && (i == 1))
	{
		printf("O grafo nao eh orientado e eh conectado!");
		free(resp);

		/* retorna 1 (devem ser chamadas as outras funções) */
		return 1;
	}

	if(directed(g))
		printf("O grafo eh orientado!\nOs componentes fortemente conexos sao:\n");
	else
		printf("O grafo nao eh orientado e nao eh conectado! \nOs componentes conectados sao:\n");

	if(!i)
		printf("\tNenhum\n");

	int j = 1;

	while(j <= i)
	{
		printf("%d\n", resp[j]);
		j++;
	}

	free(resp);

	/* Retorna 0 pois nenhuma outra função precisa ser chamada */
	return 0;
}
int conexo(TG* G)
{
	/* IMPLEMENTAÇÃO */
}
int componentes_conexos(TG* G)
{
	/* IMPLEMENTAÇÃO */
}

void information(TG* G)
{
	if(G)
	{
		printf("GRAFO:\n");

		/* Se esse grafo é orientado ou não. 
		Se for orientado, informe sua(s) componente(s) fortemente conexa(s). 
		Um grafo orientado é fortemente conexo se para qualquer par (v,w) de seus vértices existe um caminho de v a w e também um caminho de w para v.
		*/
		if(directed(G))
		{
			printf("- Orientado\n");
			fortemente_conexos(G);
		}
		/* Se esse grafo não for orientado, verifique se ele é conexo.*/
		else
		{
			printf("- Nao Orientado\n");
			
			/*	Se for conectado, indique as pontes (ponte é uma aresta cuja a remoção desconecta o grafo) e os pontos de articulação 
			(um ponto de articulação é um vértice de um grafo tal que a remoção desse vértice provoca um aumento no número de componentes conectados) existentes no grafo.
			*/
			if(conexo(G))
			{
				achaPontes(G);
				achaArticulacao(G);
			}
			/*
				Se não for conectado, informe os componentes conexos desse grafo.
			*/
			else
			{
				componentes_conexos(G);
			}
		}
	}
}


int menu(TG *G)
{
	int running = TRUE;

	while(running)
	{
		system("clear");
		printf("OPERACOES COM GRAFO:\n");
		printf("1. Exibir Grafo\n");
		printf("2. Exibir Informacoes do Grafo\n");
		printf("3. Inserir Vertice\n");
		printf("4. Inserir Arco\n");
		printf("5. Retirar Vertice\n");
		printf("6. Retirar Arco\n");
		printf("7. Buscar Vertice\n");
		printf("8. Buscar Arco\n");
		printf("9. Sair\n");
		printf("> ");

		int option = 0;
		scanf("%d", &option);

		switch(option)
		{
		case 1: 		print(G); 						break;
		case 2:			information(G);					break;

		case 3:
		{
			printf("\nId do vertice: ");

			int id = 0;

			scanf("%d", &id);
			if(insertVertex(G, id))
				printf("Vertice inserido com sucesso.\n");
			else
				printf("Falha ao inserir vertice.\n");
		}
		break;

		case 4:
		{
			int origin = 0;
			int destiny = 0;

			printf("Origem: ");
			scanf("%d", &origin);

			printf("Destino: ");
			scanf("%d", &destiny);

			if(insertEdge(G, origin, destiny, directed(G)))
				printf("Arco inserido com sucesso.\n");
			else
				printf("Falha ao inserir arco.\n");
		}
		break;

		case 5:
		{
			printf("\nId do vertice: ");

			int id = 0;

			scanf("%d", &id);
			if(removeVertex(G, id))
				printf("Vertice removido com sucesso.\n");
			else
				printf("Falha ao remover vertice.\n");
		}
		break;

		case 6:
		{
			int id1 = 0;
			int id2 = 0;
			printf("Origem: ");
			scanf("%d", &id1);
			printf("Destino: ");
			scanf("%d", &id2);

			if(removeEdge(G, id1, id2, directed(G)))
				printf("Arco removido com sucesso.\n");
			else
				printf("Falha ao remover arco.\n");
		}
		break;

		case 7:
		{
			int id = 0;
			printf("Digite o id do vertice: ");
			scanf("%d", &id);
			TNo* vertex = findVertex(G, id);

			if(vertex)
			{
				printf("\nVertice: %d\nMemoria: %p\n", vertex->id_no, vertex);
			}
			else
			{
				printf("\nVertice nao encontrado!\n");
			}
		}

		break;

		case 8:
		{
			int origin = 0;
			int destiny = 0;

			printf("Origem: ");
			scanf("%d", &origin);

			printf("Destino: ");
			scanf("%d", &destiny);

			TViz *edge = findEdge(G, origin, destiny);

			if(edge)
			{
				printf("Arco existente para %d -> %d.\nMemoria: %p.\n", origin, destiny, edge);
			}
			else
			{
				printf("Nao existe arco do vertice %d ao vertice %d.\n", origin, destiny);
			}
		}
		break;

		case 9:
			return TRUE;

		default:
			printf("Parametro invalido\n");
		}

		/* Esperar o usuario pressionar alguma tecla para prosseguir para a próxima operação */
		while(getchar() != '\n');
		getchar();
	}
}


/*
	G = (V, E)
	Graph = (Vertex, Edges)

	*Um grafo pode ter zero arestas, mas nunca zero vertices

*/
int main(int argc, char* argv[])
{
	/* Verifica se a quantidade de parâmetros é diferente de 2
	Para funcionar precisa ser 2:
	argv[0] = Nome do programa
	argv[1] = Primeiro parâmetro passado ( Nome do arquivo a ser aberto )
	*/
	if(argc != 2)
	{
		printf("Entrada invalida\n");
		return 1;
	}

	/* Abre o arquivo no modo leitura (r = read) */
	FILE * fp = fopen(argv[1], "r");

	/* Se fp for igual a zero, significa que não foi possível abrir o arquivo */
	if(!fp)
	{
		printf("Falha ao abrir o arquivo %s\n", argv[1]);
		return 1;
	}

	/* Lê quantos vértices há no grafo */
	int V = 0;
	fscanf(fp, "%d", &V);

	/* Caso não tenha vertices, terminar o programa */
	if(V <= 0)
	{
		printf("Um grafo precisa ter ao menos um vertice.\n");
		fclose(fp);
		return 1;
	}

	/* Cria um grafo vazio */
	TG *G = create();

	/* Insere os vertices no grafo */
	int i = 0;
	for(i = 0; i < V; i++)
		insertVertex(G, i + 1);

	/* Insere os arcos no grafo */
	int origin = 0;
	int destiny = 0;

	fscanf(fp, "%d %d", &origin, &destiny);

	while(!feof(fp))
	{
		insertEdge(G, origin, destiny, 1);
		fscanf(fp, "%d %d", &origin, &destiny);
	}

	/* Exibe o menu e só retorna quando for para encerrar o programa */
	menu(G);

	/* Libera os vertices e o grafo */
	release(G);

	/* Fecha o arquivo */
	fclose(fp);

	return 0;
}
