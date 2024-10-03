/*******************************************
*                                          *
* Ray Santiago Del Negro                   *
* Trabalho Prático                         *
* Disciplina: Algortimos e Programação II  *
* Professores: Ivone, Mauro e Ronaldo      *
*                                          *
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pro{
    int codigo; /*maior que zero e unico*/
    char desc[51]; /*nome do produto sem acento e espaço, sem letra minuscula*/
    int quantidade; /*quantidade do produto*/
    float preco; /*preco atual*/
    struct pro *prox;
};
typedef struct pro produto;

/*Uma busca é feita de forma recorrente, pensei em fazer uma função para facilitar*/
produto *verifica_codigo(produto *lst, int cod)
{
    produto *p;
    p = lst;
    while(p!=NULL && p->codigo != cod) /*while que percorre a lista inteira, e para quando achar o codigo 
                                        ou quando acabar a lista*/
    {
        p = p->prox;
    }
    return p; /*retorna vazio quando não possui o codigo, ou a posição do codigo*/
}

/*fiz uma função para me ajudar a inserir de forma não-descrescente a quantidade de
produto na saida*/
void insere_naodecrescente_quanti(produto **lst ,int cod, char nome[], int qtd)
{
    produto *p, *q, *nova;
    q=*lst;
    p=NULL;
    while(q!=NULL && q->quantidade < qtd)
    {
        p=q;
        q=q->prox;
    }
    nova = (produto *) malloc(sizeof(produto));
    /*não precisa da auxiliar de erro, já que essas funções já estão na finalização do 
    programa*/
    if(nova != NULL)
    {
        nova->codigo = cod;
        strcpy(nova->desc, nome);
        nova->quantidade = qtd;

        if(p==NULL)
            *lst = nova;
        else
            p->prox = nova;
        nova->prox = q;
    }
}

/*Criei outra função para me ajudar a inserir de forma ordenada alfabetica dos
produtos no comando relatorio */
void insere_palavra_ordenada(produto **lst, int *error, int cod, char nome[], int quanti, float prec)
{
    produto *p, *q, *nova;
    q=*lst;
    p=NULL;
    while(q!= NULL && strcmp(nome, q->desc) >= 0) /*verificando qual produto tem */
    {                                             /* prioridade alfabetica por strcmp*/
        p=q;
        q=q->prox;
    }
    nova = (produto *) malloc(sizeof(produto));
    if(nova != NULL)
    {
        nova->codigo = cod;
        strcpy(nova->desc, nome);
        nova->quantidade = quanti;
        nova->preco = prec;

        if(p==NULL)
            *lst = nova;
        else
            p->prox = nova;
        nova->prox = q;
    }
    else
    {
        printf("Erro ao alocar memoria, programa será encerrado!\n");
        *error = 1;  /*variavel que vai ajudar a finalizar o programa caso tenha
                           um erro de alocação*/
    }
}

/*1*/
void insere_produto(int modo, produto **lst, int *error, int cod, char nome[],
int qtd, float prec)
{
    /*A variavel int modo foi feita para diferenciar a mensagem de inserção de produto com a de inicio do
    programa.*/
    /*modo == 1, é a inserção do inicio do programa por Produtos.dat ou pelo comando importar
      modo == 2 é a inserção manual por meio do comando inserir*/
    produto *nova, *repetido, *p, *q;
    
    repetido = verifica_codigo(*lst, cod); /*verificando se ja existe o código cadastrado*/
    if(repetido == NULL) /*se for nulo é porque o codigo não existe na lista*/
    {
        nova = (produto *) malloc(sizeof(produto));
        if(nova != NULL)
        {
            /*atribuindo os valores na lista*/
            nova->codigo = cod;
            strcpy(nova->desc, nome); /*copiar uma string para outra*/
            nova->quantidade = qtd;
            nova->preco = prec;
            q = *lst;
            p = NULL; 
			while(q!=NULL && q->codigo < cod)
			{
				p=q;
				q=q->prox;
			}
			
            if(p!=NULL)
               p->prox = nova;       
            else
               *lst = nova;
            nova->prox = q;
            if(modo == 2)      
                printf("Produto %d inserido com sucesso!\n", cod);
        }
        else
        {
            printf("Erro ao alocar memoria, programa será encerrado!\n");
            *error = 1; /*variavel que vai ajudar a finalizar o programa caso tenha
                           um erro de alocação*/
        }
    }
    else 
        if(modo == 2)      
            printf("Erro ao inserir o produto %d.\n", cod);
}

