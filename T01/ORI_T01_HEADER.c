/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. Almeida
 *
 * Trabalho 01 - Indexação
 *
 * ========================================================================== *
 *   <<< IMPLEMENTE AQUI!!! COPIE E COLE O CONTEÚDO DESTE ARQUIVO NO AVA >>>
 * ========================================================================== */

/* Bibliotecas */
#include "ORI_T01_HEADER.h"

/* ===========================================================================
 * ================================= FUNÇÕES ================================= */


/* <<< COLOQUE AQUI OS DEMAIS PROTÓTIPOS DE FUNÇÕES, SE NECESSÁRIO >>> */


/* Funções auxiliares para o qsort.
 * Com uma pequena alteração, é possível utilizá-las no bsearch, assim, evitando código duplicado.
 * */

/* Função de comparação entre chaves do índice corredores_idx */
int qsort_corredores_idx(const void *a, const void *b) {
    return strcmp( ( (corredores_index *)a )->id_corredor, ( (corredores_index *)b )->id_corredor);
}

/* Função de comparação entre chaves do índice veiculos_idx */
int qsort_veiculos_idx(const void *a, const void *b) {
	
	return strcmp(((veiculos_index *)a) ->id_veiculo, ((veiculos_index *)b) ->id_veiculo);
}

/* Função de comparação entre chaves do índice pistas_idx */
int qsort_pistas_idx(const void *a, const void *b) {
	
	return strcmp(((pistas_index *)a) ->id_pista, ((pistas_index *)b) ->id_pista);
}

/* Função de comparação entre chaves do índice corridas_idx */
int qsort_corridas_idx(const void *a, const void *b) {
	
	int aux = 0;
	aux = qsort_data_idx(a,b);
	if(aux == 0)
		return strcmp(((corridas_index *)a) ->id_pista, ((corridas_index *)b) ->id_pista);
	else
		return aux;
		
}

/* Funções de comparação apenas entre data de ocorrencia do índice corridas_idx */
int qsort_data_idx(const void *a, const void *b){
	
	return strcmp(((corridas_index *)a) ->ocorrencia, ((corridas_index *)b) ->ocorrencia);
}

/* Função de comparação entre chaves do índice nome_pista_idx */
int qsort_nome_pista_idx(const void *a, const void *b){
	
	return strcmp(((nome_pista_index *)a) ->nome, ((nome_pista_index *)b) ->nome);
}

/* Função de comparação entre chaves do índice preco_veiculo_idx */
int qsort_preco_veiculo_idx(const void *a, const void *b){
	
	char reg[20], aux[20];

	sprintf(reg,"%013.2f", ((preco_veiculo_index *)a) ->preco);
	sprintf(aux,"%013.2f", ((preco_veiculo_index *)b) ->preco);
	return strcmp(reg, aux);
}

/* Função de comparação entre chaves do índice secundário de corredor_veiculos_secundario_idx */
int qsort_corredor_veiculos_secundario_idx(const void *a, const void *b){
	
	return strcmp(((corredor_veiculos_secundario_index *)a) ->chave_secundaria, ((corredor_veiculos_secundario_index *)b) ->chave_secundaria);
}

/* Cria o índice respectivo */
void criar_corredores_idx() {
    if (!corredores_idx)
        corredores_idx = malloc(MAX_REGISTROS * sizeof(corredores_index));
 
    if (!corredores_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
 
    for (unsigned i = 0; i < qtd_registros_corredores; ++i) {
        Corredor c = recuperar_registro_corredor(i);
 
        if (strncmp(c.id_corredor, "*|", 2) == 0)
            corredores_idx[i].rrn = -1; // registro excluído
        else
            corredores_idx[i].rrn = i;
 
        strcpy(corredores_idx[i].id_corredor, c.id_corredor);
    }
 
    qsort(corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);
    printf(INDICE_CRIADO, "corredores_idx");
}

void criar_veiculos_idx() {
	
	if (!veiculos_idx)
        veiculos_idx = malloc(MAX_REGISTROS * sizeof(veiculos_index));
 
    if (!veiculos_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
 
    for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);
 		veiculos_idx[i].rrn = i;
        strcpy(veiculos_idx[i].id_veiculo, v.id_veiculo);
    }
 
    qsort(veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
    printf(INDICE_CRIADO, "veiculos_idx");
}

void criar_pistas_idx() {
	
	if (!pistas_idx)
        pistas_idx = malloc(MAX_REGISTROS * sizeof(pistas_index));
 
    if (!pistas_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
 
    for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);
		pistas_idx[i].rrn = i;
        strcpy(pistas_idx[i].id_pista, p.id_pista);
    }
 
    qsort(pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx);
    printf(INDICE_CRIADO, "pistas_idx");
}

