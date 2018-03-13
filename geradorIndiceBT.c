/*
     Segundo Trabalho de Organização de Arquivos - Professor Camilo Chang Dorea
     Data: 24/09/2012

     Alunos: Hércules Silva Nunes - Matrícula: 11/0070747
             Mateus Mendelson Esteves da Silva - Matrícula: 11/0017579
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 3
#define MM 6

typedef struct Reg
{
  char Chave[31];
} Registro;

typedef struct Pagina *Ponteiro;
typedef struct Pagina
{
  int n;
  int index;
  Registro Regs[MM];
  Ponteiro ponteiros[MM+1];
} Pagina;

/* Função principal de inserção */
void Insere(Registro Reg, Ponteiro *ptr);
/* verifica a maior chave entre dois registros */
int verificaMaior(Registro reg1,Registro reg2);
/* Insere um registro na chave */
void InsereNapagina(Ponteiro ptr, Registro Reg, Ponteiro ptr_Dir);
/* Controla a logica de inserção */
void Ins(Registro Reg, Ponteiro ptr, int *Cresceu, Registro *RegRetorno, Ponteiro *ptrRetorno);
/* Imprime a arvore */
void Imprime(Ponteiro);
/* obtem o tamanho dos registros da lista */
long int obter_tam_registro(FILE *fonte);
/* Gera uma arvore b a partir da lista dada */
void gerar_arvore_b (Ponteiro *ptr);

void indexARV(Ponteiro ptr, int *index);
/* Grava a árvore-B no arquivo de saída */
void gravar_arquivo (Ponteiro, FILE *);

int main()
{
    int index =  0;
    int opcao;
    Ponteiro *arv;
    Registro reg;
    int i;
    int tam;
    char matricula[7];
    char nome[41];
    char op[6];
    char curso[10];
    char turma[3];
    FILE *arquivo;
    FILE *saida = NULL;
    Registro *novo_reg = NULL;

    //Inicializando o ponteiro raiz da árvore-B
    arv=(Ponteiro*) malloc(sizeof(Ponteiro));
    *arv = NULL;

    //Gerando a árvore-B inicial (com os registros no arquivo de entrada
    gerar_arvore_b(arv);
    indexARV(*arv,&index);
    saida = fopen ("indicelista.bt", "w");
    gravar_arquivo (*arv, saida);
    fclose (saida);

    do
    {
        printf ("***********\n   Menu\n***********\n");
        printf ("1) Inserir novo registro\n");
        printf ("2) Visualizar arvore-B atual\n");
        printf ("3) Sair\n");
        printf ("\nObs.: A arvore-B ja foi gerada.\n");
        printf ("\nOpcao: ");
        scanf ("%d", &opcao);
        getchar ();

        switch (opcao)
        {
            case 1:
                novo_reg = (Registro *) malloc (sizeof (Registro));

                for (i = 0; i < 6; i++)
                {
                    matricula[i] = ' ';
                }
                matricula[i] = '\0';

                for (i = 0; i < 40; i++)
                {
                    nome[i] = ' ';
                }
                nome[i] = '\0';

                for (i = 0; i < 5; i++)
                {
                    op[i] = ' ';
                }
                op[i] = '\0';

                for (i = 0; i < 9; i++)
                {
                    curso[i] = ' ';
                }
                curso[i] = '\0';

                for (i = 0; i < 2; i++)
                {
                    turma[i] = ' ';
                }
                turma[i] = '\0';

                printf ("\n\n\n");

                printf ("Entre com os dados do novo registro:\n");
                printf ("Matricula (ate 6 caracteres): ");
                scanf ("%[^\n]", matricula);
                getchar ();
                tam = strlen (matricula);
                if (tam != 6)
                    matricula[tam] = ' ';

                printf ("Nome (ate 40 caracteres): ");
                scanf ("%[^\n]", nome);
                getchar ();
                tam = strlen (nome);
                if (tam != 40)
                    nome[tam] = ' ';

                printf ("Opcao (ate 5 caracteres): ");
                scanf ("%[^\n]", op);
                getchar ();
                tam = strlen (op);
                if (tam != 5)
                    op[tam] = ' ';

                printf ("Curso (ate 9 caracteres): ");
                scanf ("%[^\n]", curso);
                getchar ();
                tam = strlen (curso);
                if (tam != 9)
                    curso[tam] = ' ';

                printf ("Turma (2 caracteres): ");
                scanf ("%[^\n]", turma);
                getchar ();
                tam = strlen (turma);
                if (tam != 2)
                    turma[tam] = ' ';


                arquivo = fopen ("lista.txt", "a");

                fprintf (arquivo, "\n%s ", matricula);
                fprintf (arquivo, "%s ", nome);
                fprintf (arquivo, "%s ", op);
                fprintf (arquivo, "%s ", curso);
                fprintf (arquivo, "%s", turma);

                fclose (arquivo);



                strcpy (novo_reg->Chave, matricula);
                strcat (novo_reg->Chave, " ");
                tam=strlen(novo_reg->Chave);

                for (i = 0; i < 30-tam; i++)
                {
                    novo_reg->Chave[tam -1 +i] = nome[i];
                }

                Insere (*novo_reg, arv);

                printf ("\n\n\n");
                free (novo_reg);

                saida = fopen ("indicelista.bt", "w");
                gravar_arquivo (*arv, saida);
                fclose (saida);

                break;
            case 2:
                Imprime (*arv);
                printf ("\n\n\n");
                break;
            case 3:
                break;
            default:
                printf ("\n\nOpcao invalida!\n\n");
                break;
        }
    }
    while (opcao != 3);



    return 0;
}

