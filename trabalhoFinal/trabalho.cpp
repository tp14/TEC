#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string.h>
#include <locale.h>   
using namespace std;

typedef struct linha linha;

struct linha{
    char estAtual[30];
    char simAtual[30];
    char novoSimbolo[30];
    char direcao[30];
    char novoEstado[30];
};

vector<linha> linhas = vector<linha>();
vector<string> simbolos = vector<string>();
vector<string> estados = vector<string>();

// le o arquivo de entrada e salvas as regras no vetor linhas
void lerEntrada(){
     FILE *arq;
    arq = fopen("exemplo.in", "rt");
    
    if (arq == NULL)
        printf("Problemas na abertura do arquivo\n");
            
    char aux[300], *parte;
    char *pA, *pb, *pc, *pd, *pe;
    linha nova;
    while(true){
        
        parte=fgets(aux,300,arq);
        if(parte == NULL)break;

        parte = strtok(parte, " ");
        strcpy(nova.estAtual, parte);
        pA = parte;   
    
        parte = strtok(NULL, " ");
        strcpy(nova.simAtual, parte);
        pb = parte; 

        parte = strtok(NULL, " ");
        strcpy(nova.novoSimbolo, parte);
        pc = parte; 

        parte = strtok(NULL, " ");
        strcpy(nova.direcao, parte);
        pd = parte; 

        parte = strtok(NULL, "\n");
        strcpy(nova.novoEstado, parte);
        pe = parte;       
        
    linhas.push_back(nova);
    }
    
    printf("\nEntrada: \n");
    for(int i=0; i<linhas.size(); i++)
        printf("%s %s %s %s %s\n", linhas[i].estAtual, linhas[i].simAtual, linhas[i].novoSimbolo, linhas[i].direcao, linhas[i].novoEstado);

}

// verifica se ja existe esse simbolo salvo
int existeSimbolo(string simbolo){
    for(int i=0; i<simbolos.size(); i++){
            if(simbolos[i] == simbolo)
                return 1;  
    }
    return 0;
}

//le o vetor linhas e armazena os simbolos no vetor simbolos
void armazenaSimbolos(){
    for(int i=0; i<linhas.size(); i++){
        if(existeSimbolo(linhas[i].simAtual) == 0)
                simbolos.push_back(linhas[i].simAtual);
        if(existeSimbolo(linhas[i].novoSimbolo) == 0 )
                simbolos.push_back(linhas[i].novoSimbolo);    
    }  
}

// verifica se ja existe esse estado salvo
int existeEstado(string estado){
    for(int i=0; i<estados.size(); i++){
            if(estados[i] == estado)
                return 1;  
    }
    return 0;
}

//le o vetor linhas e armazena os simbolos no vetor simbolos
void armazenaEstados(){
    for(int i=0; i<linhas.size(); i++){
        if(existeEstado(linhas[i].estAtual) == 0)
                estados.push_back(linhas[i].estAtual);
        if(existeEstado(linhas[i].novoEstado) == 0 )
                estados.push_back(linhas[i].novoEstado);    
    }  
}

// desloca todo vetor uma posicao a frente
void deslocaVetor(int i){
    linhas.push_back(linhas[linhas.size()-1]);
    for(int j = linhas.size()-2; j>i-1; j--)
       linhas[j+1] = linhas[j];
}

// verifica se existe esrita de branco e adapta as regras
void escreveBranco(){
    for(int i=0; i<linhas.size(); i++){
        if(strcmp ("_", linhas[i].novoSimbolo) == 0){
            strcpy(linhas[i].novoSimbolo, "¢");
            if(existeSimbolo(linhas[i].novoSimbolo) == 0 )
                simbolos.push_back(linhas[i].novoSimbolo); 
        }
    }
}

// verifica se existe leitura de branco e adapta as regras
void leBranco(){
    for(int i=0; i<linhas.size(); i++){
        if(strcmp ("_", linhas[i].simAtual) == 0){
            deslocaVetor(i);
            strcpy(linhas[i+1].simAtual, "¢");
            if(existeSimbolo(linhas[i].simAtual) == 0 )
                simbolos.push_back(linhas[i].simAtual); 
        }
    }
}