/*Função de inicio do Programa (está abaixo do #1 porque usa a funcao #1)*/
void inicio_produtos(produto **lst, int *error)
{
    produto novo;
    int i, num;
    FILE *arqEN;

    arqEN = fopen("Produtos.dat", "ab+"); /*escrita e leitura (binaria) (ab+) */
	if(arqEN != NULL) /*Caso não exista o Produto. dat, é necessario cria-lo*/
    {
      fread(&num, sizeof(int), 1, arqEN);
        i=0;	
	    while(i<num)
	    {
            fread(&novo, sizeof(produto), 1, arqEN);
            /*insere modo 1*/
            insere_produto(1, lst, &(*error), novo.codigo, novo.desc, 
            novo.quantidade, novo.preco);
            /*variavel auxiliar erro caso tenha um erro de alocacao*/
            i++;
        }
        printf("%d produtos importados!\n", num);
        fclose(arqEN);
    }
    else
        printf("Erro ao alocar/criar o arquivo Produtos.dat\n");
}

/*2*/
void exclua_produto(produto **lst, int cod)
{
    produto *p, *q;
    q = *lst;
    p = NULL;
    while(q!=NULL && q->codigo != cod)
    {
        p=q;
        q=q->prox;
    }
    if(q!=NULL)
    {
        if(p==NULL)
            *lst = q->prox;
        else
            p->prox = q->prox;
        printf("Produto %d excluído com sucesso!\n", cod);
        free(q);
    }
    else
        printf("Produto %d não cadastrado!\n", cod);
}

/*3*/
void atualiza_produto(produto *lst, char p[], int cod, float valor)
{
    produto *altera;
    altera = verifica_codigo(lst, cod); /*procurando o produto pelo codigo*/
    if(altera != NULL)
    {
        if(strcmp("-q", p) == 0)
            altera->quantidade = valor;
        if(strcmp("-v", p) == 0)
            altera->preco = valor;     
        printf("Produto %d atualizado!\n", cod);      
    }
    else
        printf("Produto %d não cadastrado.\n", cod);
}

/*4*/
void consulta_produtos(produto *lst, char consulta[]) /*ver isso com a ivone*/
{
    int veri;
    char *comparacao;
    produto *q;
    q=lst;   
    veri = 0; 
    while(q!=NULL)
    {
        comparacao = strstr(q->desc, consulta); /*consultar se há uma substring na desc 
                                                 produto*/
        if(veri == 0 && comparacao != NULL)
            veri = 1; /*verificador para não printar a mensagem de produto não encontrado*/

        if(comparacao != NULL)
            printf("%d - %s - %d - %.2f \n", q->codigo, q->desc, q->quantidade, q->preco);
            
        q=q->prox;
    }
    if(veri == 0)
        printf("Nenhum produto encontrado!\n");
}

