/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. Almeida
 *
 * Trabalho 02 - Árvore-B
 *
 * ========================================================================== *
 *   <<< IMPLEMENTE AQUI!!! COPIE E COLE O CONTEÚDO DESTE ARQUIVO NO AVA >>>
 * ========================================================================== */

/* Bibliotecas */
#include "ORI_T02_HEADER.h"

/* ===========================================================================
 * ================================= FUNÇÕES ================================= */

/* <<< COLOQUE AQUI OS DEMAIS PROTÓTIPOS DE FUNÇÕES, SE NECESSÁRIO >>> */
void btree_node_free(btree_node no) {
	free(no.filhos);
	for(int aux = 0; aux < btree_order - 1; aux++){
		no.chaves[aux] = NULL;
	}
	free(no.chaves);
}

/* Compara a chave (key) com cada elemento do índice (elem).
 * Funções auxiliares para o buscar e inserir chaves em Árvores-B.
 * Note que, desta vez, as funções comparam chaves no formato string, e não struct.
 * "key" é a chave do tipo string que está sendo buscada ou inserida. 
 * "elem" é uma chave do tipo string da struct btree_node.
 *
 * Dica: consulte sobre as funções strncmp() e strnlen(). Muito provavelmente, você vai querer utilizá-las no código.
 * */

/* Função de comparação entre chaves do índice corredores_idx */
int order_corredores_idx(const void *key, const void *elem) {
    return strncmp(key, elem, TAM_ID_CORREDOR-1);
}

/* Função de comparação entre chaves do índice veiculos_idx */
int order_veiculos_idx(const void *key, const void *elem) {
    
    return strncmp(key, elem, TAM_ID_VEICULO-1);
}

/* Função de comparação entre chaves do índice pistas_idx */
int order_pistas_idx(const void *key, const void *elem) {
    
    return strncmp(key, elem, TAM_ID_PISTA-1);
}

/* Função de comparação entre chaves do índice corridas_idx */
int order_corridas_idx(const void *key, const void *elem) {
    
    return strncmp(key, elem, TAM_DATETIME+TAM_ID_PISTA-2);
}

/* Função de comparação entre chaves do índice nome_pista_idx */
int order_nome_pista_idx(const void *key, const void *elem) {
	
    return strncmp(key, elem, TAM_MAX_NOME_PISTA-1);
}

/* Função de comparação entre chaves do índice preco_veiculo_idx */
int order_preco_veiculo_idx(const void *key, const void *elem) {
	
    return strncmp(key, elem, TAM_FLOAT_NUMBER+TAM_ID_VEICULO-2);
}

/* Função de comparação entre chaves do índice secundário de corredor_veiculos_secundario_idx */
int order_corredor_veiculo_idx(const void *key, const void *elem) {
	
    return strncmp(key, elem, TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);
}

/* Cria o índice respectivo */
void criar_corredores_idx() {
    char corredor_str[TAM_CHAVE_CORREDORES_IDX + 1];
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        Corredor c = recuperar_registro_corredor(i);

        sprintf(corredor_str, "%s%04d", c.id_corredor, i);
        btree_insert(corredor_str, &corredores_idx);
    }
    printf(INDICE_CRIADO, "corredores_idx");
}

void criar_veiculos_idx() {
    
    char veiculos_str[TAM_CHAVE_VEICULOS_IDX + 1];
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);

        sprintf(veiculos_str, "%s%04d", v.id_veiculo, i);
        btree_insert(veiculos_str, &veiculos_idx);
    }
    printf(INDICE_CRIADO, "veiculos_idx");
}

void criar_pistas_idx() {
    
    char pistas_str[TAM_CHAVE_PISTAS_IDX + 1];
    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);

        sprintf(pistas_str, "%s%04d", p.id_pista, i);
        btree_insert(pistas_str, &pistas_idx);
    }
    printf(INDICE_CRIADO, "pistas_idx");
}

void criar_corridas_idx() {
    
    char corridas_str[TAM_CHAVE_CORRIDAS_IDX + 1];
    for (unsigned i = 0; i < qtd_registros_corridas; ++i) {
        Corrida c = recuperar_registro_corrida(i);

        sprintf(corridas_str, "%s%s%04d", c.ocorrencia, c.id_pista, i);
        btree_insert(corridas_str, &corridas_idx);
    }
    printf(INDICE_CRIADO, "corridas_idx");
}

void criar_nome_pista_idx() {
    
    char pistas_str[TAM_CHAVE_NOME_PISTA_IDX + 2];
	char aux[TAM_MAX_NOME_PISTA];
    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);
		strcpy(aux, p.nome);
		strupr(aux);
		strpadright(aux, '#', 33);
        sprintf(pistas_str, "%s%s", aux, p.id_pista);
        btree_insert(pistas_str, &nome_pista_idx);
    }
    printf(INDICE_CRIADO, "nome_pista_idx");
}

void criar_preco_veiculo_idx() {
    
    char veiculos_str[TAM_CHAVE_PRECO_VEICULO_IDX + 1];
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);

        sprintf(veiculos_str, "%013.2lf%s", v.preco, v.id_veiculo);
        btree_insert(veiculos_str, &preco_veiculo_idx);
    }
    printf(INDICE_CRIADO, "preco_veiculo_idx");
}

void criar_corredor_veiculos_idx() {
    
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        Corredor c = recuperar_registro_corredor(i);

        for(int aux=0; aux < QTD_MAX_VEICULO; aux++){
			if(strncmp(c.veiculos[aux], "\0", 1) != 0)
				inverted_list_insert(c.veiculos[aux], c.id_corredor, &corredor_veiculos_idx);
		}
    }
    printf(INDICE_CRIADO, "corredor_veiculos_idx");
}

/* Exibe um registro com base no RRN */
bool exibir_corredor(int rrn) {
    if (rrn < 0)
        return false;
 
    Corredor c = recuperar_registro_corredor(rrn);
 
    printf("%s, %s, %s, %s, %.2lf\n", c.id_corredor, c.nome, c.apelido, c.cadastro, c.saldo);
    return true;
}

bool exibir_veiculo(int rrn) {
    
	if (rrn < 0)
        return false;
 
    Veiculo v = recuperar_registro_veiculo(rrn);
    printf("%s, %s, %s, %s, %d, %d, %d, %.2lf\n", v.id_veiculo, v.marca, v.modelo, v.poder, v.velocidade, v.aceleracao, v.peso, v.preco);
    return true;
}

bool exibir_pista(int rrn) {
    
	if (rrn < 0)
        return false;
	
    Pista p = recuperar_registro_pista(rrn);
	
    printf("%s, %s, %d, %d, %d, %.2lf\n", p.id_pista, p.nome, p.dificuldade, p.distancia, p.recorde, p.taxa_inscricao);
    return true;
}

bool exibir_corrida(int rrn) {
    
	if (rrn < 0)
        return false;
	
    Corrida c = recuperar_registro_corrida(rrn);
 
    printf("%s, %s, %s, %s\n",c.ocorrencia, c.id_pista, c.id_corredores, c.id_veiculos);
    return true;
}

/* Exibe um registro com base na chave de um btree_node */
bool exibir_btree_corredores(char *chave) {
	int rrn = atoi(chave + TAM_ID_CORREDOR - 1);
    return exibir_corredor(rrn);
}

bool exibir_btree_veiculos(char *chave) {
	
	int rrn = atoi(chave + TAM_ID_VEICULO - 1);
    return exibir_veiculo(rrn);
}

bool exibir_btree_pistas(char *chave) {
	
	int rrn = atoi(chave + TAM_ID_PISTA - 1);
    return exibir_pista(rrn);
}

bool exibir_btree_corridas(char *chave) {
	
	int rrn = atoi(chave + TAM_DATETIME+TAM_ID_PISTA-2);
    return exibir_corrida(rrn);
}

bool exibir_btree_nome_pista(char *chave) {
	
	int rrn = atoi(chave + TAM_MAX_NOME_PISTA-1);
    return exibir_pista(rrn);
}

bool exibir_btree_preco_veiculo(char *chave) {
	
	int rrn = atoi(chave + TAM_FLOAT_NUMBER-1);
    return exibir_veiculo(rrn);
}

/* Recupera do arquivo o registro com o RRN informado
 * e retorna os dados nas structs */
Corredor recuperar_registro_corredor(int rrn) {
	Corredor c;
	char temp[TAM_REGISTRO_CORREDOR + 1], *p;
	strncpy(temp, ARQUIVO_CORREDORES + (rrn * TAM_REGISTRO_CORREDOR), TAM_REGISTRO_CORREDOR);
	temp[TAM_REGISTRO_CORREDOR] = '\0';

	p = strtok(temp, ";");
	strcpy(c.id_corredor, p);
	p = strtok(NULL, ";");
	strcpy(c.nome, p);
	p = strtok(NULL, ";");
	strcpy(c.apelido, p);
	p = strtok(NULL, ";");
	strcpy(c.cadastro, p);
	p = strtok(NULL, ";");
	c.saldo = atof(p);
	p = strtok(NULL, ";");

	for(int i = 0; i < QTD_MAX_VEICULO; ++i)
		c.veiculos[i][0] = '\0';

	if(p[0] != '#') {
		p = strtok(p, "|");

		for(int pos = 0; p; p = strtok(NULL, "|"), ++pos)
			strcpy(c.veiculos[pos], p);
	}

	return c;
}

