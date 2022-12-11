#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct {
	char Name[50];
	char cpf[12];
	float salario;
	int sit; // 0 = ativo, 1 = excluído	
} cadastro;

FILE *fp;

// funções
int busca(char*);
int cadastra();
int consulta();
int altera();
int deleta();
void listagem(int);
int recuperar();
void menu();

void menu() {
	printf ("1 - Novo Cadastro\n");
	printf ("2 - Alterar Dados\n");
	printf ("3 - Consultar Dados\n");
	printf ("4 - Listagem dos Cadastros\n");
	printf ("5 - Excluir Dados\n");
	printf ("6 - Recuperar Dados\n");
	printf ("7 - Lixeira\n");
	printf("0 - Sair\n\n");
	printf("OPCAO: ");
}

int main() {
	int option;

	fp = fopen("cadastros.dad", "rb+");
		if(fp == NULL) {
			fp = fopen("cadastros.dad", "wb+");
		}

	do {
		system("cls");
		menu();
		scanf("%d", &option);

		switch(option) {
			case 1: cadastra(); break; 
			case 2: altera(); break;
			case 3: consulta(); break;
			case 4: listagem(0); break; // 0 - cadastros ativos
			case 5: deleta(); break;
			case 6: recuperar();break;
			case 7: listagem(1); // 1 - cadastros excluidos
			default: break;
		}
	} while(option != 0);
	fclose(fp);
}

int busca(char* cpf) {
	int i = 0;
	cadastro funcionario;

	// move a posição corrente de leitura/escrita no arquivo de um valor
	// especificado, a partir de um ponto especificado.
	fseek(fp, 0, SEEK_SET);

	while(!feof(fp)) {
		fread(&funcionario, sizeof(cadastro), 1, fp);
		// comparar o conteúdo de duas strings
		// se a comparação for igual a 0, as strings são iguais
		if(strcmp(funcionario.cpf, cpf) == 0) return i;
		i++;
	}	
	return -1;
}

int cadastra() {
	char CPF[12], NOME[50];
	cadastro funcionario;
	int indice;
	float SALARIO;

	system("cls");
	fflush(stdin);

	printf("CPF: ");
	gets(CPF);

	if(strlen(CPF) != 11){
		printf("CPF INVALIDO - deve conter 11 digitos!");
		Sleep(2000); // mostra a mensagem por 2 segundos
		return 0;
	}

	for(int i = 0; i < strlen(CPF); i++)
	{
		if(CPF[i] > 57)	
		{
			printf("CPF INVALIDO - deve conter apenas NUMEROS!");
			Sleep(2000); // mostra a mensagem por 2 segundos
			return 0;			
		}	
		else if(CPF[i] < 48)
		{
			printf("CPF INVALIDO - deve conter apenas NUMEROS!");
			Sleep(2000); // mostra a mensagem por 2 segundos
			return 0;			
		}
	}

	indice = busca(CPF);

	if(indice != -1) {
		printf("\nCPF ja cadastrado\n");
		Sleep(2000);
		return 0;
	}

	printf("\nNOME: ");
	fflush(stdin);
	gets(NOME);

	printf("\nSALARIO: ");
	fflush(stdin);
	scanf("%f", &SALARIO);

	strcpy(funcionario.cpf, CPF);
	strcpy(funcionario.Name, NOME);
	funcionario.salario = SALARIO;
	funcionario.sit = 0;

	fwrite(&funcionario, sizeof(cadastro), 1, fp);
}

void listagem(int N) {
	cadastro funcionario;
	int quantBytes, i;
	system("cls");
	fseek(fp, 0, SEEK_END);

	// função que retorna a quantidade de bytes do arquivo
	quantBytes = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("CPF        		NOME				SALARIO\n\n");

	for(i = 0; i < quantBytes/sizeof(cadastro); i++) {
		fread(&funcionario, sizeof(cadastro), 1, fp);
		if(funcionario.sit == N) {
			printf("%s		%-20s		%.2f\n", funcionario.cpf, funcionario.Name, funcionario.salario);
		}
	}

	system("pause");
}

