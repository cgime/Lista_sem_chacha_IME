#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#define TRUE 1
#define FALSE 0
FILE *arquivo_lista_completa_data;
FILE *arquivo_lista_oficiais;
FILE *arquivo_lista_graduados;
FILE *arquivo_lista_serv_civ;
FILE *arquivo_lista_visitante;

typedef struct  
{
    char posto_grad[21], nome[101], cpf[15]; 
} Pessoa ;

typedef struct 
{
    int dia, mes, ano;
    char dia_aux[3], mes_aux[3], ano_aux[5];
} Data; 

typedef struct 
{
    int horas, minutos; 
    char hora_aux[3], min_aux[3];
} Hora; 

typedef struct 
{
    Pessoa pessoa; 
    Data data; 
    Hora hora_entrada;
    Hora hora_saida; 
    char destino[21];    
} ControleEntrada; 

typedef struct 
{
    ControleEntrada *controle_entrada; 
    int tamanho;
    int tam_max; 
} VetorControleEntrada; 

int compara_datas(Data d1, Data d2) 
{
    if(d1.ano > d2.ano)
        return 1;
    if(d1.ano < d2.ano)
        return -1;
	else 
	{
    	if(d1.mes > d2.mes)
        	return 1;
    	if(d1.mes < d2.mes)
        	return -1;
        else
        {
        	if(d1.dia > d2.dia)
        		return 1;
   			if(d1.dia < d2.dia)
        		return -1;
        	else
        		return 0;
        }
    }
}

int comparar_horario(Hora h1, Hora h2) 
{
    if(h1.horas > h2.horas)
        return 1;
    if(h1.horas < h2.horas)
        return -1;
    if(h1.minutos > h2.minutos)
        return 1;
    if(h1.minutos < h2.minutos)
        return -1;
    return 0; 
}

void iniciar_vetor(VetorControleEntrada *vetor_controle_entrada)
{
    vetor_controle_entrada->tamanho = 0;  // Equivalente a  (*vetor_controle_entrada).tamanho = 0;
    vetor_controle_entrada->tam_max = 2;
    vetor_controle_entrada->controle_entrada = (ControleEntrada *)  malloc(vetor_controle_entrada->tam_max * sizeof(ControleEntrada));
}

void adicionar_elementos(VetorControleEntrada *vetor_controle_entrada, ControleEntrada elemento) 
{
    if(vetor_controle_entrada->tamanho >= vetor_controle_entrada->tam_max) 
	{
        vetor_controle_entrada->tam_max *= 2; 
        vetor_controle_entrada->controle_entrada = (ControleEntrada *) realloc(vetor_controle_entrada->controle_entrada, vetor_controle_entrada->tam_max * sizeof(ControleEntrada));
    }
    vetor_controle_entrada->controle_entrada[vetor_controle_entrada->tamanho] = elemento;
    vetor_controle_entrada->tamanho++;
}


void organizar_vetor(VetorControleEntrada *vetor_controle_entrada) 
{
    printf("tamamho_func: %i\n", vetor_controle_entrada->tamanho); 
    for(int i = 0; i < vetor_controle_entrada->tamanho; i++) 
	{
        for(int j = i + 1; j < vetor_controle_entrada->tamanho; j++) 
		{
            if(compara_datas(vetor_controle_entrada->controle_entrada[i].data, vetor_controle_entrada->controle_entrada[j].data) == -1 ||
            (compara_datas(vetor_controle_entrada->controle_entrada[i].data, vetor_controle_entrada->controle_entrada[j].data) == 0 && 
            comparar_horario(vetor_controle_entrada->controle_entrada[i].hora_entrada, vetor_controle_entrada->controle_entrada[j].hora_entrada) == -1)) 
			{
                ControleEntrada aux = vetor_controle_entrada->controle_entrada[i];
                vetor_controle_entrada->controle_entrada[i] = vetor_controle_entrada->controle_entrada[j];
                vetor_controle_entrada->controle_entrada[j] = aux; 
            }            
        }
    }
}