void criar_corridas_idx() {
    
	if (!corridas_idx)
        corridas_idx = malloc(MAX_REGISTROS * sizeof(corridas_index));
 
    if (!corridas_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
	
	for (unsigned i = 0; i < qtd_registros_corridas; ++i) {
        Corrida c = recuperar_registro_corrida(i);
        strcpy(corridas_idx[i].ocorrencia, c.ocorrencia);
		strcpy(corridas_idx[i].id_pista, c.id_pista);
		corridas_idx[i].rrn = i;
    }

    qsort(corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx);
    printf(INDICE_CRIADO, "corridas_idx");
}

void criar_nome_pista_idx() {
    
	if (!nome_pista_idx)
        nome_pista_idx = malloc(MAX_REGISTROS * sizeof(nome_pista_index));
 
    if (!nome_pista_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        exit(1);
    }
 
	for (unsigned i = 0; i < qtd_registros_pistas; ++i) {
        Pista p = recuperar_registro_pista(i);
		
        strcpy(nome_pista_idx[i].id_pista, p.id_pista);
		strcpy(nome_pista_idx[i].nome, p.nome);
		
    }

    qsort(nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx);
    printf(INDICE_CRIADO, "nome_pista_idx");
}

void criar_preco_veiculo_idx() {
    
	if (!preco_veiculo_idx)
        preco_veiculo_idx = malloc(MAX_REGISTROS * sizeof(preco_veiculo_index));
 
    if (!preco_veiculo_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        return;
    }
 
	for (unsigned i = 0; i < qtd_registros_veiculos; ++i) {
        Veiculo v = recuperar_registro_veiculo(i);
        strcpy(preco_veiculo_idx[i].id_veiculo, v.id_veiculo);
		preco_veiculo_idx[i].preco = v.preco;
    }

    qsort(preco_veiculo_idx, qtd_registros_veiculos, sizeof(preco_veiculo_index), qsort_preco_veiculo_idx);
    printf(INDICE_CRIADO, "preco_veiculo_idx");
}

void criar_corredor_veiculos_idx() {
    
	Corredor c;

	if(!corredor_veiculos_idx.corredor_veiculos_primario_idx)
		corredor_veiculos_idx.corredor_veiculos_primario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_idx.corredor_veiculos_primario_idx));

	if (!corredor_veiculos_idx.corredor_veiculos_primario_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        return;
	}

	if(!corredor_veiculos_idx.corredor_veiculos_secundario_idx)
		corredor_veiculos_idx.corredor_veiculos_secundario_idx = malloc(MAX_REGISTROS * sizeof(corredor_veiculos_idx.corredor_veiculos_secundario_idx));

	if (!corredor_veiculos_idx.corredor_veiculos_secundario_idx) {
        printf(ERRO_MEMORIA_INSUFICIENTE);
        return;
	}
	
	for(int aux = 0; aux<qtd_registros_corredores; aux++){
		c = recuperar_registro_corredor(aux);

		for(int a = 0; a <QTD_MAX_VEICULO; a++){
			if(c.veiculos[a][0] != '\0')
				inverted_list_insert(c.veiculos[a], c.id_corredor, &corredor_veiculos_idx);
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
	
    printf("%s, %s, %d, %d, %d\n", p.id_pista, p.nome, p.dificuldade, p.distancia, p.recorde);
    return true;
}

bool exibir_corrida(int rrn) {
	
	if (rrn < 0)
        return false;
	
    Corrida c = recuperar_registro_corrida(rrn);
 
    printf("%s, %s, %s, %s\n", c.id_pista, c.ocorrencia, c.id_corredores, c.id_veiculos);
    return true;
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

	return p;
}

Corrida recuperar_registro_corrida(int rrn) {
	Corrida c;
	
	char temp[TAM_REGISTRO_CORRIDA + 1];
	strncpy(temp, ARQUIVO_CORRIDAS + (rrn * TAM_REGISTRO_CORRIDA), TAM_REGISTRO_CORRIDA);
	temp[TAM_REGISTRO_CORRIDA] = '\0';

	strncpy(c.id_pista, temp, TAM_ID_PISTA);
	c.id_pista[TAM_ID_PISTA-1] = '\0';
    strncpy(c.ocorrencia, temp + TAM_ID_PISTA-1 , TAM_DATETIME);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME-1] = '\0';
	strncpy(c.id_corredores, temp + TAM_ID_PISTA + TAM_DATETIME - 2, TAM_ID_CORREDORES);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES-1] = '\0';
	strncpy(c.id_veiculos, temp + TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES-3, TAM_ID_VEICULOS);
	c.id_pista[TAM_ID_PISTA + TAM_DATETIME + TAM_ID_CORREDORES + TAM_ID_VEICULOS-1] = '\0';
	
	return c;
}