Veiculo recuperar_registro_veiculo(int rrn) {
	Veiculo v;
	
	char temp[TAM_REGISTRO_VEICULO + 1], *p;
	strncpy(temp, ARQUIVO_VEICULOS + (rrn * TAM_REGISTRO_VEICULO), TAM_REGISTRO_VEICULO);
	temp[TAM_REGISTRO_VEICULO] = '\0';
		
	p = strtok(temp, ";");
	strcpy(v.id_veiculo, p);
	p = strtok(NULL, ";");
	strcpy(v.marca, p);
	p = strtok(NULL, ";");
	strcpy(v.modelo, p);
	p = strtok(NULL, ";");
	strcpy(v.poder, p);
	p = strtok(NULL, ";");
	v.velocidade = atoi(p);
	p = strtok(NULL, ";");
	v.aceleracao = atoi(p);
	p = strtok(NULL, ";");
	v.peso = atoi(p);
	p = strtok(NULL, ";");
	v.preco = atof(p);
	p = strtok(NULL, ";");
    
	return v;
}

Pista recuperar_registro_pista(int rrn) {
	Pista p;
	
	char temp[TAM_REGISTRO_PISTA + 1], *a;
	strncpy(temp, ARQUIVO_PISTAS + (rrn * TAM_REGISTRO_PISTA), TAM_REGISTRO_PISTA);
	temp[TAM_REGISTRO_PISTA] = '\0';

	a = strtok(temp, ";");
	strcpy(p.id_pista, a);
	a = strtok(NULL, ";");
	strcpy(p.nome, a);
	a = strtok(NULL, ";");
	p.dificuldade = atoi(a);
	a = strtok(NULL, ";");
	p.distancia = atoi(a);
	a = strtok(NULL, ";");
	p.recorde = atoi(a);
	a = strtok(NULL, ";");
	p.taxa_inscricao = atoi(a);
	a = strtok(NULL, ";");

	return p;
}

Corrida recuperar_registro_corrida(int rrn) {
	Corrida c;
	
	char temp[TAM_REGISTRO_CORRIDA + 1];
	strncpy(temp, ARQUIVO_CORRIDAS + (rrn * TAM_REGISTRO_CORRIDA), TAM_REGISTRO_CORRIDA);
	temp[TAM_REGISTRO_CORRIDA] = '\0';

    strncpy(c.ocorrencia, temp , TAM_DATETIME);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME-1] = '\0';
	strncpy(c.id_pista, temp+TAM_DATETIME-1, TAM_ID_PISTA);
	c.id_pista[TAM_ID_PISTA-1] = '\0';
	strncpy(c.id_corredores, temp + TAM_ID_PISTA + TAM_DATETIME - 2, TAM_ID_CORREDORES);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES-1] = '\0';
	strncpy(c.id_veiculos, temp + TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES-3, TAM_ID_VEICULOS);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES + TAM_ID_VEICULOS-1] = '\0';
	
	return c;
}

/* Escreve em seu respectivo arquivo na posição informada (RRN) */
void escrever_registro_corredor(Corredor c, int rrn) {
	char temp[TAM_REGISTRO_CORREDOR + 1], p[TAM_FLOAT_NUMBER];
	temp[0] = '\0'; p[0] = '\0';

	strcpy(temp, c.id_corredor);
	strcat(temp, ";");
	strcat(temp, c.nome);
	strcat(temp, ";");
	strcat(temp, c.apelido);
	strcat(temp, ";");
	strcat(temp, c.cadastro);
	strcat(temp, ";");
	sprintf(p, "%013.2lf", c.saldo);
	strcat(temp, p);
	strcat(temp, ";");

	for(int i = 0, k = 0; i < QTD_MAX_VEICULO; ++i) {
		if(strlen(c.veiculos[i]) > 0) {
			if (k == 0)
				k = 1;
			else
				strcat(temp, "|");
			strcat(temp, c.veiculos[i]);
		}
	}

	strcat(temp, ";");

	strpadright(temp, '#', TAM_REGISTRO_CORREDOR);

	strncpy(ARQUIVO_CORREDORES + rrn*TAM_REGISTRO_CORREDOR, temp, TAM_REGISTRO_CORREDOR);
}

void escrever_registro_veiculo(Veiculo v, int rrn) {
	
	char temp[TAM_REGISTRO_VEICULO + 1], p[20];
	temp[0] = '\0'; p[0] = '\0';

	strcat(temp, v.id_veiculo);
	strcat(temp, ";");
	strcat(temp, v.marca);
	strcat(temp, ";");
	strcat(temp, v.modelo);
	strcat(temp, ";");
	strcat(temp, v.poder);
	strcat(temp, ";");
	sprintf(p, "%04d", v.velocidade);
	strcat(temp, p);
	strcat(temp, ";");
	sprintf(p, "%04d", v.aceleracao);
	strcat(temp, p);
	strcat(temp, ";");
	sprintf(p, "%04d", v.peso);
	strcat(temp, p);
	strcat(temp, ";");
	sprintf(p, "%013.2lf", v.preco);
	strcat(temp, p);
	strcat(temp, ";");

	strpadright(temp, '#', TAM_REGISTRO_VEICULO);

	strncpy(ARQUIVO_VEICULOS + rrn*TAM_REGISTRO_VEICULO, temp, TAM_REGISTRO_VEICULO);
}

void escrever_registro_pista(Pista p, int rrn) {
	
	char temp[TAM_REGISTRO_PISTA + 1], a[100];
	temp[0] = '\0'; a[0] = '\0';

	strcat(temp, p.id_pista);
	strcat(temp, ";");
	strcat(temp, p.nome);
	strcat(temp, ";");
	sprintf(a, "%04d", p.dificuldade);
	strcat(temp, a);
	strcat(temp, ";");
	sprintf(a, "%04d", p.distancia);
	strcat(temp, a);
	strcat(temp, ";");
	sprintf(a, "%04d", p.recorde);
	strcat(temp, a);
	strcat(temp, ";");
	sprintf(a, "%013.2lf", p.taxa_inscricao);
	strcat(temp, a);
	strcat(temp, ";");

	strpadright(temp, '#', TAM_REGISTRO_PISTA);

	strncpy(ARQUIVO_PISTAS + rrn*TAM_REGISTRO_PISTA, temp, TAM_REGISTRO_PISTA);
}

void escrever_registro_corrida(Corrida i, int rrn) {
	
	char temp[TAM_ARQUIVO_CORRIDAS + 1];
	temp[0] = '\0'; 

	strcat(temp, i.ocorrencia);
	strcat(temp, i.id_pista);
	strcat(temp, i.id_corredores);
	strcat(temp, i.id_veiculos);
	
	strncpy(ARQUIVO_CORRIDAS + rrn*TAM_REGISTRO_CORRIDA, temp, TAM_REGISTRO_CORRIDA);
}

/* Funções principais */
void cadastrar_corredor_menu(char *id_corredor, char *nome, char *apelido) {
    
	if(btree_search(NULL, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx)){
		printf(ERRO_PK_REPETIDA, id_corredor);
	}else{
		Corredor c;
		char corredor_str[TAM_CHAVE_CORREDORES_IDX+1];
		strncpy(corredor_str, "\0", 1);

		strcpy(c.id_corredor, id_corredor);
		strcpy(c.nome, nome);
		strcpy(c.apelido, apelido);

		char datatime[TAM_DATETIME];
		current_datetime(datatime);
		strcpy(c.cadastro, datatime);
		c.saldo = 0;	
		for(int aux = 0; aux < QTD_MAX_VEICULO; aux++){
			c.veiculos[aux][0] = '\0';
		}

		escrever_registro_corredor(c, qtd_registros_corredores);//adicionando nos arquivos
		sprintf(corredor_str, "%s%04d", c.id_corredor, qtd_registros_corredores);
        btree_insert(corredor_str, &corredores_idx);
		qtd_registros_corredores++;

		printf(SUCESSO);
	}
}

void remover_corredor_menu(char *id_corredor) {
    
	Corredor c;
	int aux;
	char result[TAM_CHAVE_CORREDORES_IDX+1];

	if(!btree_search(result, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx))
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else{
		btree_delete(result, &corredores_idx);
		aux = atoi(result + TAM_ID_CORREDOR-1);
		c = recuperar_registro_corredor(aux);
		
		c.id_corredor[0] = '*';
		c.id_corredor[1] = '|';
		escrever_registro_corredor(c, aux);
		printf(SUCESSO);
	}
}

void adicionar_saldo_menu(char *id_corredor, double valor) {
	adicionar_saldo(id_corredor, valor, true);
}

void adicionar_saldo(char *id_corredor, double valor, bool flag) {
    
	Corredor c;
	int aux; 
	char result[TAM_CHAVE_CORREDORES_IDX+1];


	if(valor <= 0)//verificacao se o valor é valido
		printf(ERRO_VALOR_INVALIDO);
	else{
		if(btree_search(result, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx)){
		
			aux = atoi(result + TAM_ID_CORREDOR-1);
			c = recuperar_registro_corredor(aux);
			c.saldo += valor;//adionando a valor na conta
			escrever_registro_corredor(c, aux);
			if(flag)
				printf(SUCESSO);
		} else
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);	
	}	
}