int validar_cpf(char cpf[])
{
    for(int i = 0; i < 11; i++)
    {
        if (!(cpf[i] >= '0' && cpf[i] <= '9'))
        {
            return FALSE;
        }
    }
    int primeiro_digito = (int) (cpf[9] - '0');
    int segundo_digito = (int) (cpf[10] - '0');
    int aux = 0;
    for (int i = 8, j = 2; i>=0 ; i--, j++)
    {
        aux += j *(int) (cpf[i] - '0');
    }
    int resto = aux % 11;
    if (resto < 2)
    {
        if (primeiro_digito != 0)
            return FALSE;
    }
    else 
    { 
        if (primeiro_digito != 11 - resto)
            return FALSE;
    }
    aux = 0;
    for (int i = 9, j = 2; i >= 0; i--, j++)
    {
        aux += j*(int) (cpf[i] - '0');
    }
    resto = aux % 11;
    if (resto <2)
    {
        if(segundo_digito != 0)
            return FALSE;
    } 
    else
    {
        if(segundo_digito != 11 - resto)
            return FALSE;
    }
    return TRUE;    
}
void formatar_cpf(char cpf[])
{
    char cpf_aux[15];
    for(int i = 0; i < 3; i++)
        cpf_aux[i] = cpf[i];
    cpf_aux[3] = '.';
    for (int i = 4; i < 7; i++)
        cpf_aux[i] = cpf[i-1];
    cpf_aux[7] = '.';
    for(int i = 8; i < 11; i++)
        cpf_aux[i] = cpf[i-2];
    cpf_aux[11] = '-';
    for(int i = 12; i < 14 ; i++)
        cpf_aux[i] = cpf[i-3];
    for(int i = 0; i < 14; i++)
        cpf[i] = cpf_aux[i];
}

void adicionar_entrada(VetorControleEntrada *vetor)
{
    int verificar_dados = FALSE;
    ControleEntrada controle_entrada;
	do
	{	 
    	printf("tamanho_main = %i\n", vetor->tamanho);
    	printf("Adicione o nome: ");
    	scanf(" %[^\n]", controle_entrada.pessoa.nome);

    	printf("Adicione o posto: ");
    	scanf(" %[^\n]", controle_entrada.pessoa.posto_grad); 

    	do
    	{
			printf("Adicione o CPF(so numeros): "); 
    		scanf(" %[^\n]", controle_entrada.pessoa.cpf);
			if(validar_cpf(controle_entrada.pessoa.cpf) == FALSE)
			{
				printf("CPF invalido. repita o numero:\n");
			} 
    	}while(validar_cpf(controle_entrada.pessoa.cpf) == FALSE);
    	formatar_cpf(controle_entrada.pessoa.cpf);

    	printf("Coloque a data (dd mm aaaa): "); 
    	scanf("%s %s %s", controle_entrada.data.dia_aux, controle_entrada.data.mes_aux, controle_entrada.data.ano_aux); 
    	fflush(stdin);
    	controle_entrada.data.dia = atoi(controle_entrada.data.dia_aux);
    	controle_entrada.data.mes = atoi(controle_entrada.data.mes_aux);
    	controle_entrada.data.ano = atoi(controle_entrada.data.ano_aux);

    	printf("Coloque a hora de entrada (HH MM): ");
    	scanf("%s %s", controle_entrada.hora_entrada.hora_aux, controle_entrada.hora_entrada.min_aux);
    	fflush(stdin);
    	controle_entrada.hora_entrada.horas = atoi(controle_entrada.hora_entrada.hora_aux);
    	controle_entrada.hora_entrada.minutos = atoi(controle_entrada.hora_entrada.min_aux);

    	printf("Coloque a hora de saida (HH MM): ");
    	scanf("%s %s", controle_entrada.hora_saida.hora_aux, &controle_entrada.hora_saida.min_aux);
    	fflush(stdin);
    	controle_entrada.hora_saida.horas = atoi(controle_entrada.hora_saida.hora_aux);
    	controle_entrada.hora_saida.minutos = atoi(controle_entrada.hora_saida.min_aux);

    	printf("Coloque o destino: ");
    	scanf(" %[^\n]", controle_entrada.destino);
		fflush(stdin); 
    
    	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
    	printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", controle_entrada.pessoa.posto_grad, controle_entrada.pessoa.nome, controle_entrada.data.dia,
    	controle_entrada.data.mes, controle_entrada.data.ano, controle_entrada.hora_entrada.horas, controle_entrada.hora_entrada.minutos, controle_entrada.hora_saida.horas, 
		controle_entrada.hora_saida.minutos, controle_entrada.destino, controle_entrada.pessoa.cpf);
		printf("\n");
    
    	printf("\nOs dados estao corretos? Digite 1 para sim e 0 para nao:\n");
    	scanf(" %d", &verificar_dados);
    	if(verificar_dados == FALSE)
    	{
    		printf("\nRefaca os dados:\n\n");
    		fflush(stdin); 
		}
		else
		{
			printf("\nOk, continuando programa.\n\n");
		}
	}while(verificar_dados == FALSE);
	
    adicionar_elementos(vetor, controle_entrada);

    if(vetor->tamanho > 1)
        organizar_vetor(vetor);
   
}
void ver_dados(VetorControleEntrada vetor) 
{
	printf("****************************************************   LISTA DE ENTRADA    *******************************************************\n");
	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
    for(int i = 0; i < vetor.tamanho; i++) 
	{
		printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
        vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
        vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    }
}

