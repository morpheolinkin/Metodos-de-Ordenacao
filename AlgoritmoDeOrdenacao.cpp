# Metodos-de-Ordenacao
Ordenação em C

/*
* Autor: Fábio Cerqueira 
* Uso: ./ordena quantidade tipo
* Exemplo: ./ordena 1000 1
* Os tipo são: {1 = Aleatório,2 = Decrescente, 3 = Crescente}
* Descrição: Programa ordena vetores simples em C gerados de acordo com a quantidade e 
* tipo passado por linha de comando, caso tamanho seja menor que 100 gera um arquivo 
* saida.txt com os vetores ordenados. Exibe o tempo levado em ms para cada algoritmos de 
* ordenação para ser feita a comparação.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Definições */
#define ALEATORIO 1
#define DECRESCENTE 2
#define CRESCENTE 3

#define TIPO_VETOR(n) n == 1?"Aleatório":n == 2?"Decrescente":"Crescente"


/* Protótipos */
void ordena(void (*sort)(int *,int),int *V,int n,char *nome,int tipoVetor);
void gerarVetor(int *vet, int tipoVet, int n);
void printV(FILE *saida,int *V,int n); 
void swap(int *x,int *y);
void copy(int *V,int *C,int n);

void bubbleFlagSort(int *V,int n);
void insertionSort(int *V,int n);
void selectionSort(int *V,int n);
void shellSort(int *V,int n);
void merge(int *V,int ini,int meio,int fim); /* para mergeSort */
void mergeSort(int *V,int n); /* Wrapper */
void mergeSortR(int *V,int ini,int fim);
int  particao(int *V,int ini,int fim); /* para quickSort */
void quickSort(int *V,int n); /* Wrapper */
void quickSortR(int *V,int ini,int fim);


int main(int argc, char **argv) {
    int n = 0;
    int tipoVetor = 0;
    int *vetor;
    FILE *saida;
        
    /* Validação simples para quantidade de argumento */
    if (argc != 3)  {
        printf("Erro de entrada.\n\tUse: ordena quantidade(n > 0) tipo(1,2,3)\n\tEx: ordena 1000 1\n");
        exit(1);
    }

    /* Pegando os valores da linha de comando*/
    n = atoi(argv[1]);
    tipoVetor = atoi(argv[2]);
    
    /* Limpando arquivo para n < 100 */
    if (n < 100) {
        saida = fopen("saida.txt","w");
        if (saida == NULL) {
            printf("Não foi possível abrir o arquivo.");
            exit(1);
        }
        fflush(saida);
        fclose(saida);
    }

    /* Alocando o vetor para ordenação e uma cópia. */
    vetor = (int *) malloc(n * sizeof(int));

    /* Gerando números para vetor */ 
    gerarVetor(vetor,tipoVetor,n);
   

    /* Chamada para o método bubbleFlag */
    ordena(bubbleFlagSort, vetor, n,"Bolha com Flag",tipoVetor);

    /* Chamada para o método insertion */
    ordena(insertionSort, vetor, n,"Inserção",tipoVetor);
    
    /* Chamada para o método selection */
    ordena(selectionSort, vetor, n, "Seleção",tipoVetor); 
    
    /* Chamada para o método shell */
    ordena(shellSort, vetor, n, "Shell",tipoVetor); 
    
    /* Chamada para o método merge */
    ordena(mergeSort, vetor, n, "Merge",tipoVetor); 
 
    /* Chamada para o método quick */
    ordena(quickSort,vetor,n,"Quick",tipoVetor); 

    /* Liberando memória alocada*/
    free(vetor);


    return 0;
}

/*
 * Chama as funções de ordenação e exibe resultados na saída padrão 
 * caso int n menor que 100 ele também escreve no arquivo saida.txt
 * void (*sort)(int *,int) - Ponteiro para função do método de ordenação
 * int *V                  - Vetor para ser ordenado
 * int n                   - Tamanho do Vetor
 * char *nome              - String com o nome do método para exibição no arquivo.
 * int tipoVetor           - Tipo de vetor(Aleatório, Decrescente, Crescente)
 */
void ordena(void (*sort)(int *,int),int *V,int n,char *nome,int tipoVetor) {
    double tini,tfim,tempo;
    int *cVetor = (int *) malloc(n * sizeof(int));

    copy(cVetor,V,n); /* Faz backup de V para poder usá-lo em várias chamadas */
    
    printf("%s (%d elementos - %s)\n", nome, n, TIPO_VETOR(tipoVetor));
    if (n < 100) {
        FILE *saida;
        saida = fopen("saida.txt","a");
        if (saida == NULL) {
            printf("Não foi possível abrir o arquivo.");
            exit(1);
        }
        fprintf(saida,"%s\nAntes: ",nome);
        printV(saida,cVetor,n);
        tini = (double)clock() / CLOCKS_PER_SEC * 1000;
        sort(cVetor,n);
        tfim = (double)clock() / CLOCKS_PER_SEC * 1000;
        tempo = (double) tfim - tini;
        fprintf(saida,"\nDepois: ");
        printV(saida,cVetor,n);
        fprintf(saida,"\n\n");
        fclose(saida);
    }
    
    else {
        tini = (double)clock() / CLOCKS_PER_SEC * 1000;
        sort(cVetor,n);
        tfim = (double)clock() / CLOCKS_PER_SEC * 1000;
        tempo = (double) tfim - tini;
    }
    
    printf("Tempo: %2.0lf ms\n\n", tempo);
    
    free(cVetor);
}

