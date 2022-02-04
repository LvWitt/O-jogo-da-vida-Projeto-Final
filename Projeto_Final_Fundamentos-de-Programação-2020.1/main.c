#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define ORG 'X'
#define VAZ ' '
#define PAR '*'
#define TAM 101
#define LARGTABU 60
#define ALTUTABU 20

typedef struct tab
{
    char nomeJogo[TAM];
    int dim1, dim2;
    char **mat;
    int ciclosVida;
} Tab;

///MANIPULA_ARQUIVOS////////////////////////////////////////////////////////////////////////////

char** LeArq(FILE *fp, int nL, int nC, int *invasao);

///FORMATOS_DE_VIDA/////////////////////////////////////////////////////////////////////////////

void iniciaBloco(Tab *jogo);
void iniciaVidaEterna2(Tab *jogo);
void iniciaBlinker (Tab *jogo);
void iniciaSapo (Tab *jogo);
void iniciaGlider(Tab *jogo, int *invasao);
void iniciaLightweight_Spaceship(Tab *jogo);

///FUNÇÕES_AUXILIARES///////////////////////////////////////////////////////////////////////////

void limpaMatriz (char** mat, int nL, int nC);
void copiaMatriz(char**m1, char** m2, Tab *jogo);
char** alocaMatriz(int nL, int nC);
void desalocaMatriz(char** mat, int nL);
void posiAleatoria(int *xInc, int *yInc);
void imprimeMatriz(char **mat, int nL, int nC);
void randomico(int numSort, Tab *jogo);

///MODOS_DE_JOGO////////////////////////////////////////////////////////////////////////////////

void modoLivre(Tab *jogo);
void modoRandomico(Tab *jogo);
void modoInvasao(Tab *jogo);

///APLICAÇÃO_NO_JOGO////////////////////////////////////////////////////////////////////////////////

void analisaEntorno(Tab *jogo, char **mJ, int k_linha, int j_coluna, int *vivos);

///GERENCIAMENTO_DO_JOGO///////////////////////////////////////////////////////////////////////

void jogaJogoVida (Tab *jogo);
void menuInicJogo(Tab *jogo1, Tab *jogo2, Tab *jogo3, int *sair);

//////////////////////////////////////////////////////////////////////////////////////////////

char** LeArq(FILE *fp, int nL, int nC, int *invasao) /* Leitura do arquivo csv e armazenamento do conteúdo em uma matriz */
{
    char line[1025], *token;
    char **m;
    int l, c;

    m = alocaMatriz(nL, nC);
    limpaMatriz(m, nL, nC);

    if (fp == NULL)
    {
        printf("Erro de arquivo\n");
        exit(1);
    }

    fgets(line, 1025, fp);
    while (!feof(fp))
    {
        token = strtok(line, ",");
        l = atoi(token) - 1;
        while (token != NULL)
        {
            token = strtok(NULL, ",");
            c = atoi(token) - 1;
            m[l][c] = ORG;
        }
        fgets(line, 1025, fp);
    }

    fclose(fp);
    return m;
}

/////////////////////////FORMATOS_DE_VIDA///////////////////////////
/* Carrega os formatos de vida usados no jogo a partir de arquivos csv */

void iniciaBloco(Tab *jogo)
{
    FILE *fp;
    fp = fopen("Bloco.csv", "r");

    char **padrao = LeArq(fp, 2, 2, 0);
    int i, j, xInic, yInic;

    posiAleatoria(&xInic, &yInic);

    for (i = 0; i < 2; i++)
        for(j = 0; j < 2; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];

    desalocaMatriz(padrao, 2);
}

void iniciaVidaEterna2(Tab *jogo)
{
    int i, j, xInic, yInic;
    FILE *fp;
    fp = fopen("VidaEterna2.csv", "r");

    char **padrao = LeArq(fp, 2, 3, 0);

    posiAleatoria(&xInic, &yInic);

    for (i = 0; i < 2; i++)
        for(j = 0; j < 3; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];
    desalocaMatriz(padrao, 1);
}

