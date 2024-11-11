#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<locale.h>

                 // Função para posicionar o cursor no terminal
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void set_color(int text_color, int bg_color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bg_color << 4) | text_color);
}

void reset_color() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07); // Branco no fundo preto
}

// Função para exibir o menu
void print_menu(int highlight, const char *choices[], int n_choices) {
    

    system("cls");  // Limpa a tela

    // Desenha bordas do menu
    gotoxy(8, 3);  // Posição para o topo do menu
    set_color(15, 0);
    printf("|-------------------------|\n");
    
    gotoxy(8, 4);
    set_color(15, 0);
    printf("|    MENU PRINCIPAL       |\n");

    gotoxy(8, 5);
    set_color(15, 0);
    printf("|-------------------------|\n");

                               // Exibe as opções do menu com espaçamento certo
    for (int i = 0; i < n_choices; ++i) {
        gotoxy(8, 6 + i);  
        if (highlight == i) {
        	set_color(15, 0);
                                         // setas para a posição selecionada
            printf("|  > %-21s|\n", choices[i]);
        } else {
            printf("|    %-21s|\n", choices[i]);
        }
    }

                 // Exibe a borda inferior do menu
    gotoxy(8, 6 + n_choices);
    set_color(15, 0);
    printf("|-------------------------|\n");

	gotoxy(8, 15 + n_choices + 1);  
    set_color(7, 0);  
    printf("USE AS SETAS PARA NAVEGAR E ENTER PARA SELECIONAR\n");
}



void criptografar_senha(const char *senha, char *senha_cripto) {
    for (int i = 0; senha[i] != '\0'; i++) {
        senha_cripto[i] = senha[i] + 3;  // Desloca cada caractere em 3 posições
    }
    senha_cripto[strlen(senha)] = '\0'; // Adiciona o caractere nulo ao final
}

int verificarSenha(const char *senha) {
    int tamanho = strlen(senha);
    int temMaiusc = 0, temMinusc = 0, temNumero = 0, temEspecial = 0;    
    if (tamanho < 8 || tamanho > 12) {
    	set_color(4,0);
        printf("Senha deve ter entre 8 e 12 caracteres.\n");
        return 0;
    }
    for (int i = 0; i < tamanho; i++) {
        if (isupper(senha[i])) temMaiusc++;
        if (islower(senha[i])) temMinusc++;
        if (isdigit(senha[i])) temNumero++;
        if (ispunct(senha[i])) temEspecial++;
    }
    if (temMaiusc && temNumero && temEspecial) {
        return 1; // Senha no padrão
    } else {
    	set_color(6,0);
        printf("Senha deve ter:\n");
        reset_color();
        set_color(4,0);
        if (!temMaiusc) printf("- Pelo menos um caractere maiúsculo.\n");
        if (!temNumero) printf("- Pelo menos um número.\n");
        if (!temEspecial) printf("- Pelo menos um caractere especial.\n");
        reset_color();
        return 0; // Senha fraca
    }
}