void comprar_veiculo_menu(char *id_corredor, char *id_veiculo) {
	
	Corredor c;
	Veiculo v;
	int aux_c, aux_v;
	char result_c[TAM_CHAVE_CORREDORES_IDX+1];
	char result_v[TAM_CHAVE_VEICULOS_IDX+1];
	
	if((!btree_search(result_c, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx)) || (!btree_search(result_v, false, id_veiculo, veiculos_idx.rrn_raiz, &veiculos_idx)) ){//verificacao se existem
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}else {
		aux_c = atoi(result_c + TAM_ID_CORREDOR-1);
		aux_v = atoi(result_v + TAM_ID_VEICULO-1);
		c = recuperar_registro_corredor(aux_c);
		v = recuperar_registro_veiculo(aux_v);
		
		if(c.saldo < v.preco){ //verificacao se tem saldo para comprar
			printf(ERRO_SALDO_NAO_SUFICIENTE);
			return;
		} else {
			for(int aux=0; aux <QTD_MAX_VEICULO; aux++){//verificando se esta repetindo
				if(!strcmp(c.veiculos[aux], v.modelo)){
					printf(ERRO_VEICULO_REPETIDO, id_corredor, id_veiculo);
					return;
				}
				if(c.veiculos[aux][0] == '\0')
					break;	
			}

			for(int aux=0; aux <QTD_MAX_VEICULO; aux++){
				if(c.veiculos[aux][0] == '\0'){
					strcpy(c.veiculos[aux], v.modelo);
					break;
				}
			}
			c.saldo = c.saldo - v.preco;
			inverted_list_insert(v.modelo, id_corredor, &corredor_veiculos_idx);
			escrever_registro_corredor(c, aux_c);
		}
	}
	printf(SUCESSO);
}

void cadastrar_veiculo_menu(char *marca, char *modelo, char *poder, int velocidade, int aceleracao, int peso, double preco) {
	
		char reg[TAM_ID_VEICULO] = "\0";
		Veiculo v;
		char veiculos_str[TAM_CHAVE_VEICULOS_IDX + 1];
		char veiculospreco_str[TAM_CHAVE_PRECO_VEICULO_IDX + 1];
		strncpy(veiculos_str, "\0", 1);

		strcpy(v.marca, marca);
		strcpy(v.modelo, modelo);
		strcpy(v.poder, poder);

		v.velocidade = velocidade;
		v.aceleracao = aceleracao;
		v.peso = peso;
		v.preco = preco;
		
		sprintf(reg,"%07d", qtd_registros_veiculos);
		strcpy(v.id_veiculo, reg);

		escrever_registro_veiculo(v, qtd_registros_veiculos);//adicionando nos arquivos
		sprintf(veiculos_str, "%07d%04d", qtd_registros_veiculos, qtd_registros_veiculos);
        btree_insert(veiculos_str, &veiculos_idx);
		sprintf(veiculospreco_str, "%013.2lf%07d", v.preco, qtd_registros_veiculos);
		btree_insert(veiculospreco_str, &preco_veiculo_idx);
		qtd_registros_veiculos++;
		
		printf(SUCESSO);
	
}

void cadastrar_pista_menu(char *nome, int dificuldade, int distancia, int recorde) {
	
	char reg[TAM_ID_PISTA]= "\0";
	Pista p;
	char pistas_str[TAM_CHAVE_PISTAS_IDX + 1];
	char nome_pista_str[TAM_CHAVE_NOME_PISTA_IDX + 1];
	char aux[TAM_MAX_NOME_PISTA];

	strcpy(aux, nome);
	strupr(aux);
	strpadright(aux, '#', 33);
	
	if(btree_search(NULL, false, aux, nome_pista_idx.rrn_raiz, &nome_pista_idx)){
		printf(ERRO_PK_REPETIDA, nome);
		return;
	}

	strcpy(p.nome, nome);
	
	p.dificuldade = dificuldade;
	p.distancia = distancia;
	p.recorde = recorde;
	p.taxa_inscricao = TX_FIXA*p.dificuldade;
	sprintf(reg,"%08d", qtd_registros_pistas);
	strcpy(p.id_pista, reg);
	
	sprintf(pistas_str, "%s%04d", p.id_pista, qtd_registros_pistas);
	btree_insert(pistas_str, &pistas_idx);
	sprintf(nome_pista_str, "%s%08d", aux, qtd_registros_pistas);
	btree_insert(nome_pista_str, &nome_pista_idx);

	escrever_registro_pista(p, qtd_registros_pistas);//adicionando nos arquivos

	qtd_registros_pistas++;

	printf(SUCESSO);
}

void executar_corrida_menu(char *id_pista, char *ocorrencia, char *id_corredores, char *id_veiculos) {
	
	char corredor_run[qtd_registros_corredores+1][TAM_ID_CORREDOR];
	char veiculo_run[qtd_registros_corredores+1][TAM_ID_VEICULO], id_corre[qtd_registros_corredores+1][TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX];
	char erro[30];
	int result =0, p=0, aux_pri=0, n = 0, cont=0, aux_v;

	Pista P;
	char result_p[TAM_ID_PISTA];
	char rrn[TAM_RRN_REGISTRO+1];
	int p_rrn;
	Veiculo V;
	char result_v[TAM_CHAVE_VEICULOS_IDX+1];
	char aux_corrida[TAM_CHAVE_CORRIDAS_IDX+1];


    
	if(!btree_search(result_p, false, id_pista, pistas_idx.rrn_raiz, &pistas_idx)){//se pista existe
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}
	strncpy(rrn, result_p+TAM_ID_PISTA-1, TAM_RRN_REGISTRO);
	strncpy(rrn+TAM_RRN_REGISTRO-1, "\0", 1);
	p_rrn = atoi(rrn);
	P = recuperar_registro_pista(p_rrn);
	
	double Premio = 6 * (TX_FIXA * P.dificuldade);
	sprintf(aux_corrida, "%s%s", ocorrencia, id_pista);
	if(btree_search(NULL, false, aux_corrida, corridas_idx.rrn_raiz, &corridas_idx)){//se ocorrencia ja existe
		strcpy(erro, ocorrencia);
		strcat(erro, id_pista);
		printf(ERRO_PK_REPETIDA, erro);
		return;
	}
	

	for(int aux=0, cont = 0; cont < 6;cont ++,  aux += TAM_ID_CORREDOR-1){//id_corredores em um vetor
		strncpy(corredor_run[cont], &id_corredores[aux], TAM_ID_CORREDOR-1);
		corredor_run[cont][TAM_ID_CORREDOR-1] =  '\0';

		if(!btree_search(NULL, false, corredor_run[cont], corredores_idx.rrn_raiz, &corredores_idx)){//se corredor existe
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);
			return;
		}
	}
	
	for(int aux=0, cont = 0; cont < 6;cont ++, aux += TAM_ID_VEICULO-1){//id_veiculos em um vetor
		strncpy(veiculo_run[cont], &id_veiculos[aux], TAM_ID_VEICULO-1);
		veiculo_run[cont][TAM_ID_VEICULO-1] = '\0';
		
		if(!btree_search(NULL, false, veiculo_run[cont], veiculos_idx.rrn_raiz, &veiculos_idx)){//se veiculo existe
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);
			return;
		}
	}
	for(int aux=0, a = 6; aux < a; aux ++){
		btree_search(result_v, false, veiculo_run[aux], veiculos_idx.rrn_raiz, &veiculos_idx);
		aux_v = atoi(result_v + TAM_ID_VEICULO-1);
		V = recuperar_registro_veiculo(aux_v);
		p=0;

		char sec[TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+1];
		strcpy(sec, V.modelo);
		strupr(sec);
		strpadright(sec, '#', TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);
		if(!inverted_list_secondary_search(&result, false, sec, &corredor_veiculos_idx)){
			printf(ERRO_CORREDOR_VEICULO, corredor_run[aux], veiculo_run[aux]);
			n = 1;
		}else{
			cont = inverted_list_primary_search(id_corre, false, result, &aux_pri, &corredor_veiculos_idx);
			for(int i = 0; i < cont; i++){
				if(!strncmp(id_corre[i], corredor_run[aux], TAM_ID_CORREDOR-1)){
					p = 1;
					break;
				}
			}
			
			if(p == 0){
				n = 1;
				printf(ERRO_CORREDOR_VEICULO, corredor_run[aux], veiculo_run[aux]);
			}
		}
	}
	
	if(n == 1)
		return;

	Corrida run;//adicionando corrida
	strcpy(run.id_corredores, id_corredores);
	strcpy(run.id_pista, id_pista);
	strcpy(run.id_veiculos, id_veiculos);
	strcpy(run.ocorrencia, ocorrencia);
	escrever_registro_corrida(run, qtd_registros_corridas);
	sprintf(aux_corrida, "%s%s%04d", ocorrencia, id_pista, qtd_registros_corridas);
	btree_insert(aux_corrida, &corridas_idx);
	qtd_registros_corridas++;

	adicionar_saldo(corredor_run[0], Premio*0.4, false);
	adicionar_saldo(corredor_run[1], Premio*0.3, false);
	adicionar_saldo(corredor_run[2], Premio*0.2, false);

	printf(SUCESSO);
}

/* Busca */
void buscar_corredor_id_menu(char *id_corredor) {
	char result[TAM_CHAVE_CORREDORES_IDX + 1];
    printf(RRN_NOS);
    bool found = btree_search(result, true, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);
    printf(" \n");
    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else
        exibir_corredor(atoi(result+TAM_ID_CORREDOR-1));
}

void buscar_pista_id_menu(char *id_pista) {
	
	char result[TAM_CHAVE_PISTAS_IDX + 1];
    printf(RRN_NOS);
    bool found = btree_search(result, true, id_pista, pistas_idx.rrn_raiz, &pistas_idx);
    printf("\n");
    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else
        exibir_pista(atoi(result+TAM_ID_PISTA-1));
}