int altera() {
	cadastro funcionario;
	char CPF[12], novoNome[50];
	int indice, option;
	float novoSalario;
	system("cls");

	printf("ALTERAR DADOS DO CPF: ");
	fflush(stdin);
	gets(CPF);

	indice = busca(CPF);

	if(indice == -1) {
		printf("\nCPF nao encontrado\n");
		Sleep(2000);
	} else {
		fseek(fp, indice*sizeof(cadastro), SEEK_SET);
		fread(&funcionario, sizeof(cadastro), 1, fp);

		if(funcionario.sit == 1) {
			printf("\nCadastro excluido");
			Sleep(2000);
			return 0;
		}

		printf("CPF: %s		NOME: %-20s		SALARIO: %.2f\n", funcionario.cpf, funcionario.Name, funcionario.salario);
		printf("Tipo de dado a ser alterado:\n1-Nome\n2-Salario\n");
		fflush(stdin);
		scanf("%d", &option);

		switch(option) {
			case 1:
				printf("\nAlterar NOME: ");
				fflush(stdin);
				gets(novoNome);

				strcpy(funcionario.Name, novoNome);
				fseek(fp, indice*sizeof(cadastro), SEEK_SET);
				fwrite(&funcionario, sizeof(funcionario), 1, fp);

				break;
			case 2:
				printf("\nAlterar SALARIO: ");
				fflush(stdin);
				scanf("%f", &novoSalario);

				funcionario.salario = novoSalario;
				fseek(fp, indice*sizeof(cadastro), SEEK_SET);
				fwrite(&funcionario, sizeof(cadastro), 1, fp);

				break;
			default: return 0;
		}
		system("pause");
	}
}

int consulta() {
	cadastro funcionario;
	char CPF[12];
	int indice;
	system("cls");

	printf("CPF a ser consultado: ");
	fflush(stdin);
	gets(CPF);

	indice = busca(CPF);

	if(indice == -1) {
		printf("\nCPF nao encontrado\n");
		Sleep(2000);
	} else {
		fseek(fp, indice*sizeof(cadastro), SEEK_SET);
		fread(&funcionario, sizeof(cadastro), 1, fp);

		if(funcionario.sit == 1) {
			printf("\nCadastro excluido");
			Sleep(2000);
			return 0;
		}

		printf("CPF: %s		NOME: %-20s		SALARIO: %.2f\n", funcionario.cpf, funcionario.Name, funcionario.salario);
		system("pause");
	}
}

int deleta() {
	cadastro funcionario;
	char CPF[12];
	int indice, confirm;
	system("cls");

	printf("CPF a ser apagado: ");
	fflush(stdin);
	gets(CPF);

	indice = busca(CPF);
	
	if(indice  == -1){
		printf("\nCPF nao encontrado\n");
		Sleep(2000);
	} else {
		fseek(fp, indice*sizeof(cadastro), SEEK_SET);
		fread(&funcionario, sizeof(cadastro), 1, fp);

		printf("\nO CPF: %s --- %s\n\nConfirmar exclusao - 1\nAnular exclusao - 0:\n\nOPCAO: ", funcionario.cpf, funcionario.Name);
		fflush(stdin);
		scanf("%d", &confirm);

		switch(confirm) {
			case 1: 
				funcionario.sit = 1;
				fseek(fp, indice*sizeof(cadastro), SEEK_SET);
				fwrite(&funcionario, sizeof(cadastro), 1, fp);
			default: return 0;
		}
	}}
	
int recuperar() {
	cadastro funcionario;
	char CPF[12];
	int indice, confirm;
	system("cls");

	printf("CPF a ser recuperado: ");
	fflush(stdin);
	gets(CPF);

	indice = busca(CPF);

	if(indice == -1) {
		printf("\nCPF nao encontrado\n");
		Sleep(2000);
	} else {
		fseek(fp, indice*sizeof(cadastro), SEEK_SET);
		fread(&funcionario, sizeof(cadastro), 1, fp);

		printf("\nO CPF: %s --- %s\n\nConfirmar recuperacao - 1\nAnular recuperacao - 0:\n\nOPCAO: ", funcionario.cpf, funcionario.Name);
		fflush(stdin);
		scanf("%d", &confirm);

		switch(confirm) {
			case 1: 
				funcionario.sit = 0;
				fseek(fp, indice*sizeof(cadastro), SEEK_SET);
				fwrite(&funcionario, sizeof(cadastro), 1, fp);
			default: return 0;
		}
	}
}