int verificaMaior(Registro reg1,Registro reg2){
    int igual = 0, i , j, Retorno;

    for(i=0;i<=30;i++){
        if(reg1.Chave[i]<reg2.Chave[i]){
          Retorno = 0;
          return Retorno;
        }
        else if(reg1.Chave[i]>reg2.Chave[i]){
          Retorno = 1;
          return Retorno;
        }
    }
    //Caso chaves iguais
    return 2;
}


void InsereNapagina(Ponteiro ptr, Registro Reg, Ponteiro ptr_Dir)
{
  int k;
  int N_achou;

  k = ptr->n;

  N_achou = k > 0; //caso ptr->n == 0 nao entra no loop

  //Encontra a posição da chave na pagina e desloca todas as chaves posteriores para a posição k+1
  while (N_achou)
  {
    if ( verificaMaior(Reg,ptr->Regs[k-1]) == 1 || verificaMaior(Reg,ptr->Regs[k-1]) == 2)
    {
      N_achou = 0;
      break;
    }
    ptr->Regs[k] = ptr->Regs[k - 1];
    ptr->ponteiros[k + 1] = ptr->ponteiros[k];
    k--;
    if (k < 1)
      N_achou = 0;
  }
  //insere registro na pagina
  ptr->Regs[k] = Reg;
  ptr->ponteiros[k + 1] = ptr_Dir;
  ptr->n++;
}

void Ins(Registro Reg, Ponteiro ptr, int *Cresceu, Registro *RegRetorno, Ponteiro *ptrRetorno)
{
  Ponteiro ptrTemp;
  int i, j;

  if (ptr == NULL)
  {
    *Cresceu = 1;
    *RegRetorno = Reg;
    *ptrRetorno = NULL;
    return;
  }
  i = 1;
  while (i < ptr->n && verificaMaior(Reg,ptr->Regs[i-1]) == 1)
    i++;
  if (verificaMaior(Reg,ptr->Regs[i-1]) == 2)
  {
    *Cresceu = 0;
    return;
  }
  if (verificaMaior(Reg,ptr->Regs[i-1]) == 0)
    Ins(Reg, ptr->ponteiros[i - 1], Cresceu, RegRetorno, ptrRetorno);
  else
    Ins(Reg, ptr->ponteiros[i], Cresceu, RegRetorno, ptrRetorno);
  if (!*Cresceu)
    return;
  if (ptr->n < MM)
  {  /* Pagina tem espaco */
    InsereNapagina(ptr, *RegRetorno, *ptrRetorno);
    *Cresceu = 0;
    return;
  }
  /* Overflow: Pagina tem que ser dividida */
  ptrTemp = (Ponteiro) malloc(sizeof(Pagina));
  ptrTemp->n = 0;
  ptrTemp->ponteiros[0] = NULL;
  if (i <= M + 1)
  {
    InsereNapagina(ptrTemp, ptr->Regs[MM-1], ptr->ponteiros[MM]);
    ptr->n--;
    InsereNapagina(ptr, *RegRetorno, *ptrRetorno);
  }
  else
    InsereNapagina(ptrTemp, *RegRetorno, *ptrRetorno);
  for (j = M + 2; j <= MM; j++)
    InsereNapagina(ptrTemp, ptr->Regs[j - 1], ptr->ponteiros[j]);
  ptr->n = M;
  ptrTemp->ponteiros[0] = ptr->ponteiros[M + 1];
  *RegRetorno = ptr->Regs[M];
  *ptrRetorno = ptrTemp;
}
void Insere(Registro Reg, Ponteiro *ptr)
{
  int Cresceu;
  Registro RegRetorno;
  Ponteiro ptrRetorno;
  Ponteiro ptrTemp;
  Ins(Reg, *ptr, &Cresceu, &RegRetorno, &ptrRetorno);
  if (Cresceu) { /* Arvore cresce na altura pela raiz */
    ptrTemp = (Ponteiro) malloc(sizeof(Pagina));
    ptrTemp->n = 1;
    ptrTemp->Regs[0] = RegRetorno;
    ptrTemp->ponteiros[1] = ptrRetorno;
    ptrTemp->ponteiros[0] = *ptr;
    *ptr = ptrTemp;
  }
}