/* Escreve em seu respectivo arquivo na posição informada (RRN) */
void escrever_registro_corredor(Corredor c, int rrn) {
	char temp[TAM_REGISTRO_CORREDOR + 1], p[100];
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

	strpadright(temp, '#', TAM_REGISTRO_PISTA);

	strncpy(ARQUIVO_PISTAS + rrn*TAM_REGISTRO_PISTA, temp, TAM_REGISTRO_PISTA);
}

void escrever_registro_corrida(Corrida i, int rrn) {
	
	char temp[TAM_ARQUIVO_CORRIDAS + 1];
	temp[0] = '\0'; 

	strcat(temp, i.id_pista);
	strcat(temp, i.ocorrencia);
	strcat(temp, i.id_corredores);
	strcat(temp, i.id_veiculos);
	
	strncpy(ARQUIVO_CORRIDAS + rrn*TAM_REGISTRO_CORRIDA, temp, TAM_REGISTRO_CORRIDA);
}

/* Funções principais */
void cadastrar_corredor_menu(char *id_corredor, char *nome, char *apelido){
	
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);

	if (found != NULL)
		printf(ERRO_PK_REPETIDA, id_corredor);
	else {
		Corredor c;
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

		strcpy(corredores_idx[qtd_registros_corredores].id_corredor, id_corredor);//atualizando o indice
		corredores_idx[qtd_registros_corredores].rrn = qtd_registros_corredores;
		qtd_registros_corredores++;
		qsort(corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);

		printf(SUCESSO);
	}
}

void remover_corredor_menu(char *id_corredor) {
	
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);
	Corredor c;
	int valor =0;

	if(found == NULL || found->rrn == -1)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else{
		c = recuperar_registro_corredor(found->rrn);
		for(int aux=0; aux<qtd_registros_corredores ; aux++){
			if(!strcmp(corredores_idx[aux].id_corredor, c.id_corredor)){
				valor = found->rrn;
				corredores_idx[aux].rrn = -1;
				break;
			}
		}

		c.id_corredor[0] = '*';
		c.id_corredor[1] = '|';
		escrever_registro_corredor(c, valor);
		printf(SUCESSO);
	}
}

void adicionar_saldo_menu(char *id_corredor, double valor) {
	adicionar_saldo(id_corredor, valor, true);
}

void adicionar_saldo(char *id_corredor, double valor, bool flag){//funcao discutida com Vinicius
	
	Corredor c;

	if(valor <= 0)//verificacao se o valor é valido
		printf(ERRO_VALOR_INVALIDO);
	else{
		corredores_index index;
		strcpy(index.id_corredor, id_corredor);
    	corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);

		if(found != NULL){
			c = recuperar_registro_corredor(found->rrn);
			c.saldo += valor;//adionando a valor na conta
			escrever_registro_corredor(c, found->rrn);
			if(flag)
				printf(SUCESSO);
		} else
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);	
	}	
	
}