void ver_dados_oficiais(VetorControleEntrada vetor) 
{
    printf("****************************************************   LISTA DE ENTRADA    *******************************************************\n");
	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
	for(int i = 0; i < vetor.tamanho; i++) 
	{
		if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Cel") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"cel") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"TC")==0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"tc") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Maj") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"maj") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Cap") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"cap") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"1 Ten") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"1 ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"2 Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"2 ten") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Asp Of") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"asp of") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Asp") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"asp")==0)
		{				
			printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
       		vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
       		vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
       		vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       		vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       		vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
		}
			
    }
}

void ver_dados_graduados(VetorControleEntrada vetor) 
{
    printf("****************************************************   LISTA DE ENTRADA    *******************************************************\n");
	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
	for(int i = 0; i < vetor.tamanho; i++) 
	{
        if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"ST") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1 Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2 Sgt") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3 Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Cb") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd EP") == 0 || 
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd EV") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sub Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1 sgt") == 0 || 
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2 sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3 sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "cb") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd ep") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd ev") == 0 || 
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sub") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3º sgt") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2º sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1º sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3º Sgt") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2º Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1º Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Aluno") == 0 || 
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "aluno") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Al") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "al") == 0)
		{
			printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        	vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        	vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        	vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
        	vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
        	vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
		}
    }
}

void ver_dados_servidores(VetorControleEntrada vetor) 
{
    printf("****************************************************   LISTA DE ENTRADA    *******************************************************\n");
	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
	for(int i = 0; i < vetor.tamanho; i++) 
	{
        if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "SC") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sc") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Serv Civ") == 0 ||
		strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "servidor civil") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Servidor Civil") == 0)
		{
			printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        	vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        	vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        	vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
        	vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
        	vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
		}
    }
}
void ver_dados_visitantes(VetorControleEntrada vetor) 
{
    printf("****************************************************   LISTA DE ENTRADA    *******************************************************\n");
	printf("Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
	for(int i = 0; i < vetor.tamanho; i++) 
	{
        if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Visitante") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "visitante") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Vist") == 0
		|| strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "vist") == 0)
		{
			printf("%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        	vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        	vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        	vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
        	vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
        	vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
		}
    }
}