/*5*/
void relatorio_produtos(produto *lst, int *error) 
{
    FILE *arqOUT;
    char letra;
    produto *q, *lstalf;
	
	lstalf = NULL;
	q = lst;
    while(q!=NULL) /*ordernar a lista de produtos de codigo para ordem alfabetica*/
    { 
        insere_palavra_ordenada(&lstalf, &(*error), q->codigo, q->desc, q->quantidade, q->preco);
        q=q->prox;
    }
	
    arqOUT = fopen("Relatorio.txt", "w");
    if(arqOUT != NULL)
    {
        q=lstalf;
        letra = q->desc[0]; /*Letra inicial do primeiro produto da Lista de produtos*/
        
        printf("%c\n", letra);
        fprintf(arqOUT, "%c\n", letra);

        while(q!=NULL)
        {
            if(letra != q->desc[0]) /*se a letra for diferente*/
            {
                letra = q->desc[0]; /*letra recebe a proxima inicial*/
                printf("%c\n", letra);
                fprintf(arqOUT, "%c\n", letra);
            }
            printf("    %d - %s - %d - %.2f \n", q->codigo, q->desc, q->quantidade, q->preco);
            fprintf(arqOUT, "    %d - %s - %d - %.2f \n", q->codigo, q->desc, 
            q->quantidade, q->preco); /*prints do arquivo e na compilação*/
            q=q->prox; /*proximo produto*/
        }
    }
    else
        printf("Erro ao abrir o documento Relatorio.txt \n");

    /*apagar a alocação da lista alfabetica criada*/
    q = lstalf;
    while(q!=NULL)
    {   
        free(q);
        q = q->prox;
    }     
    fclose(arqOUT);
}

/*6*/
void importar_produtos(produto **lst, int *error, char nomearq[])
{
    produto *verifica; 
    /*auxiliares para inserção*/
    int i, num, n_cod, n_qtd, cont; 
    char n_nome[51];
    float n_prec;
    FILE *arqEN;
    cont = 0;
    
    arqEN = fopen(nomearq, "r"); /*apenas leitura*/
	if(arqEN == NULL)
        printf("Erro ao importar do documento %s. \n", nomearq);
    else
    {
        fscanf(arqEN, "%d", &num);
        i=0;	
	    while(i<num)
	    {
            fscanf(arqEN, "%d;%[^;];%d;%f", &n_cod, n_nome, &n_qtd,  &n_prec);

            /*É necessário fazer isso por causa da mensagem de produtos importados
            mesmo repetindo duas comparações de códigos (nessa função e na insere)
            é o único jeito de verificar a quantidade de produtos importados*/ 
            verifica = verifica_codigo(*lst, n_cod); /*verificar se o codigo está cadastrado*/

            if(verifica == NULL) /*codigo não esta na lista*/
            {
                /*insere modo 1*/
                insere_produto(1, &(*lst), &(*error), n_cod, n_nome, n_qtd,  n_prec); 
                /*auxiliar erro caso tenha erro na alocacao dentro da insere*/
                cont = cont + 1;
            }
		    i++;
	    }
        printf("%d produtos importados!\n", cont);  
        fclose(arqEN);
    }
}

/*7*/
float venda_produto(produto *lst, int cod)
{
    float valor;/*preco do produto*/
    produto *vende; /*ponteiro para o produto*/
    valor = 0; /*valor inicial para caso nao existir o produto*/

    vende = verifica_codigo(lst, cod); /*procurar o codigo*/
    if(vende != NULL)
    {
        if(vende->quantidade == 0) /*se a quantidade foi igual a 0, não é possivel vender*/
            printf("%d - produto em falta no estoque\n", cod);
        else
        {
           vende->quantidade = vende->quantidade - 1; /*decrementa*/
           valor = vende->preco; /*adiciona o valor do produto*/
           printf("%d - %s - %.2f \n", vende->codigo, vende->desc, vende->preco);
        }
    }
    else
         printf("%d - produto não cadastrado.\n", cod);
    return valor;
}