void cadastrarUsuario(const char *nome, const char *senha) {
    FILE *arquivo = fopen("usuarios.txt", "a"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char senha_cripto[50];  
    criptografar_senha(senha, senha_cripto);  

    // Adiciona o nome e a senha criptografada no arquivo
    fprintf(arquivo, "%s %s\n", nome, senha_cripto);  
    fclose(arquivo);
 	set_color(2,0);
    printf("\nUSUÁRIO CADASTRADO COM SUCESSO.\n");
    Beep(2000,500);
    reset_color();
}

void listarUsuarios() {
    FILE *arquivo = fopen("usuarios.txt", "r");  // Abre o arquivo para leitura
    if (arquivo == NULL) {
        printf("Nenhum usuário cadastrado.\n");
        return;
    }

    char linha[100];  // Buffer para armazenar a linha
    char nome[50], senha_cripto[50];
    set_color(6,0);
    printf("USUARIOS CADASTRADOS:\n");
    reset_color();

    while (fgets(linha, sizeof(linha), arquivo)) { 
        sscanf(linha, "%49s %49s", nome, senha_cripto); 
        printf("Nome: %s -> Senha criptografada: %s\n", nome, senha_cripto);
    }

    fclose(arquivo);
}


void excluirUsuario(const char *nome) {
    char escolha[50];
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    char nomeUsuario[50], senha_cripto[50];
    int encontrado = 0;
    while (fscanf(arquivo, "%s %s", nomeUsuario, senha_cripto) != EOF) {
        if (strcmp(nomeUsuario, nome) != 0) {
            fprintf(temp, "%s %s\n", nomeUsuario, senha_cripto);  // Copia p arquivo temporario
        } else {
            encontrado++; 
        }
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
    	set_color(4,0);
        printf("\nDeseja excluir este usuário? (Digite 'Sim' para confirmar e 'Nao' para cancelar):\n ");
        reset_color();
        scanf("%49s", escolha);
        if (strcmp(escolha, "Sim") == 0) {
            remove("usuarios.txt");
            rename("temp.txt", "usuarios.txt"); // Renomeia o arquivo temporário para o original
            set_color(2,0);
            printf("\nUSUÁRIO EXCLUÍDO COM SUCESSO.\n");
            Beep(2000,500);
			reset_color();    
        } else if (strcmp(escolha, "Nao") == 0) {
            remove("temp.txt");  // Não faz a substituição, apaga o arquivo temporário
            set_color(2,0);
            printf("\nOPERAÇÃO DE EXCLUSÃO CANCELADA.\n");
            reset_color();
        }
    } else {
        remove("temp.txt");  // Remove o arquivo temporário se o usuário não foi encontrado
        set_color(4,0);
        printf("\nUsuário não encontrado, pressione ENTER para tentar novamente\n");
        reset_color();
		getchar();
        system("cls");
        listarUsuarios();
        printf("\nInforme o nome do usuário a ser excluído: ");
        char novoNome[50];
        fgets(novoNome, sizeof(novoNome), stdin);
        novoNome[strcspn(novoNome, "\n")] = '\0';

        
        excluirUsuario(novoNome);  
    }
}

void alterarUsuario(const char *nome) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(arquivo);
        return;
    }

    char nomeUsuario[50], senha_cripto[50];
    int encontrado = 0;
    while (fscanf(arquivo, "%49s %49s", nomeUsuario, senha_cripto) != EOF) {
        if (strcmp(nomeUsuario, nome) == 0) {
            encontrado = 1;
            set_color(3,0);
            printf("Usuário encontrado.\n");
            reset_color();

           
            char novoNome[50], novaSenha[50];
            set_color(3,0);
            printf("Informe o novo nome (ou pressione Enter para manter o atual): ");
            reset_color();
            fgets(novoNome, sizeof(novoNome), stdin);
            novoNome[strcspn(novoNome, "\n")] = '\0'; // Remove o '\n'

            if (strlen(novoNome) == 0) {
                strcpy(novoNome, nomeUsuario); // Mantém o nome atual
            }
			set_color(3,0);
            printf("Informe a nova senha: ");
            reset_color();
            fgets(novaSenha, sizeof(novaSenha), stdin);
            novaSenha[strcspn(novaSenha, "\n")] = '\0';

            char senha_cripto_nova[50];
            criptografar_senha(novaSenha, senha_cripto_nova); // Criptografa a nova senha

            // Salva as alterações no arquivo temporário
            fprintf(temp, "%s %s\n", novoNome, senha_cripto_nova);
        } else {
            // Copia os dados não alterados para o arquivo temporário
            fprintf(temp, "%s %s\n", nomeUsuario, senha_cripto);
        }
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        set_color(2,0);
        printf("USUÁRIO ALTERADO COM SUCESSO.\n");
        reset_color();
    } else {
    	set_color(4,0);
        printf("USUÁRIO NÃO ENCONTRADO.\n");
        reset_color();
        remove("temp.txt");
    }
}

void mostrarCreditos() {
	system("cls");
	set_color(4,0);
    printf("\n----- Créditos -----\n");
    reset_color();
    printf("Desenvolvedores: Gean Luca, Adriano Mota e Gabriel Bott.\n");
    printf("Projeto: Sistema de Cadastro de Usuários\n");
    printf("Tecnologias usadas: C, Banco de Dados txt simples e Criptografia de Senha.\n");
    printf("Data de desenvolvimento: 08/11/24 -- 11/11/24\n");
    printf("Versão: 1.0\n");
    printf("Agradecimentos: Agradecemos aos professores Alexandre Moreno e Erinaldo por nos ajudarem e nos incentivarem a fazer este projeto.\n");
    printf("---------------------\n");
}