void iniciaBlinker (Tab *jogo)
{
    int i, j, xInic, yInic;
    FILE *fp;
    fp = fopen("Blinker.csv", "r");

    char **padrao = LeArq(fp, 1, 3, 0);

    posiAleatoria(&xInic, &yInic);

    for (i = 0; i < 1; i++)
        for(j = 0; j < 3; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];
    desalocaMatriz(padrao, 1);
}

void iniciaSapo (Tab *jogo)
{
    FILE *fp;
    fp = fopen("Sapo.csv", "r");

    char **padrao = LeArq(fp, 2, 4, 0);
    int i, j, xInic, yInic;

    posiAleatoria(&xInic, &yInic);

    for(i = 0; i < 2; i++)
        for(j = 0; j < 4; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];
   desalocaMatriz(padrao, 2);
}


void iniciaGlider(Tab *jogo, int *invasao)
{
    FILE *fp;
    fp = fopen("Glider.csv", "r");

    char **padrao = LeArq(fp, 3, 3, invasao);
    int i, j, xInic, yInic;

    if (*invasao == 1)
    {
        xInic = 14;
        yInic = 30;
    }
    else
        posiAleatoria(&xInic, &yInic);

    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];
    desalocaMatriz(padrao, 3);
}

void iniciaLightweight_Spaceship(Tab *jogo) //spaceship
{
    FILE *fp;
    fp = fopen("LWSS.csv", "r");

    char **padrao = LeArq(fp, 4, 5, 0);
    int i, j, xInic, yInic;

    posiAleatoria(&xInic, &yInic);
    /* xInic = nL - 5; */
    yInic = yInic - 1;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 5; j++)
            jogo->mat[xInic + i][yInic + j] = padrao[i][j];

    desalocaMatriz(padrao, 4);
}

////////////////FUNCOES_AUXILIARES/////////////////////////

void limpaMatriz (char** mat, int nL, int nC)
{
    int i, j;
    for ( i = 0; i < nL; i++)
        for (j = 0; j < nC; j++)
            mat[i][j] = VAZ;
}

void copiaMatriz(char**m1, char** m2, Tab *jogo)
{
    for (int l = 0; l < jogo->dim1; l++)
    {
        for (int c = 0; c < jogo->dim2; c++)
        {
            m2[l][c] = m1[l][c];
        }
    }
}

char** alocaMatriz(int nL, int nC)
{
    char **matriz;
    matriz = (char **) malloc(nL * sizeof(char *));

    for (int c = 0; c < nL; c++)
        matriz[c] = (char *) malloc(nC * sizeof(char));

    limpaMatriz(matriz, nL, nC);

    return matriz;

}

void desalocaMatriz(char **mat, int nL)
{
    for (int l = 0; l < nL; l++)
    {
        free(mat[l]);
    }
    free(mat);
}

void posiAleatoria(int *xInc, int *yInc)
{
    srand(time(NULL));
    *xInc = rand() % 18;
    *yInc = rand() % 55;
}

void imprimeMatriz(char **mat, int nL, int nC) /* Imprime o tabuleiro do jogo */
{
    system("cls");
    printf("\n\n");
    printf("                  ______________________________________________________________\n");
    for (int l = 0; l < nL; l++)
    {   printf("                  |");
        for (int c = 0; c < nC; c++)
        {
            printf("%c", mat[l][c]);
        }
        printf("|");
        printf("\n");
    }
    printf("                  ______________________________________________________________\n");
}

void randomico(int numSort, Tab *jogo) /* Função responsável pelo modo randômico do jogo */
{
  int invasor = 0;
  switch(numSort)
  {
    case 1:   iniciaBloco(jogo);                 break;
    case 2:   iniciaVidaEterna2(jogo);           break;
    case 3:   iniciaBlinker(jogo);               break;
    case 4:   iniciaSapo(jogo);                  break;
    case 5:   iniciaGlider(jogo, &invasor);      break;
    case 6:   iniciaLightweight_Spaceship(jogo); break;
  }
}
////////////////FIM_FUNCOES_AUXILIARES////////////////////