/*9*/
void finaliza_programa(produto **lst)
{
    int soma; /*quantidade de produtos no Produto.dat*/
    FILE *arqCOUT, *arqPOUT; /*Arquivo de Compras.txt e Produto.dat (w+b)*/
    produto *compra, *q;    /*ponteiros auxiliares*/
    compra = NULL;
    soma = 0;

    /*While para adicionar todos os produtos com a quantidade menor que 15*/
    q = *lst;
    while(q!=NULL)
    {
        soma = soma + 1; /*somar quantidade de produtos para usar no Produtos.dat*/
        if(q->quantidade < 15)
             insere_naodecrescente_quanti(&compra, q->codigo, q->desc, q->quantidade);
        q=q->prox;
    }

    /*Inserir no arquivo Compras.txt*/
    arqCOUT = fopen("Comprar.txt", "w");
    if(arqCOUT == NULL)
        printf("Erro ao inserir arquivo Compras.txt \n");
    else
    {    
        for(q=compra;q!=NULL;q=q->prox)
        {
            printf("%d - %s - %d  \n", q->codigo, q->desc, q->quantidade);
            fprintf(arqCOUT, "%d - %s - %d  \n", q->codigo, q->desc, q->quantidade);
        }
        fclose(arqCOUT);
    }

    /*Inserir no arquivo Produto.dat*/
    arqPOUT = fopen("Produtos.dat", "wb");
    if(arqPOUT == NULL)
        printf("Erro ao inserir arquivo Produto.dat \n");
    else
    { 
        fwrite(&soma, sizeof(int),1 , arqPOUT);
        for(q=*lst;q!=NULL;q=q->prox)
        {
            fwrite(q, sizeof(produto),1 , arqPOUT);
        }
        fclose(arqPOUT);
    }

    /*while para apagar a lista de comprar*/
    q = compra;
    while(q!=NULL)
    {   
        free(q);
        q = q->prox;
    }     


    /*while para limpar a lista principal*/
    q = *lst;
    while(q!=NULL)
    {
        free(q);
        q = q->prox;
    }
    
}

