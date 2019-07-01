#include "Grafo.h"

/*
 * grafo.c
 *
 *  Created on: Nov 18, 2016
 *      Author: Renan Augusto Starke
 */

#include <stdio.h>
#include <string.h>

#include "grafo.h"
#define MAX_VERTICES 22

struct vertices
{
    int id;         /*!< Identifica��o num�rica do v�rtice  */

    /* Mais informa��es, se necess�rio */
};

struct arestas
{
    int adj;        /*!< Valor booleando. Verdadeiro representa uma adjac�ncia entre dois v�rtices  */
    void *chave; /*! alteracao */
};

struct grafos
{
    int n_vertices;        /*!< N�mero de v�rtices do grafo  */
    vertice_t *vertices;   /*!< Vetor de ponteiros: conjunto V */
    aresta_t **matriz_adj;	/*< Matriz de adjac�ncia: conjunto E */
};

void *ar_get_chave(grafo_t *g,int a,int b)
{


    return g->matriz_adj[a][b].chave;
}

void ar_set_chave(grafo_t *g,void *key, int a, int b)
{
    g->matriz_adj[a][b].chave=key;




}





/**
  * @brief  Cria uma novo grafo est�tico
  * @param	vertices: quantidade de v�rtices
  *
  * @retval grafo_t: ponteiro para um novo grafo
  */
grafo_t *cria_grafo(int vertices)
{
    int i;
    aresta_t **matriz_adj;
    /* Aloca estrutura do grafo */
    grafo_t *g = malloc(sizeof(grafo_t));

    if (g == NULL)
    {
        perror("cria_grafo (g)");
        exit(EXIT_FAILURE);
    }

    /* Guarda n�mero total de v�rtices */
    g->n_vertices = vertices;
    /* Aloca v�rtices */
    g->vertices = malloc(vertices * sizeof(vertice_t));

    if (g->vertices == NULL)
    {
        perror("cria_grafo (vertices)");
        exit(EXIT_FAILURE);
    }

    /* Zera vetor de v�rtices */
    memset(g->vertices, 0, vertices * sizeof(vertice_t));

    /* Aloca 1a dimens�o da matriz de adjac�ncia */
    matriz_adj = malloc(vertices * sizeof(aresta_t *));

    if (matriz_adj == NULL)
    {
        perror("cria_grafo (matriz_adj)");
        exit(EXIT_FAILURE);
    }

    /* Aloca 2a dimens�o da matriz de adjac�ncia */
    for ( i = 0; i < vertices; i++ )
    {
        matriz_adj[i] = calloc(vertices, sizeof(aresta_t));

        if (matriz_adj[i] == NULL)
        {
            perror("cria_grafo (matriz_adj[i])");
            exit(EXIT_FAILURE);
        }
    }

    g->matriz_adj = matriz_adj;

    return g;
}

/**
  * @brief  Libera a mem�ria utilizada pelo grafo
  * @param	Nenhum
  *
  * @retval Nenhum
  */
void libera_grafo (grafo_t *g)
{
    int i;

    if (g == NULL)
    {
        perror("libera_grafo");
        exit(EXIT_FAILURE);
    }

    for (i=0; i < g->n_vertices; i++)
        free(g->matriz_adj[i]);

    free(g->matriz_adj);
    free(g->vertices);
    free(g);
}

/**
  * @brief  Cria adjac�ncia entre v�rtices u e v
  * @param	u: �ndice do v�rtice u
  * @param  v: �ndice do v�rtice v
  *
  * @retval int: verdadeiro se adjac�ncia for criada
  */
int cria_adjacencia(grafo_t *g, int u, int v)
{

    if (g == NULL)
    {
        return FALSE;
    }

    if (u > g->n_vertices || v > g->n_vertices )
        return FALSE;

    g->matriz_adj[u][v].adj = TRUE;

    return TRUE;
}

/**
  * @brief  Remove adjac�ncia entre v�rtices u e v
  * @param	u: �ndice do v�rtice u
  * @param  v: �ndice do v�rtice v
  *
  * @retval int: verdadeiro se adjac�ncia for removida
  */
int rem_adjacencia(grafo_t *g, int u, int v)
{

    if (g == NULL)
    {
        return FALSE;
    }

    if (u > g->n_vertices || v > g->n_vertices)
        return FALSE;

    g->matriz_adj[u][v].adj = FALSE;

    return TRUE;
}

/**
  * @brief  Retorna adjac�ncia entre v�rtices u e v
  * @param	u: �ndice do v�rtice u
  * @param  v: �ndice do v�rtice v
  *
  * @retval int: verdadeiro se u for adjacente a v
  */
int adjacente(grafo_t *g, int u, int v)
{

    if (u > MAX_VERTICES || v > MAX_VERTICES)
        return FALSE;

    return ((g->matriz_adj[u][v].adj));
}
/**
  * @brief  Retorna o numero adjac�ncias no v�rticev
  * @param  v: �ndice do v�rtice v
  *
  * @retval int: verdadeiro se u for adjacente a v
  */
int adjacencias(grafo_t *g,int u)
{
    int i;
    int s=0;
    for(i=0; i<g->n_vertices; i++)
    {
        if(adjacente(g,u,i))
            s++;

    }
    return s;
}

int  next_branchout(grafo_t *g,int inicial,int caminho,tipo_t *res_atual,tipo_t* (*aresta_sum)(void *))
{
    int a,i,c;

    c=adjacencias(g,caminho);
    if(res_atual!=NULL)
    addup(res_atual,aresta_sum((g->matriz_adj[inicial][caminho].chave)));

    if(c<2)
        return -1;
    else if(c==2)
    {
        for(i=0; i<g->n_vertices; i++)
        {

            if(adjacente(g,caminho,i) && i!=inicial)
            {
                a=next_branchout(g,caminho,i,res_atual,aresta_sum);
            }
        }
    }
    else if(c>2)
    {

        return caminho;
    }

    return a;
}
void exportar_grafo_a(grafo_t *g,FILE *fp,void (*printkey)(void *,
                                                           FILE *)){

int i,j;

    for (i=0; i < g->n_vertices; i++){
        for (j=0; j < g->n_vertices && j<=i; j++){
            if (adjacente(g,i,j)){

                fprintf(fp, "%d -> %d", j, i);
                 printf("a");
                printkey(g->matriz_adj[i][j].chave,fp);
                fprintf(fp,"\n");

            }
        }
    }
    fprintf(fp, "}");
    fclose (fp);





}