void comprar_veiculo_menu(char *id_corredor, char *id_veiculo) {
	
	Corredor c;
	Veiculo v;
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);

	veiculos_index index_v;
	strcpy(index_v.id_veiculo, id_veiculo);
    veiculos_index *found_v = busca_binaria((void*)&index_v, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx, false, 0);
	
	if((found == NULL) || (found_v == NULL)){//verificacao se existem
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	} else {
		c = recuperar_registro_corredor(found->rrn);
		v = recuperar_registro_veiculo(found_v->rrn);
		
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
			inverted_list_insert(v.modelo, found->id_corredor, &corredor_veiculos_idx);
			escrever_registro_corredor(c, found->rrn);
		}
	}
	printf(SUCESSO);
}

void cadastrar_veiculo_menu(char *marca, char *modelo, char *poder, int velocidade, int aceleracao, int peso, double preco) {
	
	char reg[TAM_ID_VEICULO] = "\0";
	Veiculo v;

	strcpy(v.marca, marca);
	strcpy(v.modelo, modelo);
	strcpy(v.poder, poder);

	v.velocidade = velocidade;
	v.aceleracao = aceleracao;
	v.peso = peso;
	v.preco = preco;
	
	sprintf(reg,"%07d", qtd_registros_veiculos);
	strcpy(v.id_veiculo, reg);

	strcpy(veiculos_idx[qtd_registros_veiculos].id_veiculo, reg);//atualizando o indice
	veiculos_idx[qtd_registros_veiculos].rrn = qtd_registros_veiculos;
	strcpy(preco_veiculo_idx[qtd_registros_veiculos].id_veiculo, reg);//atualizando o indice
	preco_veiculo_idx[qtd_registros_veiculos].preco = preco;
	
	escrever_registro_veiculo(v, qtd_registros_veiculos);//adicionando nos arquivos

	qtd_registros_veiculos++;
	qsort(veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx);
	qsort(preco_veiculo_idx, qtd_registros_veiculos, sizeof(preco_veiculo_index), qsort_preco_veiculo_idx);

	printf(SUCESSO);
	
}

void cadastrar_pista_menu(char *nome, int dificuldade, int distancia, int recorde){
	
		nome_pista_index index;
		strcpy(index.nome, nome);
    	nome_pista_index *found = busca_binaria((void*)&index, nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx, false, 0);

		
		if (found != NULL)
			printf(ERRO_PK_REPETIDA, nome);
		else{
			char reg[TAM_ID_PISTA]= "\0";
			Pista p;
			strcpy(p.nome, nome);
			p.dificuldade = dificuldade;
			p.distancia = distancia;
			p.recorde = recorde;

			sprintf(reg,"%08d", qtd_registros_pistas);

			strcpy(p.id_pista, reg);
			strcpy(pistas_idx[qtd_registros_pistas].id_pista, reg);//atualizando o indice
			pistas_idx[qtd_registros_pistas].rrn = qtd_registros_pistas;
			strcpy(nome_pista_idx[qtd_registros_pistas].id_pista, reg);//atualizando o indice
	
			strcpy(nome_pista_idx[qtd_registros_pistas].nome, p.nome);
		
			escrever_registro_pista(p, qtd_registros_pistas);//adicionando nos arquivos

			qtd_registros_pistas++;
			qsort(pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx);
			qsort(nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx);

			printf(SUCESSO);
		}
}