////////////////APLICAÇÃO_NO_JOGO/////////////////////////
/* Funções que fazem o jogo funcionar de acordo com suas regras estabelecidas */

void analisaEntorno(Tab *jogo, char **mJ, int k_linha, int j_coluna, int *vivos) /* Aplica as regras em cada célula ao analisar seu entorno */
{
    int contaORG = 0;

    if(mJ[k_linha][j_coluna] == ORG)
    {
      contaORG = -1;
    }


    for(int i = k_linha - 1; i <= k_linha + 1; i++)
    {
        for(int j = j_coluna - 1; j <= j_coluna + 1; j++)
        {
           if(i >= 0 && j >= 0 && i < jogo->dim1 && j < jogo->dim2)
            {
                if(jogo->mat[i][j] == ORG)
                contaORG++;
            }
        }
    }

    if (jogo->mat[k_linha][j_coluna] == ORG)
    {
        (*vivos)++;
    }

    if(mJ[k_linha][j_coluna] != PAR)
    {

        if(contaORG <= 1)
        {
            mJ[k_linha][j_coluna] = VAZ;
        }

        if(contaORG >= 4)
        {
            mJ[k_linha][j_coluna] = VAZ;
        }
    }

    if(contaORG == 3)
    {
        mJ[k_linha][j_coluna] = ORG;
    }
}

////////////////FIM_APLICAÇÃO_DO_JOGO/////////////////////

///////////////////MODOS_DE_JOGO//////////////////////////

void modoLivre(Tab *jogo1)
{
    int opcao, add = 1, invasor = 0;

    do
    {
      printf("Escolha um formato de vida:\n");
      printf("-Vidas paradas-\n   (1) Vida eterna 1 (Bloco) (2) Vida eterna 2\n-Osciladores- \n   (3) Blinker               (4) Sapo\n-Naves espaciais-\n   (5) Glider                (6) Lightweight spaceship\nEntre com  uma opcao: ");

      scanf("%d", &opcao);
      system("cls");

      switch(opcao)
      {
        case 1:   iniciaBloco(jogo1);                 break;
        case 2:   iniciaVidaEterna2(jogo1);           break;
        case 3:   iniciaBlinker(jogo1);               break;
        case 4:   iniciaSapo(jogo1);                  break;
        case 5:   iniciaGlider(jogo1, &invasor);      break;
        case 6:   iniciaLightweight_Spaceship(jogo1); break;
        default: printf("ERRO: Opcao invalida.");
      }

      imprimeMatriz(jogo1->mat, jogo1->dim1, jogo1->dim2);

      printf("Digite '1' para adicionar mais um formato ou '0' para continuar.\n");
      scanf("%d", &add);
    }
    while(add != 0);

    jogaJogoVida(jogo1);
}

void modoRandomico(Tab *jogo1)
{
    int numRandom;
    srand(time(NULL));
    printf("Carregando Modo Randomico...\n");
    for (int i = 0; i <= 20; i++)
    {
      Sleep(1000);
      numRandom = (rand() % 5) + 1;
      randomico(numRandom, jogo1);
    }

    jogaJogoVida(jogo1);
}

void modoInvasao(Tab *jogo2)
{
    int invasor = 1;

    iniciaGlider(jogo2, &invasor);

    for(int i = 0; i < jogo2->dim1; i++)
    {
        for(int j = 0; j < jogo2->dim2/2; j++)
        {
            jogo2->mat[i][j] = PAR;
        }
    }

    jogaJogoVida(jogo2);
}

////////////////FIM_MODOS_DE_JOGO/////////////////////////

////////////////GERENCIAMENTO_DO_JOGO/////////////////////