void indexARV(Ponteiro ptr, int *index){
    int i, j;

    if(ptr!=NULL){
       ptr->index = *index;
       *index = *index + 1;

       for(j=0; j<ptr->n+1; j++) {
          indexARV(ptr->ponteiros[j],index);
       }
    }

}

void Imprime(Ponteiro ptr){
    int i, j;
    if(ptr!=NULL){

        printf("\n\nPagina %d:\n",ptr->index);
        printf("\nChaves: \n");
        for(i = 0;i<ptr->n;i++) {
            puts(ptr->Regs[i].Chave);
        }
        printf("Ponteiros: ");
        for(j=0; j<ptr->n+1; j++) {
        if(ptr->ponteiros[j]!=NULL)
        printf("  |Pagina %d| ",ptr->ponteiros[j]->index);
        }
        for(j=0; j<ptr->n+1; j++) {
            Imprime(ptr->ponteiros[j]);
        }
    }
}
long int obter_tam_registro(FILE *fonte){
    char c;
    long int tamanho;

    do{
        fscanf(fonte, "%c", &c);
    }while(c != '\n');

    tamanho = ftell(fonte);

    return tamanho;
}
void gerar_arvore_b (Ponteiro *ptr)
{
    int i,j = 0;
    Registro reg;
    FILE *fonte;

    fonte = fopen ("lista.txt", "r");

    if (fonte == NULL)
    {
        printf ("Arquivo fonte nao encontrado!\n\n");
        exit (1);
    }
    long int tamanho = obter_tam_registro(fonte);
    //Leitura das chaves dos registros
    while (!feof (fonte))
    {
        //limpando a variável para armazenar uma nova chave
        for (i = 0; i < 30; i++)
        {
            reg.Chave[i] = ' ';
        }
        reg.Chave[i] = '\0';

        fseek(fonte,tamanho*j, SEEK_SET);
        j++;
        fread (reg.Chave, sizeof (char), 30, fonte);

        //Agora, vamos inserir a chave na árvore-B
        Insere(reg, ptr);
        }
}

void gravar_arquivo (Ponteiro ptr, FILE *saida)
{
    int i, j;

    if(ptr!=NULL){

        fprintf(saida, "\n\nPagina %d:\n", ptr->index);
        fprintf(saida, "\nChaves: \n");
        for(i = 0;i<ptr->n;i++) {
            fprintf(saida, "%s\n",ptr->Regs[i].Chave);
        }
        fprintf(saida,"Ponteiros: ");
        for(j=0; j<ptr->n+1; j++) {
        if(ptr->ponteiros[j]!=NULL)
        fprintf(saida,"  |Pagina %d| ",ptr->ponteiros[j]->index);
        }
        for(j=0; j<ptr->n+1; j++) {
            gravar_arquivo(ptr->ponteiros[j], saida);
        }
    }
}