int main(void)
{
    /*Para economizar memoria na main, usei auxiliares para ajudar em comandos diferentes
    Para assim mesmo se usar determinada função a maioria das variaveis criadas serem 
    utilizadas.*/
    int aux_codigo, aux_quanti, erro;
    char aux_palavra[51], comando[10];
    float aux_numreais;
    produto *lista;
    erro = 0;
    lista = NULL;

    inicio_produtos(&lista, &erro); /*inicia o procedimento de inicio do programa*/
    do
    {
        scanf("%s", comando); /*leitura do comando*/
        
        if(strcmp("inserir", comando) == 0) /*1*/
        {
            scanf("%d %s %d %f", &aux_codigo, aux_palavra, &aux_quanti, &aux_numreais); /*codigo novo, descrição nova,
																			quantidade do produto, e preco do novo produto*/
            insere_produto(2, &lista, &erro, aux_codigo, 
            aux_palavra, aux_quanti, aux_numreais); /*função de inserir no modo 2, para mostrar a inserção do produto*/
        }

        if(strcmp("excluir", comando) == 0) /*2*/
        {
            scanf("%d", &aux_codigo); /*código do produto que vai ser excluido*/
            exclua_produto(&lista, aux_codigo);
        }

        if(strcmp("atualizar", comando) == 0) /*3*/
        {
            scanf("%s %d %f", aux_palavra, &aux_codigo, &aux_numreais); /*O -q ou -v, o codigo do 
                                                                        produto que você vai alterar, e o 
																			valor que você vai alterar*/
            atualiza_produto(lista, aux_palavra, aux_codigo, aux_numreais);
        } 
    
        if(strcmp("consultar", comando) == 0) /*4*/
        {
            scanf("%s", aux_palavra); /*A sub cadeia que vai ser procurar na lista ordenada pelo codigo*/
            consulta_produtos(lista, aux_palavra);
        }   
        
       if(strcmp("relatorio", comando) == 0)   /*5*/
            relatorio_produtos(lista, &erro); /*relatorio da lista em ordem alfabetica*/
   

       if(strcmp("importar", comando) == 0)/*6*/
       {
            scanf("%s", aux_palavra); /*localização/nome do arquivo a ser importado*/
            importar_produtos(&lista, &erro, aux_palavra);
       }

       if(strcmp("vender", comando) == 0) /*7*/
       {
            aux_numreais = 0; /*valor inicial da venda*/
            scanf("%d", &aux_codigo); /*primeiro codigo do produto comprado*/
            while(aux_codigo != 0)
            {
                aux_numreais =  aux_numreais + venda_produto(lista, aux_codigo); /*a função vendar retorna o valor do produto, e isso 
																				 será somado aos demais produtos escolhidos*/
                scanf("%d", &aux_codigo); /*proximo produto pelo codigo*/
            }
            printf("---------\n");
            printf("Total %.2f\n", aux_numreais); /*valor final da venda*/
       } 

       if(strcmp("help", comando) == 0)/*8*/
       {
            printf("    Antes de explicar minha escolha de tipo de lista, é importante ressaltar o motivo\n");
            printf("de eu não escolher as seguintes opções:\n");
            printf("\n");
            printf("    Pilha: o sistema de LIFO (Last in First out) é interessante em problemas que envolvem a\n");
            printf("abertura e fechaduras de chaves, ou pilhas de produtos. Talvez se tivesse algum sistema de\n");
            printf("prateleira seria interessante usar pilha.\n");
            printf("\n");
            printf("    Fila: o sistema de FIFO (First in First out) é bom em sistema com filas em particular, como a\n");
            printf("lista que vou criar vai estar em ordem crescente pelo código, não será necessário usar uma\n");
            printf("fila. Se tivesse um sistema de caixa, poderia usar a fila para organizar os clientes.\n");
            printf("\n");
            printf("    Lista dupla encadeada: Pensei em usar esse tipo de lista mas todas as funcionalidades\n");
            printf("apresentadas e as que criei podem ser feitas por uma lista simples, então usar essa lista só\n");
            printf("ocuparia memória.\n");
            printf("\n");            
            printf("    Lista circular: Pensei em usar essa para ajudar na operação de registro de venda, para\n");
            printf("facilitar a busca de produtos, porém acho que não é muito efetivo porque não me sinto\n");
            printf("confiante em usar um ponteiro que fica mudando de posição em diferentes funções.\n");
            printf("\n");
            printf("    Lista com cabeça:Por afinidade escolhi usar sem cabeça para não confundir e criar novas funções.\n");
            printf("\n");
            printf("    A escolhida foi a Lista Encadeada Ordenada Simples sem Cabeça:\n");
            printf("\n");
            printf("    A escolha desta lista foi feita a partir de uma análise de todas as funções\n");
            printf("a fim de deixar o programa com a complexidade mais baixa possível.\n");
            printf("\n");
            printf("    A ordenação escolhida é crescente a partir do código do produto,\n");
            printf("é escolhido essa forma de ordenação a partir das funções que são mais utilizadas.\n");
            printf("\n");
            printf("    Poderiam existir uma ordenação por ordem alfabetica, igual feito no comando relatório\n");
            printf("mas seria usual. Já que as funções sair do programa e consultar tiram\n");
            printf("melhor proveito da ordenação crescente pelo código. \n"); 
            printf("\n");
            printf("    Portanto, o uso da Lista Encadeada Ordenada Simples sem Cabeça é a \n");
            printf("pensada para resolver o problema de seu Canindé, algumas especificações mudariam \n"); 
            printf("totalmente a forma que seria implementada a lista, porém este não é o caso, já que \n");
            printf("uma ordenada simples já consegue suprir todas as exigências do mercado Capi Lagoa. \n");
       }     

       if(strcmp("sair", comando) == 0 || erro == 1) /*9*/ /*Se ocorrer algum erro de alocação esse comando também 
															é iniciado*/
       {
            finaliza_programa(&lista); /*finaliza o programa e apaga a lista*/
       }

    }while(strcmp("sair", comando) != 0 && erro == 0);

    return 0;
}