void jogaJogoVida (Tab *jogo) /* processamento principal do jogo */
{
    char **aux;
    int add, vivos = 0;

    aux = alocaMatriz(jogo->dim1, jogo->dim2);

    printf("Digite o numero de geracoes: ");
    scanf("%d", &jogo->ciclosVida);

    for (int i = 0; i < jogo->ciclosVida; i ++) /* Funciona por X ciclos de vida, exibe a interface do jogo e aplica todas as regras de funcionamento */
    {
        copiaMatriz(jogo->mat, aux, jogo);

        vivos = 0;

        for (int j = 0; j < jogo->dim1; j++)
        {
            for (int k = 0; k < jogo->dim2; k++)
            {
                analisaEntorno(jogo, aux, j, k, &vivos);
            }
        }
        copiaMatriz(aux, jogo->mat, jogo);
        imprimeMatriz(aux, jogo->dim1, jogo->dim2);

        printf("                                      ");
        printf("Geracao: %d Vidas: %d\n", i + 1, vivos);

        Sleep(700);

        if (i + 1 == jogo->ciclosVida) /* Se estiver no penúltimo ciclo */
        {
            printf("Geracao concluida, deseja continuar?\nSe sim digite por quantas geracoes ou '0' para sair.\n");
            scanf("%d", &add);

            jogo->ciclosVida = jogo->ciclosVida + add;
        }
    }

    system("cls");

    desalocaMatriz(aux, jogo->dim1);
}

void menuInicJogo(Tab *jogo1, Tab *jogo2, Tab *jogo3, int *sair)
{
    int opcao;
    do
    {
        system("cls");
        printf("Modos de jogo:\n\n");
        printf("(1) %s\n(2) %s\n(3) %s \n(4) Sair\n\n", jogo1->nomeJogo, jogo2->nomeJogo, jogo3->nomeJogo);
        printf("Escolha um modo de jogo: ");
        scanf("%d", &opcao);
    }
    while(opcao < 1 ||opcao > 4);

    system("cls");

    switch(opcao)
    {
      case 1: modoLivre(jogo1);     break;
      case 2: modoRandomico(jogo2); break;
      case 3: modoInvasao(jogo3);   break;
      case 4: *sair = 1; break;
    }
}

////////////////FIM_GERENCIAMENTO_DO_JOGO/////////////////////

int main()
{
    int sair;

    Tab jogo1, jogo2, jogo3;

    /* Configuração Padrão */

    strcpy(jogo1.nomeJogo, "Livre");
    jogo1.dim1 = ALTUTABU;
    jogo1.dim2 = LARGTABU;
    jogo1.mat = alocaMatriz(jogo1.dim1, jogo1.dim2);

    strcpy(jogo2.nomeJogo, "Randomico");
    jogo2.dim1 = ALTUTABU;
    jogo2.dim2 = LARGTABU;
    jogo2.mat = alocaMatriz(jogo2.dim1, jogo2.dim2);

    strcpy(jogo3.nomeJogo, "Invasao");
    jogo3.dim1 = ALTUTABU;
    jogo3.dim2 = LARGTABU;
    jogo3.mat = alocaMatriz(jogo3.dim1, jogo3.dim2);

    do
    {
        sair = 0;
        printf("-----JOGO DA VIDA by john conway-----\n\n");
        menuInicJogo(&jogo1, &jogo2, &jogo3, &sair);

        system("cls");

        limpaMatriz(jogo1.mat, jogo1.dim1, jogo1.dim2);
        limpaMatriz(jogo2.mat, jogo2.dim1, jogo2.dim2);
        limpaMatriz(jogo3.mat, jogo3.dim1, jogo3.dim2);

    }
    while(sair == 0);

    desalocaMatriz(jogo1.mat, jogo1.dim1);
    desalocaMatriz(jogo2.mat, jogo2.dim1);
    desalocaMatriz(jogo3.mat, jogo3.dim1);

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