void executar_corrida_menu(char *id_pista, char *ocorrencia, char *id_corredores, char *id_veiculos) {
	
	char corredor_run[qtd_registros_corredores+1][TAM_ID_CORREDOR];
	char veiculo_run[qtd_registros_corredores+1][TAM_ID_VEICULO], id_corre[qtd_registros_corredores+1][12];
	char erro[30];
	int result =0, p=0, aux_pri=0, n = 0, cont=0;

	pistas_index index;
	corredores_index c, *found_c;
	veiculos_index v, *found_v;
	Veiculo V;
	Pista P;

	strcpy(index.id_pista, id_pista);
    pistas_index *found_p = busca_binaria((void*)&index, pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx, false, 0);

	if(found_p == NULL){//se pista existe
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
		return;
	}

	P = recuperar_registro_pista(found_p->rrn);

	double Premio = 6 * (TX_FIXA * P.dificuldade);

	for(int aux = 0; aux < qtd_registros_corridas; aux++){//verificando se ja tem corrida na pista e no horario
		if(!qsort_corridas_idx(id_pista, corridas_idx[aux].id_pista)==0){
			if(!qsort_data_idx(ocorrencia, corridas_idx[aux].ocorrencia)){
				strcpy(erro, ocorrencia);
				strcat(erro, id_pista);
				printf(ERRO_PK_REPETIDA, erro);
				return;
			}
		}
	}
	
	for(int aux=0, cont = 0; cont < 6;cont ++,  aux += TAM_ID_CORREDOR-1){//id_corredores em um vetor
		strncpy(corredor_run[cont], &id_corredores[aux], TAM_ID_CORREDOR-1);
		corredor_run[cont][TAM_ID_CORREDOR-1] =  '\0';
		strcpy(c.id_corredor, corredor_run[cont]);

    	found_c = busca_binaria((void*)&c, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);
		
		if(found_c == NULL){//se corredor existe
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);
			return;
		}
	}
	
	for(int aux=0, cont = 0; cont < 6;cont ++, aux += TAM_ID_VEICULO-1){//id_veiculos em um vetor
		strncpy(veiculo_run[cont], &id_veiculos[aux], TAM_ID_VEICULO);
		veiculo_run[cont][TAM_ID_VEICULO-1] = '\0';
		strcpy(v.id_veiculo, veiculo_run[cont]);
    	found_v = busca_binaria((void*)&v, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx, false, 0);

		if(found_v == NULL){//se veiculo existe
			printf(ERRO_REGISTRO_NAO_ENCONTRADO);
			return;
		}
	}
	
	for(int aux=0, a = 6; aux < a; aux ++){
		veiculos_index h;
		strcpy(h.id_veiculo, veiculo_run[aux]);
    	veiculos_index *vei = busca_binaria((void*)&h, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx, false, 0);
		V = recuperar_registro_veiculo(vei->rrn);

		p=0;

		if(!inverted_list_secondary_search(&result, false, strupr(V.modelo), &corredor_veiculos_idx)){
			printf(ERRO_CORREDOR_VEICULO, corredor_run[aux], veiculo_run[aux]);
			n = 1;
			
		}else{
			cont = inverted_list_primary_search(id_corre, false, result, &aux_pri, &corredor_veiculos_idx);
			for(int i = 0; i < cont; i++){
				if(!strcmp(id_corre[i], corredor_run[aux])){
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
	strcpy(corridas_idx[qtd_registros_corridas].ocorrencia, ocorrencia);
	strcpy(corridas_idx[qtd_registros_corridas].id_pista, id_pista);
	corridas_idx[qtd_registros_corridas].rrn = qtd_registros_corridas;
	qtd_registros_corridas++;

	adicionar_saldo(corredor_run[0], Premio*0.4, false);
	adicionar_saldo(corredor_run[1], Premio*0.3, false);
	adicionar_saldo(corredor_run[2], Premio*0.2, false);

	printf(SUCESSO);
}

/* Busca */
void buscar_corredor_id_menu(char *id_corredor) {
	corredores_index index;
	strcpy(index.id_corredor, id_corredor);
    corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, true, 0);
	if (found == NULL || found->rrn < 0)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else
		exibir_corredor(found->rrn);
}

void buscar_pista_id_menu(char *id_pista) {
	
	pistas_index index;
	strcpy(index.id_pista, id_pista);
    pistas_index *found = busca_binaria((void*)&index, pistas_idx, qtd_registros_pistas, sizeof(pistas_index), qsort_pistas_idx, true, 0);
	
	if (found == NULL || found->rrn < 0)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else
		exibir_pista(found->rrn);
}

void buscar_pista_nome_menu(char *nome_pista) {
	
	nome_pista_index index;
	strcpy(index.nome, nome_pista);
    nome_pista_index *found = busca_binaria((void*)&index, nome_pista_idx, qtd_registros_pistas, sizeof(nome_pista_index), qsort_nome_pista_idx, true, 0);
	if (found == NULL || found->id_pista < 0)
		printf(ERRO_REGISTRO_NAO_ENCONTRADO);
	else
		buscar_pista_id_menu(found->id_pista);
}

/* Listagem */
void listar_corredores_id_menu() {
	if (qtd_registros_corredores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	else
		for (unsigned int i = 0; i < qtd_registros_corredores; i++)
			exibir_corredor(corredores_idx[i].rrn);
}

void listar_corredores_modelo_menu(char *modelo){
	
	int result=0, aux=0, chaves;
	char corredores_mod[qtd_registros_corredores+1][TAM_ID_CORREDOR];
	corredores_index RUNNERS[MAX_REGISTROS];

	if (qtd_registros_corredores == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);

	if(!inverted_list_secondary_search(&result, false, strupr(modelo), &corredor_veiculos_idx)){
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
		return;
	}
	chaves = inverted_list_primary_search(corredores_mod, true, result, &aux, &corredor_veiculos_idx);

	for(int aux =0; aux< chaves; aux++){
		strcpy(RUNNERS[aux].id_corredor, corredores_mod[aux]);
	}	
	qsort(RUNNERS, chaves, sizeof(corredores_index), qsort_corredores_idx);

	for(int aux=0; aux<chaves; aux++){
		corredores_index index;
		strcpy(index.id_corredor, RUNNERS[aux].id_corredor);
    	corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);
		exibir_corredor(found->rrn);	
	}
	
}

void listar_veiculos_compra_menu(char *id_corredor) {
	
	Corredor c;
	Veiculo v;
	int aux = 0;
	
	for (unsigned int i = 0; i < qtd_registros_corredores; i++){//buscando corredor
		corredores_index index;
		strcpy(index.id_corredor, id_corredor);
    	corredores_index *found = busca_binaria((void*)&index, corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx, false, 0);

		if(strcmp(id_corredor, found->id_corredor)==0){
			c = recuperar_registro_corredor(found->rrn);
			break;
		}
	}

	for (unsigned int i = 0; i < qtd_registros_veiculos; i++){

		veiculos_index index_v;
		strcpy(index_v.id_veiculo, preco_veiculo_idx[i].id_veiculo);
    	veiculos_index *found_v = busca_binaria((void*)&index_v, veiculos_idx, qtd_registros_veiculos, sizeof(veiculos_index), qsort_veiculos_idx, false, 0);
		v = recuperar_registro_veiculo(found_v->rrn);

		if(c.saldo > v.preco){
			exibir_veiculo(found_v->rrn);
			aux++;
		}
	}
	if (aux == 0)
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
}

void listar_corridas_periodo_menu(char *data_inicio, char *data_fim) {
	

	if(qtd_registros_corridas == 0){//verificando se tem corrida no sistema
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
		return;
	}

	corridas_index index;
	int cont = 0;

	strcpy(index.ocorrencia, data_inicio);
    corridas_index *found_ini = busca_binaria((void*)&index, corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx, true, 1);
	strcpy(index.ocorrencia, data_fim);
    corridas_index *found_fim = busca_binaria((void*)&index, corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx, false, -1);

	for(int aux = found_ini->rrn; aux < found_fim->rrn+1 ;aux++){
		if((strcmp(corridas_idx[aux].ocorrencia, data_inicio) >= 0) && (strcmp(corridas_idx[aux].ocorrencia, data_fim) <=0)){//confirmando q a corrida esta entre as datas estipuladas
			exibir_corrida(corridas_idx[aux].rrn);
			cont++;
		}
	}

    qsort(corridas_idx, qtd_registros_corridas, sizeof(corridas_index), qsort_corridas_idx);//ordenando o arquivo de corridas por id_pista

	if(cont == 0){
		printf(AVISO_NENHUM_REGISTRO_ENCONTRADO);
	}
}

/* Liberar espaço */
void liberar_espaco_menu() {
	
	char CORRE[TAM_ARQUIVO_CORREDORES], aux[TAM_ARQUIVO_CORREDORES];
	corredores_idx = malloc(MAX_REGISTROS * sizeof(corredores_index));
	corredores_index *COR_idx = malloc(MAX_REGISTROS * sizeof(corredores_index));

	int i = 0;

	for(int aux1=0; aux1 < qtd_registros_corredores; aux1++){//atualizando arquivo
		if (strncmp(&ARQUIVO_CORREDORES[aux1 * TAM_REGISTRO_CORREDOR] , "*|", 2)){//selecionando os arquivos q nao foram removidos
			if(i == 0){//primeiro elemento do arquivo
				strncpy(CORRE, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_REGISTRO_CORREDOR);
			}else{ //vai concatenar no arquivo
				strncpy(aux, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_REGISTRO_CORREDOR);
				strcat(CORRE, aux);
			}
			strncpy(COR_idx[i].id_corredor, ARQUIVO_CORREDORES + aux1*TAM_REGISTRO_CORREDOR, TAM_ID_CORREDOR-1);
			COR_idx[i].rrn = i;
			i++;
		}
	}
	free(corredores_idx);
	corredores_idx = COR_idx;
	qtd_registros_corredores = i;//atualizando numero de corredores
	strcpy(ARQUIVO_CORREDORES, CORRE);
	qsort(corredores_idx, qtd_registros_corredores, sizeof(corredores_index), qsort_corredores_idx);
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
	if (corredores_idx == NULL || qtd_registros_corredores == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_corredores; ++i)
			printf("%s, %0d\n", corredores_idx[i].id_corredor, corredores_idx[i].rrn);
}

void imprimir_veiculos_idx_menu() {
	
	if (veiculos_idx == NULL || qtd_registros_veiculos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_veiculos; ++i)
			printf("%s, %0d\n", veiculos_idx[i].id_veiculo, veiculos_idx[i].rrn);
}

void imprimir_pistas_idx_menu() {
	
	if (pistas_idx == NULL || qtd_registros_pistas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_pistas; ++i)
			printf("%s, %0d\n", pistas_idx[i].id_pista, pistas_idx[i].rrn);
}

void imprimir_corridas_idx_menu() {
	
	if (corridas_idx == NULL || qtd_registros_corridas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_corridas; ++i)
			printf("%s, %s, %0d\n", corridas_idx[i].ocorrencia,corridas_idx[i].id_pista, corridas_idx[i].rrn);
}

/* Imprimir índices secundários */
void imprimir_nome_pista_idx_menu() {
	
	if (nome_pista_idx == NULL || qtd_registros_pistas == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_pistas; ++i)
			printf("%s, %s\n", strupr(nome_pista_idx[i].nome), nome_pista_idx[i].id_pista);
}

void imprimir_preco_veiculo_idx_menu() {
	
	if (preco_veiculo_idx == NULL || qtd_registros_veiculos == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < qtd_registros_veiculos; ++i)
			printf("%.2f, %s\n", preco_veiculo_idx[i].preco, preco_veiculo_idx[i].id_veiculo);
}

void imprimir_corredor_veiculos_secundario_idx_menu() {
	
	if (corredor_veiculos_idx.corredor_veiculos_secundario_idx == NULL || corredor_veiculos_idx.qtd_registros_secundario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_secundario; ++i)
			printf("%s, %d\n", corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].chave_secundaria, corredor_veiculos_idx.corredor_veiculos_secundario_idx[i].primeiro_indice);
}

void imprimir_corredor_veiculos_primario_idx_menu(){
	
	if (corredor_veiculos_idx.corredor_veiculos_primario_idx == NULL || corredor_veiculos_idx.qtd_registros_primario == 0)
		printf(ERRO_ARQUIVO_VAZIO);
	else
		for (unsigned i = 0; i < corredor_veiculos_idx.qtd_registros_primario; ++i)
			printf("%s, %d\n", corredor_veiculos_idx.corredor_veiculos_primario_idx[i].chave_primaria, corredor_veiculos_idx.corredor_veiculos_primario_idx[i].proximo_indice);
}

/* Liberar memória e encerrar programa */
void liberar_memoria_menu() {
	
	free(corredores_idx);
	free(veiculos_idx);
	free(pistas_idx);
	free(corridas_idx);
	free(nome_pista_idx);
	free(preco_veiculo_idx);
	free(corredor_veiculos_idx.corredor_veiculos_primario_idx);
	free(corredor_veiculos_idx.corredor_veiculos_secundario_idx);
}

void inverted_list_insert(char *chave_secundaria, char *chave_primaria, inverted_list *t) {
	
	int result = 0, aux_pri=0;

	if(!inverted_list_secondary_search(&result, false, strupr(chave_secundaria), t)){//inserindo caso nao tenha o modelo no indice secundario

		strcpy(t->corredor_veiculos_secundario_idx[t->qtd_registros_secundario].chave_secundaria, strupr(chave_secundaria));
		strcpy(t->corredor_veiculos_primario_idx[t->qtd_registros_primario].chave_primaria, chave_primaria);

		t->corredor_veiculos_primario_idx[t->qtd_registros_primario].proximo_indice = -1;
		t->corredor_veiculos_secundario_idx[t->qtd_registros_secundario].primeiro_indice = t->qtd_registros_primario;

		t->qtd_registros_primario++;
		t->qtd_registros_secundario++;

		qsort(t->corredor_veiculos_secundario_idx, t->qtd_registros_secundario, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx);
	} else {
		inverted_list_primary_search(NULL, false, result, &aux_pri, t);
		strcpy(t->corredor_veiculos_primario_idx[t->qtd_registros_primario].chave_primaria, chave_primaria);
		t->corredor_veiculos_primario_idx[t->qtd_registros_primario].proximo_indice = -1;

		t->corredor_veiculos_primario_idx[aux_pri].proximo_indice = t->qtd_registros_primario;
		t->qtd_registros_primario++;
	}
}

bool inverted_list_secondary_search(int *result, bool exibir_caminho, char *chave_secundaria, inverted_list *t) {
	
	corredor_veiculos_secundario_index index;
	strcpy(index.chave_secundaria, chave_secundaria);
    corredor_veiculos_secundario_index *found = busca_binaria((void*)&index, t->corredor_veiculos_secundario_idx, t->qtd_registros_secundario, sizeof(corredor_veiculos_secundario_index), qsort_corredor_veiculos_secundario_idx, exibir_caminho, 0);
	int aux;
	
	if(!found)
		return false;
	else{
		aux = found->primeiro_indice;
		if(result)
			*result = aux;
		return true;
	}
}

int inverted_list_primary_search(char result[][TAM_ID_CORREDOR], bool exibir_caminho, int indice, int *indice_final, inverted_list *t) {
	
	int chaves_encontradas = 0;

	if(exibir_caminho){
		printf(REGS_PERCORRIDOS);
		printf(" %d", indice);
	}

	if(result != NULL)
		strcpy(result[chaves_encontradas], t->corredor_veiculos_primario_idx[indice].chave_primaria);
	
	if(indice_final)
		*indice_final = indice; 
	
	chaves_encontradas++;
	indice = t->corredor_veiculos_primario_idx[indice].proximo_indice;
	
	while(indice != -1){
		if(indice_final)
			*indice_final = indice; 
		
		if(exibir_caminho)
			printf(" %d", indice);
		
		if(result != NULL)
			strcpy(result[chaves_encontradas], t->corredor_veiculos_primario_idx[indice].chave_primaria);
		
		indice = t->corredor_veiculos_primario_idx[indice].proximo_indice;
		chaves_encontradas++;

	}

	if(exibir_caminho)
			printf("\n");

	return chaves_encontradas;
}


void* busca_binaria_com_reps(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int posicao_caso_repetido, int retorno_se_nao_encontrado) {
	
	int left = 0, right = nmemb-1, mid, result, found = 0;//left representa o inicio, right o ultimo elemento e o mid o elemento central

	if(exibir_caminho)
		printf(REGS_PERCORRIDOS);

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
		printf ("\n");

	if(found == 1){//elemento foi encontrado
		if((left+right) % 2 != 0)
			mid = (left+right)/2 + 1;
		else
			mid = (left+right)/2; 
		void *aux = (void *)base0 + ((mid)*size);
		return aux;
	} 
	else {
		if(retorno_se_nao_encontrado == 0)
			return NULL;
		else if (retorno_se_nao_encontrado == 1){
			void *aux = (void *)base0 + ((mid)*size);
			return aux;
		} else {
			void *aux = (void *)base0 + ((mid)*size);
			return aux;
		}
	}
}

void* busca_binaria(const void *key, const void *base0, size_t nmemb, size_t size, int (*compar)(const void *, const void *), bool exibir_caminho, int retorno_se_nao_encontrado) {
	return busca_binaria_com_reps(key, base0, nmemb, size, compar, exibir_caminho, 0, retorno_se_nao_encontrado);
}

char *strpadright(char *str, char pad, unsigned size){
	for (unsigned i = strlen(str); i < size; ++i)
		str[i] = pad;
	str[size] = '\0';
	return str;
}
char *strupr(char *str){
	for (char *p = str; *p; ++p)
		*p = toupper(*p);
	return str;
}
char *strlower(char *str){
	for (char *p = str; *p; ++p)
		*p = tolower(*p);
	return str;
}