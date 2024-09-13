/*
Trabalho 4 - Gerenciamento de memoria
Isaac Fiuza Vieira e Thassiana Camilia Amorim Muller
*/ 

#include <iostream>
#include <vector>
using namespace std;

#define MAX 50

struct frame {
    int pagina; //número da página armazenada
    int tempo; //momento que foi armazenada
};

vector<frame> ini_frames(bool otimo, int FRAMES){ 
    vector<frame> frames(FRAMES);
    for(int i = 0; i < FRAMES; i++){
        frames[i].pagina = -1;
        frames[i].tempo = otimo ? MAX:0;
    }
    return frames;
}

int fifo(vector<int> seq, int FRAMES){

    int page_faults = 0;
    int antigo;
    int k;
    bool encontrado;

    vector<frame> frames = ini_frames(false, FRAMES);

    for(int i = 0; i < seq.size(); i++){
        encontrado = false;
        antigo = frames[0].tempo; // inicia com 0 - equivale a fila
        k = 0;
        //busca se a página já está salva nos quadros
        for(int j = 0; j < FRAMES; j++){ 
            if(seq[i] == frames[j].pagina){
                encontrado = true;
                break;
            }
            // ao mesmo tempo que percorre os quadros identifica a página mais antiga armazenada
            if(antigo > frames[j].tempo){ 
                    antigo = frames[j].tempo; 
                    k = j;
                }

        }

        // Uma vez que a página não foi encontrada, ela é alocada no mesmo local de memória identificado como mais antiga
        if(!encontrado){
            page_faults++;
            frames[k].pagina = seq[i];
            frames[k].tempo = i + 1;
        }
    }

    return page_faults;
}

int lru(vector<int> seq, int FRAMES){
    int page_faults = 0;
    int antigo;
    int k;
    bool encontrado;

    vector<frame> frames = ini_frames(false, FRAMES);
    
    for(int i = 0; i < seq.size(); i++){
        encontrado = false;
        antigo = frames[0].tempo;
        k = 0;
        //busca se a página já está salva nos quadros
        for(int j = 0; j < FRAMES; j++){
            if(seq[i] == frames[j].pagina){
                encontrado = true;
                frames[j].tempo = i + 1; // O que diferencia do FIFO é que o tempo armazenado 
                break;                   // é atualizado quando ela é frequentemente acessada
            }
            // ao mesmo tempo que percorre os quadros identifica a página mais antiga acessada
            if(antigo > frames[j].tempo){
                    antigo = frames[j].tempo;
                    k = j;
                }
        }

        if(!encontrado){
            page_faults++;
            frames[k].pagina = seq[i];
            frames[k].tempo = i + 1;
        }
    }

    return page_faults;
}

int proximo(int agora, vector<int> seq){
    int alvo = seq[agora];
    for(int i = agora + 1; i < seq.size(); i++){
        if(seq[i] == alvo){
            return i;
        }
    }
    return MAX; // caso a pagina não seja acessada novamente
}

int otimo (vector<int> seq, int FRAMES){
    int page_faults = 0;
    int antigo;
    int k;
    bool encontrado;

    //Nesse algoritimo a variavel tempo de frame simboliza o tempo que a página será acessada novamente

    vector<frame> frames = ini_frames(true, FRAMES);

    for(int i = 0; i < seq.size(); i++){
        encontrado = false;
        antigo = frames[0].tempo;
        k = 0;

        for(int j = 0; j < FRAMES; j++){
            if(seq[i] == frames[j].pagina){
                encontrado = true;
                frames[j].tempo = proximo(i, seq);
                break;                   
            }
            if(antigo < frames[j].tempo){
                antigo = frames[j].tempo;
                k = j;
            }

        }

        if(!encontrado){
            page_faults++;
            frames[k].pagina = seq[i];
            frames[k].tempo = proximo(i, seq);
        }
    }

    return page_faults;
}



int main (){

    vector<int> seq = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    vector<int> seq1 = {0, 1, 2, 3, 4, 0, 1, 2, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 0, 1, 2, 5, 0, 1, 2, 3, 4, 5};
    
    printf("\nIsaac Fiuza Vieira e Thassiana Camilia Amorim Muller\n");
    printf("{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}");
    printf("\n\n");
    cout << " FIFO: " << fifo(seq, 3) << " -> " << fifo(seq, 4) << " -> " << fifo(seq, 5) << " -> " << lru(seq, 6) << endl;
    cout << "  LRU: " << lru(seq, 3) << " -> " << lru(seq, 4) << " -> " << lru(seq, 5) << " -> " << lru(seq, 6) << endl;
    cout << "Otimo: " << otimo(seq, 3) << " -> " << otimo(seq, 4) << " -> " << otimo(seq, 5) << " -> " << otimo(seq, 6) << endl;
    printf("\n---------------------------------------------------------------------------------------------------------------\n\n");
    printf("{0, 1, 2, 3, 4, 0, 1, 2, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 0, 1, 2, 5, 0, 1, 2, 3, 4, 5}");
    printf("\n\n");
    cout << " FIFO: " << fifo(seq1, 3) << " -> " << fifo(seq1, 4) << " -> " << fifo(seq1, 5) << " -> " << lru(seq1, 6) << endl;
    cout << "  LRU: " << lru(seq1, 3) << " -> " << lru(seq1, 4) << " -> " << lru(seq1, 5) << " -> " << lru(seq1, 6) << endl;
    cout << "Otimo: " << otimo(seq1, 3) << " -> " << otimo(seq1, 4) << " -> " << otimo(seq1, 5) << " -> " << otimo(seq1, 6) << endl;

    return 0;
}