void buscar_pista_nome_menu(char *nome_pista) {
	
	char result[TAM_CHAVE_NOME_PISTA_IDX + 1];
	char aux[TAM_MAX_NOME_PISTA];
	char id_p[TAM_ID_PISTA+1];


	strcpy(aux, nome_pista);
	strupr(aux);
	strpadright(aux, '#', 33);

    printf(RRN_NOS);
    bool found = btree_search(result, true, aux, nome_pista_idx.rrn_raiz, &nome_pista_idx);
	strncpy(id_p, result+TAM_MAX_NOME_PISTA-1, TAM_ID_PISTA);

    printf("\n");
    if (!found)
        printf(ERRO_REGISTRO_NAO_ENCONTRADO);
    else
        buscar_pista_id_menu(id_p);
}

/* Listagem */
void listar_corredores_id_menu() {
	if (qtd_registros_corredores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	else
		btree_print_in_order(NULL, NULL, exibir_btree_corredores, corredores_idx.rrn_raiz, &corredores_idx);
}

void listar_corredores_modelo_menu(char *modelo) {
	
	int result=0, aux=0, chaves, aux_v;
	char corredores_mod[qtd_registros_corredores+1][TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX];
	char sec[TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+1];

	char result_v[TAM_CHAVE_VEICULOS_IDX+1];
	strcpy(sec, modelo);
	strupr(sec);
	strpadright(sec, '#', TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);

	if(!inverted_list_secondary_search(&result, false, sec, &corredor_veiculos_idx)){
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
		return;
	}
	chaves = inverted_list_primary_search(corredores_mod, true, result, &aux, &corredor_veiculos_idx);

	
	qsort(corredores_mod, chaves, TAM_ID_CORREDOR-1, order_corredores_idx);

	for(int aux=0; aux<chaves; aux++){
		btree_search(result_v, false, corredores_mod[aux], veiculos_idx.rrn_raiz, &veiculos_idx);
		aux_v = atoi(result_v + TAM_ID_VEICULO-1);
		exibir_corredor(aux_v);	
	}
}

void listar_veiculos_compra_menu(char *id_corredor) {
	
	Corredor c;

	int aux_c;
	char result_c[TAM_ID_CORREDOR+1];
	char saldo[TAM_FLOAT_NUMBER+1];
	char aux[TAM_FLOAT_NUMBER+1];
	
	
	btree_search(result_c, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);
	aux_c = atoi(result_c + TAM_ID_CORREDOR-1);
	c = recuperar_registro_corredor(aux_c);
	sprintf(saldo, "%013.2lf", c.saldo);
	sprintf(aux, "0000000000.00");
	
	if (!btree_print_in_order(aux, saldo, exibir_btree_preco_veiculo, preco_veiculo_idx.rrn_raiz, &preco_veiculo_idx))
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
}

void listar_corridas_periodo_menu(char *data_inicio, char *data_fim) {
	
	if(qtd_registros_corridas == 0){//verificando se tem corrida no sistema
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
		return;
	}

	if(!btree_print_in_order(data_inicio, data_fim, exibir_btree_corridas, corridas_idx.rrn_raiz, &corridas_idx)){
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	}
}

/* Liberar espaço */
void liberar_espaco_menu() {
	
	char CORRE[TAM_ARQUIVO_CORREDORES], aux[TAM_ARQUIVO_CORREDORES];
	int i = 0;
	char result[TAM_CHAVE_CORREDORES_IDX+1], id_corredor[TAM_ID_CORREDOR+1];
	char corredor_str[TAM_CHAVE_CORREDORES_IDX+2];

	for(int aux1=0; aux1 < qtd_registros_corredores; aux1++){//atualizando arquivo
		if (strncmp(&ARQUIVO_CORREDORES[aux1 * TAM_REGISTRO_CORREDOR] , "*|", 2)){//selecionando os arquivos q nao foram removidos
			if(i == 0){//primeiro elemento do arquivo
				strncpy(CORRE, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_REGISTRO_CORREDOR);
			}else{ //vai concatenar no arquivo
				strncpy(aux, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_REGISTRO_CORREDOR);
				strcat(CORRE, aux);
			}
			strncpy(id_corredor, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_ID_CORREDOR);
			btree_search(result, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);
		
			btree_delete(result, &corredores_idx);
			i++;
		}
	}

	for(int aux1=0; aux1<corredores_idx.qtd_nos; aux1++){
		btree_node no_aux = btree_read(aux1, &corredores_idx);
		btree_node_free(no_aux);
	}
	for(int a=0; a<TAM_ARQUIVO_CORREDORES_IDX; a++)//strpadright nao funcionou nesse caso
		ARQUIVO_CORREDORES_IDX[a] = '\0';
	
	strcpy(ARQUIVO_CORREDORES, CORRE);
	corredores_idx.qtd_nos = 0;
	corredores_idx.rrn_raiz = -1;
	qtd_registros_corredores = i;//atualizando numero de corredores

	for(int a=0; a<qtd_registros_corredores; a++){
		strncpy(id_corredor, ARQUIVO_CORREDORES + a*TAM_REGISTRO_CORREDOR, TAM_ID_CORREDOR-1);
		strncpy(id_corredor +TAM_ID_CORREDOR-1, "\0", 1);
		sprintf(corredor_str, "%s%04d", id_corredor, a);
        btree_insert(corredor_str, &corredores_idx);
	}
	
	printf(SUCESSO);
}

/* Imprimir arquivos de dados */
void imprimir_arquivo_corredores_menu() {
	if (qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDORES);
}

void imprimir_arquivo_veiculos_menu() {
	if (qtd_registros_veiculos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_VEICULOS);
}

void imprimir_arquivo_pistas_menu() {
	if (qtd_registros_pistas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PISTAS);
}

void imprimir_arquivo_corridas_menu() {
	if (qtd_registros_corridas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORRIDAS);
}

/* Imprimir índices primários */
void imprimir_corredores_idx_menu() {
	if (corredores_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDORES_IDX);
}

void imprimir_veiculos_idx_menu() {
	
	if (veiculos_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_VEICULOS_IDX);
}

void imprimir_pistas_idx_menu() {
	
	if (pistas_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PISTAS_IDX);
}

void imprimir_corridas_idx_menu() {
	
	if (corridas_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORRIDAS_IDX);
}

/* Imprimir índices secundários */
void imprimir_nome_pista_idx_menu() {
	
	if (nome_pista_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_NOME_PISTA_IDX);
}

void imprimir_preco_veiculo_idx_menu() {
	
	if (preco_veiculo_idx.qtd_nos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_PRECO_VEICULO_IDX);
}

void imprimir_corredor_veiculos_secundario_idx_menu() {
	
	if (corredor_veiculos_idx.qtd_registros_secundario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDOR_VEICULO_SECUNDARIO_IDX);
}

void imprimir_corredor_veiculos_primario_idx_menu(){
	
	if (corredor_veiculos_idx.qtd_registros_primario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		printf("%s\n", ARQUIVO_CORREDOR_VEICULO_PRIMARIO_IDX);
}

/* Funções de manipulação de Lista Invertida */
/**
 * Responsável por inserir duas chaves (chave_secundaria e chave_primaria) em uma Lista Invertida (t).<br />
 * Atualiza os parâmetros dos índices primário e secundário conforme necessário.<br />
 * As chaves a serem inseridas devem estar no formato correto e com tamanho t->tam_chave_primario e t->tam_chave_secundario.<br />
 * O funcionamento deve ser genérico para qualquer Lista Invertida, adaptando-se para os diferentes parâmetros presentes em seus structs.<br />
 *
 * @param chave_secundaria Chave a ser buscada (caso exista) ou inserida (caso não exista) no registro secundário da Lista Invertida.
 * @param chave_primaria Chave a ser inserida no registro primário da Lista Invertida.
 * @param t Ponteiro para a Lista Invertida na qual serão inseridas as chaves.
 */
void inverted_list_insert(char *chave_secundaria, char *chave_primaria, inverted_list *t) {
	
	int result = 0, aux_pri=0;
	char aux[TAM_RRN_REGISTRO+1];
	char sec[TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+1];
	strcpy(sec, chave_secundaria);
	strupr(sec);
	strpadright(sec, '#', TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);
	

	if(!inverted_list_secondary_search(&result, false, sec, t)){//inserindo caso nao tenha o modelo no indice secundario
		
		strncpy(t->arquivo_secundario + ((TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_secundario), sec, TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);
		strcpy(aux, "-001");
	
		strncpy(t->arquivo_primario+((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_primario), chave_primaria, TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX);
		strncpy(t->arquivo_primario+((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_primario)+TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX, aux, TAM_RRN_REGISTRO);

		sprintf(aux, "%04d", t->qtd_registros_primario);
		strncpy(t->arquivo_secundario + ((TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_secundario)+TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX, aux, TAM_RRN_REGISTRO);

		t->qtd_registros_primario++;
		t->qtd_registros_secundario++;
		qsort(t->arquivo_secundario, t->qtd_registros_secundario, TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+TAM_RRN_REGISTRO, t->compar);
	} else {
		inverted_list_primary_search(NULL, false, result, &aux_pri, t);
		strcpy(aux, "-001");
		strncpy(t->arquivo_primario+((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_primario), chave_primaria, TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX);
		strncpy(t->arquivo_primario+((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*t->qtd_registros_primario)+TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX, aux, TAM_RRN_REGISTRO);
		
		sprintf(aux, "%04d", t->qtd_registros_primario);
		strncpy(t->arquivo_primario+((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*aux_pri)+TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX, aux, TAM_RRN_REGISTRO);
		t->qtd_registros_primario++;
	}
}

/**
 * Responsável por buscar uma chave no índice secundário de uma Lista invertida (T). O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e conterá o índice inicial das chaves no registro primário.<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. A chave encontrada deverá ser retornada e o caminho não deve ser informado.<br />
 * ...<br />
 * int result;<br />
 * bool found = inverted_list_secondary_search(&result, false, modelo, &corredor_veiculos_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse na chave encontrada, apenas se ela existe, e o caminho não deve ser informado.<br />
 * ...<br />
 * bool found = inverted_list_secondary_search(NULL, false, modelo, &corredor_veiculos_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 3. Há interesse no caminho feito para encontrar a chave.<br />
 * ...<br />
 * int result;<br />
 * bool found = inverted_list_secondary_search(&result, true, modelo, &corredor_veiculos_idx);<br />
 * </code>
 *
 * @param result Ponteiro para ser escrito o índice inicial (primeira ocorrência) das chaves do registro primário. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave_secundaria Chave a ser buscada.
 * @param t Ponteiro para o índice do tipo Lista invertida no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool inverted_list_secondary_search(int *result, bool exibir_caminho, char *chave_secundaria, inverted_list *t) {
    
	char i[TAM_RRN_REGISTRO+1];
	int aux;
	
	if(!inverted_list_binary_search(&aux, exibir_caminho, chave_secundaria, t))
		return false;
	else{
		strncpy(i, t->arquivo_secundario+TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+ aux*(TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+TAM_RRN_REGISTRO), TAM_RRN_REGISTRO);
		strncpy(i+TAM_RRN_REGISTRO, "\0", 1);
		if(result)
			*result = atoi(i);
		return true;
	}
}

/**
 * Responsável por percorrer o índice primário de uma Lista invertida (T). O valor de retorno indica a quantidade de chaves encontradas.
 * O ponteiro para o vetor de strings result pode ser fornecido opcionalmente, e será populado com a lista de todas as chaves encontradas.
 * O ponteiro para o inteiro indice_final também pode ser fornecido opcionalmente, e deve conter o índice do último campo da lista encadeada
 * da chave primaria fornecida (isso é útil na inserção de um novo registro).<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. As chaves encontradas deverão ser retornadas e tanto o caminho quanto o indice_final não devem ser informados.<br />
 * ...<br />
 * char chaves[TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX][MAX_REGISTROS];<br />
 * int qtd = inverted_list_primary_search(chaves, false, indice, NULL, &corredor_veiculos_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse nas chaves encontradas, apenas no indice_final, e o caminho não deve ser informado.<br />
 * ...<br />
 * int indice_final;
 * int qtd = inverted_list_primary_search(NULL, false, indice, &indice_final, &ccorredor_veiculos_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 3. Há interesse nas chaves encontradas e no caminho feito.<br />
 * ...<br />
 * char chaves[TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX][MAX_REGISTROS];<br />
 * int qtd = inverted_list_primary_search(chaves, true, indice, NULL, &corredor_veiculos_idx);<br />
 * ...<br />
 * <br />
 * </code>
 *
 * @param result Ponteiro para serem escritas as chaves encontradas. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param indice Índice do primeiro registro da lista encadeada a ser procurado.
 * @param indice_final Ponteiro para ser escrito o índice do último registro encontrado (cujo campo indice é -1). É ignorado caso NULL.
 * @param t Ponteiro para o índice do tipo Lista invertida no qual será buscada a chave.
 * @return Indica a quantidade de chaves encontradas.
 */
int inverted_list_primary_search(char result[][TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX], bool exibir_caminho, int indice, int *indice_final, inverted_list *t) {

	int chaves_encontradas = 0;
	char indice_aux[TAM_RRN_REGISTRO];
	if(exibir_caminho){
		printf(REGS_PERCORRIDOS);
		printf(" %d", indice);
	}

	if(result != NULL)
		strncpy(result[chaves_encontradas], t->arquivo_primario + ((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*indice), TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX);
	
	chaves_encontradas++;

	strncpy(indice_aux, t->arquivo_primario + ((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*indice)+TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX, TAM_RRN_REGISTRO);
	if(indice_final)
		*indice_final = indice;
	
	indice = atoi(indice_aux);
	while(indice > -1){
		if(exibir_caminho)
			printf(" %d", indice);
		
		if(result != NULL)
			strncpy(result[chaves_encontradas], t->arquivo_primario + ((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*indice), TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX);
	
		strncpy(indice_aux, t->arquivo_primario + ((TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX+TAM_RRN_REGISTRO)*indice)+TAM_CHAVE_CORREDOR_VEICULO_PRIMARIO_IDX, TAM_RRN_REGISTRO);
		if(indice_final)
			*indice_final = indice;
		chaves_encontradas++;
		indice = atoi(indice_aux);
	}

	if(exibir_caminho)
			printf("\n");
	
	return chaves_encontradas;
}

/**
 * Responsável por buscar uma chave (k) dentre os registros secundários de uma Lista Invertida de forma eficiente.<br />
 * O valor de retorno deve indicar se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e conterá o índice no registro secundário da chave encontrada.<br />
 *
 * @param result Ponteiro para ser escrito o índice nos registros secundários da chave encontrada. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Lista Invertida.
 * @param t Ponteiro para o índice da Lista Invertida no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool inverted_list_binary_search(int* result, bool exibir_caminho, char *chave, inverted_list *t) {
    
		int left = 0, right = t->qtd_registros_secundario-1, mid, result_aux, found = 0;//left representa o inicio, right o ultimo elemento e o mid o elemento central
		char aux_arq[TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+1];
	if(exibir_caminho)
		printf(REGS_PERCORRIDOS);

	while(left <= right){
		if((left+right) % 2 != 0)
			mid = (left+right)/2 + 1;
		else
			mid = (left+right)/2;
        
		strncpy(aux_arq, t->arquivo_secundario + mid * (TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX+TAM_RRN_REGISTRO), TAM_CHAVE_CORREDOR_VEICULO_SECUNDARIO_IDX);
		result_aux = t->compar(chave, aux_arq);//result_auxado da comparacao entre a key e o elemento na posicao mid
		
		if(result_aux == 0){
			left = mid+1;
			right = mid-1;
			found = 1;
			if(exibir_caminho)
				printf (" %d", mid);
		} else if (result_aux > 0){
			left = mid+1;
			if(exibir_caminho)
				printf (" %d", mid);
		} else {
			right = mid-1;
			if(exibir_caminho)
				printf (" %d", mid);
		}
	}
	if(result)
		*result = mid;
	if(exibir_caminho)
		printf ("\n");
	if(found ==1)
		return true;

	return false;
}

/* Funções de manipulação de Árvores-B */
/**
 * Responsável por inserir uma chave (k) em uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * A chave a ser inserida deve estar no formato correto e com tamanho t->tam_chave.<br />
 * O funcionamento deve ser genérico para qualquer Árvore-B, considerando que os únicos parâmetros que se alteram entre
 * as árvores é o t->tam_chave.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * ...<br />
 * char corredor_str[TAM_CHAVE_CORREDORES_IDX + 1];<br />
 * sprintf(corredor_str, "%s%04d", id_corredor, rrn_corredor);<br />
 * btree_insert(corredor_str, &corredores_idx);<br />
 * ...<br />
 * </code>
 *
 * @param chave Chave a ser inserida na Árvore-B.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 */
void btree_insert(char *chave, btree *t) {//funcao baseado do pseudocodigo fornecido
    
	if(t->rrn_raiz < 0){//arvore vazia
		btree_node no = btree_node_malloc(t);
		no.this_rrn = t->qtd_nos;
		no.qtd_chaves = 1;
		strcpy(no.chaves[0], chave);
		no.folha = true;

		t->rrn_raiz = t->qtd_nos;
		t->qtd_nos = 1;
		
		btree_write(no, t);
		
	} else {
		promovido_aux p = btree_insert_aux(chave, t->rrn_raiz, t);
		if(p.chave_promovida[0] != '\0'){
			btree_node no = btree_node_malloc(t);
			no.this_rrn = t->qtd_nos;
			no.qtd_chaves = 1;
			strncpy(no.chaves[0], p.chave_promovida, t->tam_chave);
			no.folha = false;
			no.filhos[0] = t->rrn_raiz;
			no.filhos[1] = p.filho_direito;

			t->qtd_nos++;
			t->rrn_raiz = no.this_rrn;

			btree_write(no, t);
			
		}
	}
}

/**
 * Função auxiliar de inserção de uma chave (k) em uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * Esta é uma função recursiva. Ela recebe o RRN do nó a ser trabalhado sobre.<br />
 *
 * @param chave Chave a ser inserida na Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Retorna uma struct do tipo promovido_aux que contém a chave promovida e o RRN do filho direito.
 */
promovido_aux btree_insert_aux(char *chave, int rrn, btree *t) {
	promovido_aux p;
    
	
	int aux;
    strcpy(p.chave_promovida, chave);
    p.filho_direito = -1;

    if (rrn >= 0) {

        btree_node no = btree_read(rrn, t);
        if (!btree_binary_search(&aux, false, chave, &no, t)){
            p = btree_insert_aux(chave, no.filhos[aux], t);
            if (p.chave_promovida[0] != '\0') {
                if (no.qtd_chaves < btree_order - 1){ //tem lugar no no
					for (int a = no.qtd_chaves - 1; a >= aux; a--){
						strncpy(no.chaves[a+1], no.chaves[a], t->tam_chave);
						no.filhos[a+2] = no.filhos[a+1];
					}

					strncpy(no.chaves[aux], p.chave_promovida, t->tam_chave);
					no.filhos[aux + 1] = p.filho_direito;	
                    no.qtd_chaves ++;
                    btree_write(no, t);
					
                    p.chave_promovida[0] = '\0';//mostrar q nao deu overflow
				} else //no cheio		
                    p = btree_divide(p, &no, aux, t);
            }
        }
    }

    return p;
}


/**
 * Função auxiliar para dividir um nó de uma Árvore-B (T). Atualiza os parâmetros conforme necessário.<br />
 *
 * @param promo Uma struct do tipo promovido_aux que contém a chave a ser inserida e o RRN do seu filho direito.
 * @param node Ponteiro para nó que deve ser dividido. 
 * @param i O índice da posição onde a chave a ser inserida deve estar.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Retorna uma struct do tipo promovido_aux que contém a chave promovida e o RRN do filho direito.
 */
promovido_aux btree_divide(promovido_aux promo, btree_node *node, int i, btree *t) {
	promovido_aux p;
    
	
	strcpy(p.chave_promovida, promo.chave_promovida);
	p.filho_direito = promo.filho_direito;
    btree_node no = btree_node_malloc(t);
    no.this_rrn = t->qtd_nos;
    no.qtd_chaves = 0;
    no.folha = node->folha;
    int min = (btree_order - 1) / 2; 
    t->qtd_nos ++;
 
    
    if (i == btree_order / 2) {
		strcpy(no.chaves[0], node->chaves[node->qtd_chaves - 1]);
		no.filhos[0] = p.filho_direito;
		no.filhos[1] = node->filhos[node->qtd_chaves];
		node->filhos[node->qtd_chaves] --;
		p.filho_direito = -1;
		
		no.qtd_chaves ++;        
		node->qtd_chaves --;
		
		for (int a = node->qtd_chaves - 1; a >= i; a--) {
			strncpy(node->chaves[a + 1], node->chaves[a], t->tam_chave);
			node->filhos[a + 2] = node->filhos[a + 1];
		}

		strncpy(node->chaves[i], p.chave_promovida, t->tam_chave);
		node->filhos[i + 1] = p.filho_direito;
		p.filho_direito = no.this_rrn;

    	btree_write(*node, t);
    	btree_write(no, t);
		btree_node_free(no);

    	return p;
    }else if (i > btree_order / 2) {

		strcpy(no.chaves[0], p.chave_promovida);
        no.filhos[0] = node->filhos[node->qtd_chaves];
        no.filhos[1] = p.filho_direito;
        no.qtd_chaves += 1;
        node->filhos[node->qtd_chaves] = -1;
    } else {
		strcpy(no.chaves[0], node->chaves[node->qtd_chaves - 1]);
		no.filhos[0] = node->filhos[node->qtd_chaves - 1];
		no.filhos[1] = node->filhos[node->qtd_chaves];

		node->filhos[node->qtd_chaves - 1] = -1;            
		node->filhos[node->qtd_chaves] = -1;
		
		no.qtd_chaves ++;        
		node->qtd_chaves --;

		for (int a = node->qtd_chaves - 1; a >= i; a--) {
			strncpy(node->chaves[a + 1], node->chaves[a], t->tam_chave);
			node->filhos[a + 2] = node->filhos[a + 1];
		}

		strncpy(node->chaves[i], p.chave_promovida, t->tam_chave);
		node->filhos[i + 1] = p.filho_direito;
		strcpy(p.chave_promovida, node->chaves[node->qtd_chaves]);
		p.filho_direito = no.this_rrn;

		btree_write(*node, t);
		btree_write(no, t);
		

		return p;
	}
        
    for (int b = no.qtd_chaves,  a = min; b < min; b++, a++) {
        strcpy(no.chaves[b], node->chaves[a]);
        no.filhos[b + 1] = node->filhos[a + 1];
        node->filhos[a + 1] = -1;
        node->qtd_chaves --;
        no.qtd_chaves ++;
    }
 
    strcpy(p.chave_promovida, node->chaves[node->qtd_chaves - 1]);
    node->qtd_chaves --;
    p.filho_direito = no.this_rrn;

    btree_write(*node, t);
    btree_write(no, t);
	

    return p;
}


/**
 * Responsável por remover uma chave (k) de uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * A chave a ser removida deve estar no formato correto e com tamanho t->tam_chave.<br />
 * O funcionamento deve ser genérico para qualquer Árvore-B, considerando que os únicos parâmetros que se alteram entre
 * as árvores é o t->tam_chave.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * ...<br />
 * char corredor_str[TAM_CHAVE_USUARIOS_IDX + 1];<br />
 * sprintf(corredor_str, "%s%04d", id_corredor, rrn_corredor);<br />
 * btree_delete(corredor_str, &corredores_idx);<br />
 * ...<br />
 * </code>
 *
 * @param chave Chave a ser removida da Árvore-B.
 * @param t Ponteiro para o índice do tipo Árvore-B do qual será removida a chave.
 */
void btree_delete(char *chave, btree *t) {
	
    if (btree_delete_aux(chave, t->rrn_raiz, t))//inicio da recursao com o no raiz
        t->rrn_raiz = 0;
}

/**
 * Função auxiliar de remoção de uma chave (k) de uma Árvore-B (T). Atualiza os parâmetros da Árvore-B conforme necessário.<br />
 * Esta é uma função recursiva. Ela recebe o RRN do nó a ser trabalhado sobre.<br />
 *
 * @param chave Chave a ser removida da Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B do qual será removida a chave.
 * @return Indica se a remoção deixou o nó que foi processado com menos chaves que o mínimo necessário.
 */
bool btree_delete_aux(char *chave, int rrn, btree *t) {
	
	int min = (btree_order-1)/2;
	int aux = 0;

	if(rrn >= 0){//verificando se o rrn é valido
		btree_node no = btree_read(rrn, t);
		if(btree_binary_search(&aux, false, chave, &no, t)){
		
			if(no.folha == false){
				btree_node no_aux = btree_read(no.filhos[aux], t);
				strncpy(no.chaves[aux], no_aux.chaves[no_aux.qtd_chaves-1], t->tam_chave);//substitui pelo filho predecessor e chama para remover ele
				btree_write(no, t);
				
				if(btree_delete_aux(no.chaves[aux], no.filhos[aux], t)){
					return btree_borrow_or_merge(&no, aux, t);
				}
			}else {//no folha
				if(aux < no.qtd_chaves -1){//nao é o ultimo elemento
					for(int a = aux; a<no.qtd_chaves-1; a++ ){//tirando a chave do no
						strcpy(no.chaves[a], no.chaves[a+1]);
					}
				}
				no.qtd_chaves--;
				
				btree_write(no, t);
				
				if(no.qtd_chaves < min)//verificando se quebrou o no
					return true;
				else 
					return false;
			}
		}else {//chave nao encontrada, buscar pelos filhos
			if(btree_delete_aux(chave, no.filhos[aux], t))
					return btree_borrow_or_merge(&no, aux, t);
		}
	}
    return false;
}

/**
 * Função auxiliar para redistribuir ou concatenar nós irmãos adjacentes à esquerda e à direita de um nó pai em uma Árvore-B (T). 
 * Atualiza os parâmetros conforme necessário.<br />
 *
 * @param node Ponteiro para nó pai dos nós irmãos adjacentes que deve ser redistribuidos ou concatenados. 
 * @param i O índice da posição no nó pai onde se encontra a chave separadora dos nós irmãos adjacentes.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual serão redistribuídos ou concatenados os nós irmãos adjacentes.
 * @return Indica se a redistribuição ou concatenação deixou o nó pai com menos chaves que o mínimo necessário.
 */
bool btree_borrow_or_merge(btree_node *node, int i, btree *t) {//discutido com Vinicius
    
	int min = (btree_order-1)/2;//min de descendentes em um nó pelo calculo m/2(teto) - 1
	//preferencia para emprestar/concatenar irmao a direita
	btree_node no_central = btree_read(node->filhos[i], t);

	if(i + 1 < btree_order){//verificando se é possivel ter irmao a direita 
		if(node->filhos[i+1] != -1){//se o irmao existe
			btree_node no_direita = btree_read(node->filhos[i+1], t);
			if(no_direita.qtd_chaves > min){//verificando se o irmao tem para emprestar
				
				strcpy(no_central.chaves[no_central.qtd_chaves], node->chaves[i]);//passa do pai para ultimo posicao do filho esquerda
				no_central.qtd_chaves++;
				btree_write(no_central, t);
				
				strcpy(node->chaves[i], no_direita.chaves[0]);//passa do filho a direita para o pai
				btree_write(*node, t);

				for(int aux = 0; aux < no_direita.qtd_chaves-1; aux++)//arrumando no_direita depois da remoção
					strcpy(no_direita.chaves[aux], no_direita.chaves[aux+1]);
				no_direita.qtd_chaves--;
				btree_write(no_direita, t);
				
				return false;//nao alterou em nada a qtd do pai
			}
		}
	}

	if(i - 1 >= 0){//verificando se é possivel ter irmao a esquerda 
		if(node->filhos[i-1] != -1){
			btree_node no_esquerda = btree_read(node->filhos[i-1], t);
			if(no_esquerda.qtd_chaves > min){//verificando se o irmao tem para emprestar
				
				for(int aux = no_central.qtd_chaves; aux > 0; aux--)//arrumando no_central antes de receber
					strcpy(no_central.chaves[aux], no_central.chaves[aux-1]);
				strcpy(no_central.chaves[0], node->chaves[i-1]);
				no_central.qtd_chaves++;
				btree_write(no_central, t);
				
				strcpy(node->chaves[i-1], no_esquerda.chaves[no_esquerda.qtd_chaves-1]);
				btree_write(*node, t);
				
				no_esquerda.qtd_chaves--;
				btree_write(no_esquerda, t);
				
				return false;//nao alterou em nada a qtd do pai
			}
		}
	}

	if(i + 1 < btree_order){//verificando se é possivel ter irmao a direita 
		if(node->filhos[i+1] != -1){
			btree_node no_direita = btree_read(node->filhos[i+1], t);
			if(no_direita.qtd_chaves <= min){
				
				strcpy(no_central.chaves[no_central.qtd_chaves], node->chaves[i]);
				no_central.qtd_chaves ++;
				for(int aux=0; aux< no_direita.qtd_chaves; aux++){//pegando do no direita com seus filhos
					strcpy(no_central.chaves[no_central.qtd_chaves], no_direita.chaves[aux]);
					no_central.filhos[no_central.qtd_chaves] = no_direita.filhos[aux];
					no_central.qtd_chaves ++;
				}
				no_central.filhos[no_central.qtd_chaves] = no_direita.filhos[no_direita.qtd_chaves];
				btree_write(no_central, t);
			
				for(int aux=i; aux<node->qtd_chaves-1; aux++){
					strcpy(node->chaves[aux], node->chaves[aux+1]);
				}
					node->qtd_chaves --;
				if(node->qtd_chaves > 0){
					
					for(int aux=i; aux < node->qtd_chaves; aux++)
						node->filhos[aux]= node->filhos[aux+1];
					node->filhos[node->qtd_chaves+1] = -1;
				}else
					for(int aux=0; aux <= btree_order; aux++)
						node->filhos[aux] = -1;
				btree_write(*node, t);

				no_direita.qtd_chaves =0;
				btree_write(no_direita, t);
				
				if(node->qtd_chaves < min)
					return true;
				else 
					return false;
				
			}
		}
	}

	if(i - 1 >= 0){//verificando se é possivel ter irmao a esquerda 
		if(node->filhos[i-1] != -1){
			btree_node no_esquerda = btree_read(node->filhos[i-1], t);
			if(no_esquerda.qtd_chaves <= min){
				strcpy(no_esquerda.chaves[no_esquerda.qtd_chaves], node->chaves[i-1]);
				no_esquerda.qtd_chaves ++;
				
				for(int aux=0; aux< no_central.qtd_chaves; aux++){
					strcpy(no_esquerda.chaves[no_esquerda.qtd_chaves], no_central.chaves[aux]);
					no_esquerda.filhos[no_esquerda.qtd_chaves] = no_central.filhos[aux];
					no_esquerda.qtd_chaves ++;
				}
				no_esquerda.filhos[no_esquerda.qtd_chaves] = no_central.filhos[no_central.qtd_chaves];
				btree_write(no_esquerda, t);
				
				for(int aux=i; aux<node->qtd_chaves; aux++)
					strcpy(node->chaves[aux], node->chaves[aux+1]);
				node->qtd_chaves --;
				if(node->qtd_chaves > 0){
					for(int aux=i; aux < node->qtd_chaves; aux++)
						node->filhos[aux]= node->filhos[aux+1];
				}else
					for(int aux=0; aux <= btree_order; aux++)
						node->filhos[aux] = -1;
				btree_write(*node, t);

				no_central.qtd_chaves =0;
				btree_write(no_central, t);
				
				if(node->qtd_chaves < min)
					return true;
				else 
					return false;	
			}
		}
	}

    return false;
}

/**
 * Responsável por buscar uma chave (k) em uma Árvore-B (T). O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para a string result pode ser fornecido opcionalmente, e conterá o resultado encontrado.<br />
 * Esta é uma função recursiva. O parâmetro rrn recebe a raíz da Árvore-B na primeira chamada, e nas chamadas
 * subsequentes é o RRN do filho de acordo com o algoritmo fornecido.<br />
 * Comportamento de acordo com as especificações do PDF sobre Árvores-B e suas operações.<br />
 * <br />
 * Exemplos de uso:<br />
 * <code>
 * // Exemplo 1. A chave encontrada deverá ser retornada e o caminho não deve ser informado.<br />
 * ...<br />
 * char result[TAM_CHAVE_CORREDORES_IDX + 1];<br />
 * bool found = btree_search(result, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 2. Não há interesse na chave encontrada, apenas se ela existe, e o caminho não deve ser informado.<br />
 * ...<br />
 * bool found = btree_search(NULL, false, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);<br />
 * ...<br />
 * <br />
 * // Exemplo 3. Busca por uma chave de tamanho variável (específico para o caso de buscas por chaves PIX).<br />
 * ...<br />
 * char modelo_str[TAM_MAX_MODELO];<br />
 * strcpy(nome_pista_str, nome_pista);<br />
 * strpadright(nome_pista_str, '#', TAM_MAX_NOME_PISTA - 1);<br />
 * bool found = btree_search(NULL, false, nome_pista_str, nome_pista_idx.rrn_raiz, &nome_pista_idx);<br />
 * ...<br />
 * <br />
* // Exemplo 4. Há interesse no caminho feito para encontrar a chave.<br />
  * ...<br />
 * char result[TAM_CHAVE_CORREDORES_IDX + 1];<br />
 * printf(RRN_NOS);<br />
 * bool found = btree_search(result, true, id_corredor, corredores_idx.rrn_raiz, &corredores_idx);<br />
 * printf("\n");<br />
 * </code>
 *
 * @param result Ponteiro para ser escrita a chave encontrada. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Árvore-B.
 * @param rrn RRN do nó no qual deverá ser processado. É o RRN da raíz da Árvore-B caso seja a primeira chamada.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool btree_search(char *result, bool exibir_caminho, char *chave, int rrn, btree *t) {
    
	if(exibir_caminho)
		printf(" %d ", rrn);

	if(rrn == -1)
		return false;

	btree_node no_aux = btree_read(rrn, t);
	int aux;
	
	if(btree_binary_search(&aux, exibir_caminho, chave, &no_aux, t)){//verificando se encontrou o elemento no no
		if(result)
			strcpy(result, no_aux.chaves[aux]);
		return true;
	}else if (!no_aux.folha)//continuar a busca caso ele nao seja folha pelo seu filho
		return btree_search(result, exibir_caminho, chave, no_aux.filhos[aux], t);
	return false;
}

/**
 * Responsável por buscar uma chave (k) dentro do nó de uma Árvore-B (T) de forma eficiente. O valor de retorno indica se a chave foi encontrada ou não.
 * O ponteiro para o int result pode ser fornecido opcionalmente, e indica o índice da chave encontrada (caso tenha sido encontrada) 
 * ou o índice do filho onde esta chave deve estar (caso não tenha sido encontrada).<br />
 *
 * @param result Ponteiro para ser escrito o índice da chave encontrada ou do filho onde ela deve estar. É ignorado caso NULL.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso.
 * @param chave Chave a ser buscada na Árvore-B.
 * @param node Ponteiro para o nó onde a busca deve ser feita.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será buscada a chave.
 * @return Indica se a chave foi encontrada.
 */
bool btree_binary_search(int *result, bool exibir_caminho, char* chave, btree_node* node, btree* t) {
	int i = busca_binaria(chave, node->chaves[0], node->qtd_chaves, t->tam_chave+1, t->compar, exibir_caminho, +1);
    if (result != NULL) 
        *result = i;
    return i >= 0 && i < node->qtd_chaves && !t->compar(chave, node->chaves[i]);
}

/**
 * Função para percorrer uma Árvore-B (T) em ordem.<br />
 * Os parâmetros chave_inicio e chave_fim podem ser fornecidos opcionalmente, e contém o intervalo do percurso.
 * Caso chave_inicio e chave_fim sejam NULL, o índice inteiro é percorrido.
 * Esta é uma função recursiva. O parâmetro rrn recebe a raíz da Árvore-B na primeira chamada, e nas chamadas
 * subsequentes é o RRN do filho de acordo com o algoritmo de percursão em ordem.<br />
 * <br />
 * Exemplo de uso:<br />
 * <code>
 * // Exemplo 1. Intervalo não especificado.
 * ...<br />
 * bool imprimiu = btree_print_in_order(NULL, NULL, exibir_btree_corredor, corredores_idx.rrn_raiz, &corredores_idx);
 * ...<br />
 * <br />
 * // Exemplo 2. Imprime as transações contidas no intervalo especificado.
 * ...<br />
 * bool imprimiu = btree_print_in_order(data_inicio, data_fim, exibir_btree_pista, pistas_idx.rrn_raiz, &pistas_idx);
 * ...<br />
 * </code>
 *
 * @param chave_inicio Começo do intervalo. É ignorado caso NULL.
 * @param chave_fim Fim do intervalo. É ignorado caso NULL.
 * @param exibir Função utilizada para imprimir uma chave no índice. É informada a chave para a função.
 * @param rrn RRN do nó no qual deverá ser processado.
 * @param t Ponteiro para o índice do tipo Árvore-B no qual será inserida a chave.
 * @return Indica se alguma chave foi impressa.
 */
bool btree_print_in_order(char *chave_inicio, char *chave_fim, bool (*exibir)(char *chave), int rrn, btree *t) {
	
	if(rrn == -1)
		return false;
	btree_node no = btree_read(rrn, t);
	
	for(int aux = 0; aux < no.qtd_chaves; aux++){

		if(no.filhos[aux] != -1)
			btree_print_in_order(chave_inicio, chave_fim, exibir, no.filhos[aux], t);
		if((chave_inicio == NULL) && (chave_fim == NULL))// && por conta do do listar veiculos compra
			exibir(no.chaves[aux]);
		else {
			if((t->compar(no.chaves[aux], chave_inicio) >= 0) && (t->compar(no.chaves[aux], chave_fim) <= 0))//verificao se esta entre o inicio e fim para exibir	
				exibir(no.chaves[aux]);

		}
	}

	if (no.filhos[no.qtd_chaves] != -1)//verificando ultimo filho, pois se deixa no for o acesso ao no.chaves quebra
            btree_print_in_order(chave_inicio, chave_fim, exibir, no.filhos[no.qtd_chaves], t);
	return true;
}

/**
 * Função interna para ler um nó em uma Árvore-B (T).<br />
 *
 * @param no No a ser lido da Árvore-B.
 * @param t Árvore-B na qual será feita a leitura do nó.
 */
btree_node btree_read(int rrn, btree *t) {//mesma ideia do recuperar
    btree_node no;
    
	no = btree_node_malloc(t);
	no.this_rrn = rrn;
	int Regs = btree_register_size(t) * rrn;
	char no_aux [t->tam_chave +1];
	strncpy(no_aux, "\0", 1);

	strncpy(no_aux, t->arquivo + Regs, t->tam_chave);
	strncpy(no_aux+3, "\0", 1);; //para passar para int
	no.qtd_chaves = atoi(no_aux);
	Regs= Regs+3;

	for(int aux = 0; aux< btree_order -1; aux++, Regs = Regs+t->tam_chave){
		strncpy(no_aux, t->arquivo + Regs, t->tam_chave);
		strncpy(no_aux+t->tam_chave, "\0", 1);
		
		if(strncmp(no_aux, "#", 1) != 0)
			strcpy(no.chaves[aux], no_aux);
		
	}

	strncpy(no_aux, t->arquivo + Regs, 1);
	if(strncmp(no_aux, "F", 1) == 0)
		no.folha = false;
	else 
		no.folha = true;

	Regs++;
	for(int aux = 0; aux< btree_order; aux++, Regs = Regs+3){
		strncpy(no_aux, t->arquivo + Regs, 3);
		strncpy(no_aux+3, "\0", 1);

		if(strncmp(no_aux, "*", 1) != 0)
			no.filhos[aux] = atoi(no_aux);
		else 
			no.filhos[aux] = -1;
	}
	return no;
}

/**
 * Função interna para escrever um nó em uma Árvore-B (T).<br />
 *
 * @param no No a ser escrito na Árvore-B.
 * @param t Árvore-B na qual será feita a escrita do nó.
 */
void btree_write(btree_node no, btree *t) {//mesma ideia do escrever
    
	int qtdNaoUsadas = btree_order - no.qtd_chaves - 1;
	char no_princ[btree_register_size(t) + 1];
	char no_aux[t->tam_chave +1];
	
	strncpy(no_princ, "\0", 1);//colocando como vazio
	sprintf(no_aux, "%03d", no.qtd_chaves);
	strcpy(no_princ, no_aux);
	
	for(int aux =0; aux < no.qtd_chaves; aux++){
		strncat(no_princ, no.chaves[aux], t->tam_chave);
	}
	
	if(qtdNaoUsadas != 0)//tem espaço sobrando no nó
		strpadright(no_princ, '#', strlen(no_princ)+ qtdNaoUsadas * t->tam_chave);//completando com # os espaços vazios

	if(no.folha == true)//verificando se o nó é folha
		strcat(no_princ, "T");
	else
		strcat(no_princ, "F");
	
	for(int aux =0; aux < btree_order; aux++){
		if(no.filhos[aux] != -1){//verificando se tem filho
			sprintf(no_aux, "%03d", no.filhos[aux]);
			strncat(no_princ, no_aux, 3);
		}else
			strcat(no_princ, "***");
	}
	
	strncpy(t->arquivo + no.this_rrn * btree_register_size(t), no_princ, btree_register_size(t));
}

/**
 * Função interna para alocar o espaço necessário dos campos chaves (vetor de strings) e filhos (vetor de inteiros) da struct btree_node.<br />
 *
 * @param t Árvore-B base para o qual será alocado um struct btree_node.
 */
btree_node btree_node_malloc(btree *t) {//comentado com Vinicius
    btree_node no;
    
	//criar bloco dos ponteiros
	char *bloco_chaves=malloc((btree_order-1)*(t->tam_chave+1));

	strpadright(bloco_chaves, '\0', ((btree_order-1)*(t->tam_chave+1)));

	no.chaves = malloc((btree_order - 1));
	for(int aux = 0; aux < btree_order - 1; aux++){
		no.chaves[aux] = bloco_chaves + aux*(t->tam_chave+1);
	}

	no.filhos = malloc(btree_order  * sizeof(int*));
	for(int aux = 0; aux < btree_order; aux++)
		no.filhos[aux] = -1;
	
	no.folha = true;
	no.qtd_chaves=0;
	no.this_rrn=-1;
	
	return no;
}

/**
 * Função interna para calcular o tamanho em bytes de uma Árvore-B.<br />
 *
 * @param t Árvore-B base para o qual será calculado o tamanho.
 */
int btree_register_size(btree *t) {
    int chaves_ordenadas = (btree_order-1)*t->tam_chave;
    return 3 + chaves_ordenadas + 1 + (btree_order*3);
}

/* Funções de busca binária */
/**
 * Função Genérica de busca binária, que aceita parâmetros genéricos (assinatura baseada na função bsearch da biblioteca C).
 *
 * @param key Chave de busca genérica.
 * @param base0 Base onde ocorrerá a busca, por exemplo, um ponteiro para um vetor.
 * @param nmemb Número de elementos na base.
 * @param size Tamanho do tipo do elemento na base, dica: utilize a função sizeof().
 * @param compar Ponteiro para a função que será utilizada nas comparações.
 * @param exibir_caminho Indica se o caminho percorrido deve ser impresso. Se true, imprime as posições avaliadas durante a busca, até que todas sejam visitadas ou o elemento seja encontrado (pela 1a vez).
 *                       Caso o número de elementos seja par (p.ex, 10 elementos), então há 2 (duas) possibilidades para a posição da mediana dos elementos (p.ex., 5a ou 6a posição se o total fosse 10).
 *                       Neste caso, SEMPRE escolha a posição mais à direita (p.ex., a posição 6 caso o total for 10).
 * @param posicao_caso_repetido Caso o elemento seja encontrado, se houver mais do que 1 ocorrência, indica qual deve ser retornada. As opções são:
 *                     -1 = primeira : retorna a PRIMEIRA ocorrência (posição mais à esquerda).
 *                      0 = meio : retorna a ocorrência do MEIO (posição central). [modo padrão]
 *                     +1 = ultima : retorna a ÚLTIMA ocorrência (posição mais à direita).
 * @param retorno_se_nao_encontrado Caso o elemento NÃO seja encontrado, indica qual valor deve ser retornado. As opções são:
 *                     -1 = predecessor : retorna o elemento PREDECESSOR (o que apareceria imediatamente antes do elemento procurado, caso fosse encontrado).
 *                      0 = nulo : retorna NULL. [modo padrão]
 *                     +1 = sucessor : retorna o elemento SUCESSOR (o que apareceria imediatamente depois do elemento procurado, caso fosse encontrado).
 * @return Retorna o elemento encontrado ou NULL se não encontrou.
 * @return Retorna o indice do elemento encontrado ou -1 se não encontrou. 
 */
int busca_binaria_com_reps(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int posicao_caso_repetido, int retorno_se_nao_encontrado) {
    int ret=-1;
	
	int left = 0, right = nmemb-1, mid, result, found = 0;//left representa o inicio, right o ultimo elemento e o mid o elemento central

	if(exibir_caminho)
		printf("(");

	while(left <= right){
		if((left+right) % 2 != 0)
			mid = (left+right)/2 + 1;
		else
			mid = (left+right)/2;
        
		void *aux = (void *)base0 + (mid * size);
		
		result = compar(key, aux);//resultado da comparacao entre a key e o elemento na posicao mid
		if(result == 0){
			left = mid+1;
			right = mid-1;
			found = 1;
			if(exibir_caminho)
				printf (" %d", mid);
		} else if (result > 0){
			left = mid+1;
			if(exibir_caminho)
				printf (" %d", mid);
		} else {
			right = mid-1;
			if(exibir_caminho)
				printf (" %d", mid);
		}
	}
	
	if(exibir_caminho)
		printf (")");
	
	if(found == 1){//elemento foi encontrado
		if((left+right) % 2 != 0)
			mid = (left+right)/2 + 1;
		else
			mid = (left+right)/2; 
		return mid;
	} 
	else {
		if(retorno_se_nao_encontrado == 0)
			return -1;
		else {
			if(result > 0)
				return mid+1;
			else	
				return mid;		
		}	
	}
	return ret;
}

int busca_binaria(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int retorno_se_nao_encontrado) {
	return busca_binaria_com_reps(key, base0, nmemb, size, compar, exibir_caminho, 0, retorno_se_nao_encontrado);
}

char *strpadright(char *str, char pad, unsigned size) {
	for (unsigned i = strlen(str); i < size; ++i)
		str[i] = pad;
	str[size] = '\0';
	return str;
}

char *strupr(char *str) {
	for (char *p = str; *p; ++p)
		*p = toupper(*p);
	return str;
}
char *strlower(char *str) {
	for (char *p = str; *p; ++p)
		*p = tolower(*p);
	return str;
}