int main() {
	setlocale(LC_ALL, "portuguese");
    int highlight = 0;
    int choice = -1;
    char ch;

    const char *choices[] = { 
		"CADASTRAR USUARIO",
        "LISTAR USUARIOS ",
		"ALTERAR USUÁRIO ",       
		"EXCLUIR USUARIOS",
		"CRÉDITOS",
        "SAIR",
    };
    int n_choices = sizeof(choices) / sizeof(choices[0]);

    char nome[50][20];
    char senha[50][20];
    char senha_confirmacao[50];
    char senha_cripto[50];  // Armazenará a senha criptografada

    // Exibir o menu inicialmente
    print_menu(highlight, choices, n_choices);

    
    while (1) {
    
    print_menu(highlight, choices, n_choices);
    ch = _getch();  // Captura a tecla pressionada

    
    switch (ch) {
        case 72: // Seta para cima
            if (highlight == 0) 
                highlight = n_choices - 1;
            else
                --highlight;
            break;
        case 80: // Seta para baixo
            if (highlight == n_choices - 1) 
                highlight = 0;
            else 
                ++highlight;
            break;
        case 13: // Enter
            choice = highlight;
			break;
        case 8: // Backspace
                // Volta ao menu sem sair
                choice = -1;
                break;
            default:
                break;
            
    	        
    
	
	
	}

    // Se a opção foi selecionada (Enter pressionado), executa a ação
    if (choice != -1) {
        Beep(450,300);
		break;
	}
}



    if (choice == 0) {                   
    	system("cls");
    	set_color(3,0);
    	printf("INFORME SEU NOME :\n");
    	reset_color();
    	fgets(nome[0], sizeof(nome[0]), stdin);  
    	nome[0][strcspn(nome[0], "\n")] = '\0';   // Remove o '\n' no final, se presente

    	int tamanho = strlen(nome[0]);  
		while (tamanho < 4) {
			set_color(4,0);  
        	printf("\nNome precisa de no minimo 4 caracteres, pressione ENTER para informar novamente:\n");
        	reset_color();
			getchar();  
        	system("cls");
        	set_color(3,0);
        	printf("INFORME SEU NOME :\n");
        	reset_color();
        	fgets(nome[0], sizeof(nome[0]), stdin);  
        	nome[0][strcspn(nome[0], "\n")] = '\0';   
        	tamanho = strlen(nome[0]);  
    	}
		
		int senhaValida = 0;
    	while (!senhaValida) {
			set_color(3,0);    
        	printf("DIGITE SUA SENHA: ");
        	reset_color();
        	fgets(senha[0], sizeof(senha[0]), stdin);   
        	senha[0][strcspn(senha[0], "\n")] = '\0';    
        	if (verificarSenha(senha[0])) {
				set_color(3,0);    
            	printf("DIGITE A SENHA NOVAMENTE: ");
            	reset_color();
            	fgets(senha_confirmacao, sizeof(senha_confirmacao), stdin);
            	senha_confirmacao[strcspn(senha_confirmacao, "\n")] = '\0'; 
            	if (strcmp(senha[0], senha_confirmacao) == 0) {
                	cadastrarUsuario(nome[0], senha[0]);
                	int sleep(1000);
                	senhaValida = 1;                         // Senha e confirmação estão corretas, podemos sair do loop
            	}else{
                set_color(4,0);
				printf("Senha informada está diferente, informe novamente.\n");
				getch();
				system("cls");
            	}reset_color();
        	}	else {
        			set_color(4,0);
            		printf("Senha não atende aos requisitos. Tente novamente.\n");
            		getch();
            		system("cls");
        		}	reset_color();
    }



    
    } else if (choice == 1) { 
        while(1) {
		system("cls");
        listarUsuarios();
        printf("\n\n\nPressione Backspace para retornar ao menu...\n");
        ch = _getch();
		 if (ch == 8) {
		 system("cls");
		 print_menu(highlight, choices, n_choices);
		 break;
		 
		 
		}
		}
        

	} else if (choice == 3) { 
        system("cls");
        listarUsuarios();
        printf("\nINFORME O NOME DO USUÁRIO A SER EXCLUÍDO:\n");
		fgets(nome[0], sizeof(nome[0]), stdin);
        nome[0][strcspn(nome[0], "\n")] = '\0';
        excluirUsuario(nome[0]);
    
	} else if (choice == 5) { // Sair
        char escolha[4];
		system("cls");
        set_color(4,0);
        printf("Deseja mesmo sair? Digite 'sim' ou 'nao'\n");
        reset_color();
		fgets(escolha, sizeof(escolha), stdin);
    	escolha[strcspn(escolha, "\n")] = '\0';  

    if (strcmp(escolha, "sim") == 0) { 
        set_color(2, 0);  
        printf("\n\nSaindo...\n");
        reset_color(); 
    } else if (strcmp(escolha, "nao") == 0) { 
        system("cls");
        print_menu(highlight, choices, n_choices);  // Mostra o menu novamente
        choice = -1;
    } else {
        printf("Opção inválida. Digite 'sim' ou 'nao'.\n");
        getch();
		system("cls");
        
    }

}	else if (choice == 2) {
    	system("cls");
    	listarUsuarios();
    	set_color(3,0);
    	printf("\nINFORME O NOME DO USUÁRIO A SER ALTERADO:\n");
    	reset_color();
    	fgets(nome[0], sizeof(nome[0]), stdin);
    	nome[0][strcspn(nome[0], "\n")] = '\0';
    	alterarUsuario(nome[0]);
	
	}else if (choice==4) {
		mostrarCreditos();
		
	}
	
	
	
	return 0;
}