// verifica se existe movimento estacionario e adapta as regras
void movimentoEstacionario(){  
    int  a=0;
    for(int i=0; i<linhas.size(); i++){
        if(strcmp ("*", linhas[i].direcao) == 0){
            int cont=1;
            string salva = linhas[i].novoEstado;
            string tmp = to_string(a);
            a++;
            char const *num_char = tmp.c_str();
            char *k;
            strcpy(k, "£");
            strcat(k, num_char);

            strcpy(linhas[i].novoEstado, k);
            strcpy(linhas[i].direcao, "r");
            if(existeEstado(k) == 0)
                estados.push_back(k);

            for(int j=0; j<simbolos.size(); j++){
                if(simbolos[j] != "_"){
                    deslocaVetor(i+cont);
                    strcpy(linhas[i+cont].estAtual, k);
                    strcpy(linhas[i+cont].simAtual, simbolos[j].c_str());
                    strcpy(linhas[i+cont].novoEstado, salva.c_str());
                    strcpy(linhas[i+cont].novoSimbolo, simbolos[j].c_str());
                    strcpy(linhas[i+cont].direcao, "l");
                    cont++;
                }
            } 
        }
    }
}

//criar o estado final devido a condição de não poder escrever branco
void estadoFinal(){
    linha final;
    strcpy(final.estAtual, "*");
    strcpy(final.simAtual, "_");
    strcpy(final.novoEstado, "halt-accept");
    strcpy(final.novoSimbolo, "§");
    strcpy(final.direcao, "l");
    linhas.push_back(final);
}

//cria o estado de rejeção para a palavra vazia
void estadoRejeicao(){
    linha rejeicao;
    strcpy(rejeicao.estAtual, "0");
    strcpy(rejeicao.simAtual, "_");
    strcpy(rejeicao.novoEstado, "£reject");
    strcpy(rejeicao.novoSimbolo, "§");
    strcpy(rejeicao.direcao, "r");
    linhas.push_back(rejeicao);
    
    strcpy(rejeicao.estAtual, "£reject");
    strcpy(rejeicao.simAtual, "_");
    strcpy(rejeicao.novoEstado, "halt-reject");
    strcpy(rejeicao.novoSimbolo, "§");
    strcpy(rejeicao.direcao, "r");
    linhas.push_back(rejeicao);
}

int main(){
    lerEntrada();
    armazenaSimbolos();
    armazenaEstados();

    printf("\nSimbolos:\n");
    for(int i=0; i<simbolos.size(); i++)
        printf("%s ",simbolos[i].c_str());

    printf("\n\nEstados:\n");
    for(int i=0; i<estados.size(); i++)
        printf("%s ",estados[i].c_str());

    movimentoEstacionario();

    escreveBranco();

    leBranco();

    estadoFinal();

    estadoRejeicao();

    printf("\n\nSimbolos:\n");
    for(int i=0; i<simbolos.size(); i++)
        printf("%s ",simbolos[i].c_str());

    printf("\n\nEstados:\n");
    for(int i=0; i<estados.size(); i++)
        printf("%s ",estados[i].c_str());

    printf("\n\nSaida: \n");
    

    for(int i=0; i<linhas.size(); i++)
        printf("%s %s %s %s %s\n", linhas[i].estAtual, linhas[i].simAtual, linhas[i].novoSimbolo, linhas[i].direcao, linhas[i].novoEstado);
        

    FILE *saida;
    saida = fopen("saida.out", "wt");
    if (saida == NULL) 
    {
        printf("Problemas na CRIACAO do arquivo\n");
    }

    for(int i=0; i<linhas.size(); i++)
        fprintf(saida,"%s %s %s %s %s\n", linhas[i].estAtual, linhas[i].simAtual, linhas[i].novoSimbolo, linhas[i].direcao, linhas[i].novoEstado);
    
    return 0;
}