int main()
{
    VetorControleEntrada vetor; 
    iniciar_vetor(&vetor);
    
    int opcao=0; 
    while(TRUE) 
	{
		do
		{   
			printf("\n");
			printf("********************MENU***********************\n"); 
			printf("1- Adicionar uma pessoa:\n"); 
			printf("2- Ver relacao de cadastro completa:\n");
			printf("3- Ver relacao de cadastro de oficiais:\n");
			printf("4- Ver relacao de cadastro de graduados:\n");
			printf("5- Ver relacao de cadastro de servidores civis:\n");
			printf("6- Ver relacao de cadastro de visitantes:\n");
			printf("7- para sair e imprimir todas relacoes:\n");
			printf("digite os numeros acima: ");
			scanf("%i", &opcao);
			fflush(stdin);

			if (opcao !=1 && opcao !=2 && opcao !=3 && opcao !=4 && opcao !=5 && opcao !=6 && opcao !=7)
				memset (&opcao, 0, sizeof (opcao));
			
		}while (opcao !=1 && opcao !=2 && opcao !=3 && opcao !=4 && opcao !=5 && opcao !=6 && opcao !=7);

        switch(opcao) 
		{
            case 1:
            {
                adicionar_entrada(&vetor);
                break;
            }
            case 2:
            {
                ver_dados(vetor);
				break; 
            }
            case 3:
            {
                ver_dados_oficiais(vetor);
				break; 
            }
            case 4:
            {
                ver_dados_graduados(vetor);
				break; 
            }
            case 5:
            {
                ver_dados_servidores(vetor);
				break; 
            }
            case 6:
            {
                ver_dados_visitantes(vetor);
				break; 
            }
            case 7:
            {
                arquivo_lista_completa_data = fopen("Lista_Completa_por_data.txt", "a"); 
				arquivo_lista_oficiais = fopen("Lista_de_Oficias_por_data.txt", "a"); 
				arquivo_lista_graduados = fopen("Lista_de_Graduados_por_data.txt", "a");
				arquivo_lista_serv_civ = fopen("Lista_de_Servidores_Civis_por_data.txt", "a");
				arquivo_lista_visitante = fopen("Lista_de_Visitantes_por_data.txt", "a");
				
				fprintf(arquivo_lista_completa_data, "****************************************************   LISTA DE ENTRADA    *******************************************************\n");
				fprintf(arquivo_lista_completa_data, "Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
				for(int i = 0; i < vetor.tamanho; i++) 
				{
        			fprintf(arquivo_lista_completa_data, "%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        			vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        			vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        			vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       				vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       				vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    			}
    			
    			fprintf(arquivo_lista_oficiais, "****************************************************   LISTA DE ENTRADA    *******************************************************\n");
				fprintf(arquivo_lista_oficiais, "Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
				for(int i = 0; i < vetor.tamanho; i++)
				{
					if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Cel") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"cel") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"TC")==0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"tc") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Maj") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"maj") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Cap") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"cap") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"1 Ten") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"1 ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"2 Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"2 ten") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Asp Of") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"asp of") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"Asp") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"asp")==0)
					{
        				fprintf(arquivo_lista_oficiais, "%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        				vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        				vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        				vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       					vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       					vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    				}
    			}
    			
    			fprintf(arquivo_lista_graduados, "****************************************************   LISTA DE ENTRADA    *******************************************************\n");
				fprintf(arquivo_lista_graduados, "Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
    			for(int i = 0; i < vetor.tamanho; i++) 
				{
					if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad,"ST") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1 Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2 Sgt") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3 Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Cb") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd EP") == 0 || 
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd EV") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sub Ten") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1 sgt") == 0 || 
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2 sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3 sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "cb") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd ep") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sd ev") == 0 || 
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Sd") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sub") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3º sgt") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2º sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1º sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "3º Sgt") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "2º Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "1º Sgt") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Aluno") == 0 || 
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "aluno") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Al") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "al") == 0)
					{
        				fprintf(arquivo_lista_graduados, "%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        				vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        				vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        				vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       					vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       					vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    				}
    			}
    			
    			fprintf(arquivo_lista_serv_civ, "****************************************************   LISTA DE ENTRADA    *******************************************************\n");
				fprintf(arquivo_lista_serv_civ, "Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
				for(int i = 0; i < vetor.tamanho; i++) 
				{
					if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "SC") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "sc") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Serv Civ") == 0 ||
					strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "servidor civil") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Servidor Civil") == 0)
					{
        				fprintf(arquivo_lista_serv_civ, "%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        				vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        				vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        				vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       					vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       					vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    				}
    			}
    			
    			fprintf(arquivo_lista_visitante, "****************************************************   LISTA DE ENTRADA    *******************************************************\n");
				fprintf(arquivo_lista_visitante, "Posto/Grad/SC/Visitante  |                nome                 | dia/mes/ano |hora: min(entrada)| hora: min(saida)| destino  | CPF\n");
				for(int i = 0; i < vetor.tamanho; i++) 
				{
					if(strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Visitante") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "visitante") == 0 || strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "Vist") == 0
					|| strcmp(vetor.controle_entrada[i].pessoa.posto_grad, "vist") == 0)
					{
				
        				fprintf(arquivo_lista_visitante, "%24s | %35s | %2.2i / %2.2i/%4i|  %2.2i: %2.2i          |   %2.2i: %2.2i        | %8s | %.14s\n", 
        				vetor.controle_entrada[i].pessoa.posto_grad, vetor.controle_entrada[i].pessoa.nome,
        				vetor.controle_entrada[i].data.dia, vetor.controle_entrada[i].data.mes, vetor.controle_entrada[i].data.ano,
        				vetor.controle_entrada[i].hora_entrada.horas, vetor.controle_entrada[i].hora_entrada.minutos,
       					vetor.controle_entrada[i].hora_saida.horas, vetor.controle_entrada[i].hora_saida.minutos, 
       					vetor.controle_entrada[i].destino, vetor.controle_entrada[i].pessoa.cpf);
    				}
    			}
				fclose(arquivo_lista_completa_data);
				fclose(arquivo_lista_oficiais);
				fclose(arquivo_lista_graduados);
				fclose(arquivo_lista_serv_civ);
				fclose(arquivo_lista_visitante);
				exit(0); 
            }
            default:
        	{	
				printf("\nOpcao invalida. Tente novamente:\n\n");
        		break;
        	}
        }
    }
    return 0;
}