/*
 * Imprime na tela o vetor V no formato [1,2,3,4,5,6]
 * int *V - Vetor de inteiros
 * int n - Tamanho do vetor
 */
void printV(FILE *saida,int *V,int n) {
    int i = 0;
    if (saida != NULL) 
       for (i = 0;i < n;i++)
           fprintf(saida,"%d ", V[i]);
}

/*
 * Copia o vetor C para o Vetor V
 * int *V - Vetor para onde vai a cópia
 * int *C - Vetor que vai ser copiado
 * int n - Tamanho dos vetores
 */
void copy(int *V,int *C,int n) {
    int i = 0;
    for (i = 0;i < n;i++)
        V[i] = C[i];
}


/*
 * Gerador de conteúdo vetor de acordo com o tipo passado no parâmetro tipoVet.
 * Preenche o vetor vet e cVet com os mesmos valores.
 * int *vet - Vetor principal
 * int *cvet - Vetor para cópia
 * int tipoVet - Tipo de preechimento (ALEATORIO,CRESCENTE,DECRESCENTE)
 * int n - tamanho do vetor
 */
void gerarVetor(int *vet, int tipoVet, int n) {
    int i = 0;

    switch (tipoVet) {
        case ALEATORIO:
            /* inicializando a semente */
            srand(time(NULL));
            for (i = 0;i < n;i++) 
                vet[i] = rand() % (n * 10);

            break;

        case CRESCENTE:
            for (i = 0;i < n;i++) 
                vet[i] = i;
            break;

        case DECRESCENTE:
            for (i = 0;i < n;i++)
                vet[i] = n - i - 1;
            break;

    }
}


void swap(int *x,int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;

}

/*********************************
 * Métodos de ordenação
 *********************************/

void bubbleFlagSort(int *V,int n) {
    int flag;
    int j = 0;
    int i = 0;
    
    do {
        flag = 0;
        for (i = 0;i < n - 1 - j;i++) { 
	        if (V[i] > V[i+1]) {	
        	    swap(&amp;V[i],&amp;V[i+1]);
                flag = 1;
            }
        }
	    j++;	
    } while (flag);
}

void insertionSort(int *V,int n) {
    int i, j, pivo;
    
    for (i = 1;i < n;i++) {
        pivo = V[i];
        j = i - 1;
        while ((j >= 0) &amp;&amp; (V[j] > pivo)) {
            V[j + 1] = V[j];
            j--;
        }
        V[j + 1] = pivo; 
    }
}


void selectionSort(int *V,int n) {
    int i, j, min;
    
    for (i = 0;i  < n-1;i++) {
        min = i;

        for (j = i + 1;j < n;j++)
            if (V[j] < V[min])  
                min = j;
	    
        swap(&amp;V[i],&amp;V[min]);
    }
}


void shellSort(int *V,int n) {
    int h = 1; 
    int i, j, pivo; /* Para insertion */
    
    do
        h = (3 * h) + 1;
    while (h < n);

    do { 
        h /= 3;
        /* insertion */
        for (i = h;i < n;i++) {
            pivo = V[i];
            j = i - h;
            while ((j >= 0) &amp;&amp; (pivo < V[j])) {
		        V[j + h] = V[j];
                j = j - h;
            }
            V[j + h] = pivo;
        }
        /* insertion */

    } while (h > 1);
}

/*************************
 * MergeSort
 *************************/
void merge(int *V,int ini,int meio,int fim) {
    int i = ini;
    int j = meio + 1;
    int k = 0;
    int aux[fim - ini + 1];
    
    while ((i <= meio) &amp;&amp; (j <= fim)) {
    	if (V[i] <= V[j]) {
            aux[k] = V[i];
            i++;
        }
        else {
            aux[k] = V[j];
            j++;
        }
        k++;
    }

    if (i <= meio)  {
        j = meio;
        while (j >= i) {
            V[fim - meio + j] = V[j];
            j--;
        }
    }

    for (i = 0;i < k;i++) 
        V[ini + i] = aux[i];
    

}

void mergeSort(int *V,int n) {
    mergeSortR(V,0,n-1);
}

void mergeSortR(int *V,int ini,int fim) {
    int meio;

    if (ini < fim) {
        meio = (ini + fim) / 2;
        if (ini < meio) 
            mergeSortR(V,ini,meio);
        if ((meio + 1) < fim) 
            mergeSortR(V,meio + 1,fim);
        
        merge(V,ini,meio,fim);
    }
}

/*********************************
 * QuickSort
 ********************************/
int particao(int *V,int ini,int fim) {
    int pivo = V[ini];
    int i = ini + 1;
    int j = fim;
    
    while (i <= j) {
        while ((i <= j) &amp;&amp; (V[i] <= pivo)) 
	        i++;
	    
        while (V[j] > pivo) 
            j--;
        
        if (i < j) {
            swap(&amp;V[i],&amp;V[j]);
            i++;
            j--;
        }
    }
    swap(&amp;V[ini],&amp;V[j]);
    return j;
}

void quickSort(int *V,int n) {
    quickSortR(V,0,n-1);
}

void quickSortR(int *V,int ini,int fim) {
    int j;
    if (ini < fim) {
        j = particao(V,ini,fim);
        if (ini < (j - 1))
            quickSortR(V,ini,j - 1);
        if ((j + 1) < fim) 
            quickSortR(V,j + 1,fim);
    }
    
}
