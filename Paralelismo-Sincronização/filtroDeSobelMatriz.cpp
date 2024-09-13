#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

using namespace std;

//Isaac Fiuza Vieira e Thassiana C. A. Muller

// kernel de Sobel

//deteccao bordas horizontais
int Gx[3][3] = {{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};
//deteccao bordas verticais
int Gy[3][3] = {{-1, -2, -1},
                { 0,  0,  0},
                { 1,  2,  1}};

int totalMagnitude = 0; // vamos utilizar o armazenamento total das magnitudes para exemplificar o uso do critical 

void filtroDeSobel(const vector<vector<int>>& input, vector<vector<int>>& output, vector<vector<int>>& outputInvertido, int largura, int altura){


    #pragma omp parallel 
    {

            #pragma omp for collapse(2) 
        // O comando collapse(2) paraleliza os dois próximos for's em um único loop 
        // isso é útil quando se trabalha com laços aninhados que não dependem entre si 
        // pois permite a melhor distribuição das iterações entre as threads.
        // Na prática ele combina cada i e j em uma sequência e faz o balanceamento de acordo com o número de threads.

        // Exemplo:
        // Para 4 threads, uma matriz de 5x5 gera 25 sequências de i e j para serem executadas individualmente

        // thread 0: executa as iterações 0 a 6:
        // (i=0, j=0), (i=0, j=1), (i=0, j=2), (i=0, j=3), (i=0, j=4), (i=1, j=0), (i=1, j=1)

        // thread 1: executa as iterações 7 a 12:
        // (i=1, j=2), (i=1, j=3), (i=1, j=4), (i=2, j=0), (i=2, j=1), (i=2, j=2)

        // thread 2: executa as iterações 13 a 18:
        // (i=2, j=3), (i=2, j=4), (i=3, j=0), (i=3, j=1), (i=3, j=2), (i=3, j=3)

        // thread 3: executa as iterações 19 a 24:
        // (i=3, j=4),(i=4, j=0), (i=4, j=1), (i=4, j=2), (i=4, j=3), (i=4, j=4)
        

            for(int i = 1; i < altura - 1; i++){
                for(int j = 1; j < largura - 1; j++){
                    // declarar instancias das variaveis em cada thread
                    // evita que outras threads modifiquem as mesmas variaveis ao msm tempo
                        int sumY = 0;
                        int sumX = 0;

                    for(int k = -1; k <= 1; k++){ //aplica o filtro de Sobel
                        for(int l = -1; l <= 1; l++){
                            sumX += input[i + k][j + l] * Gx[k + 1][l + 1];
                            sumY += input[i + k][j + l] * Gy[k + 1][l + 1];
                        }           
                    }
                    int magnitude = static_cast<int>(sqrt(sumX*sumX + sumY*sumY));

                    #pragma omp atomic write
                    // garante que a operação de escrita na matriz de saida seja feita de maneira atomica.
                    // apenas uma thread por vez pode executar essa operação evitando condições de corrida.
                    // não seria necessário pois cada thread processa um par (i, j) a condições de corrida já é evitada 
                    output[i][j] = min(max(magnitude, 0), 255);

                    // varias threads estao atualizando essa variável simultaneamente gerando condições de corrida 
                    // critical garante que apenas uma thread de cada vez execute o bloco de código 
                    // nao poderia ser atomic pois alem de atribuição há uma soma
                    #pragma omp critical // sem o critical o calculo varia
                    {
                        totalMagnitude += magnitude;
                    }


            }
        }

        // espera que todas as threads terminem a primeira etapa para a matriz output estar completa
        #pragma omp barrier
        #pragma omp for collapse(2)
        for(int i = 1; i < altura - 1; i++){
            for(int j = 1; j < largura - 1; j++){
                outputInvertido[i][j] = 255 - output[i][j];// para inverter as bordas
            }
        }
    }
}


void printMatrix(vector<vector<int>> output, int altura, int largura){
     for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            printf(" %3d", output[i][j] == 255 ? 1: 0);
        }
        printf("\n");
    }
}

int main() {
    vector<vector<int>> input = {{255, 255, 255, 255, 255, 255, 255},
                                 {255,   0,   0,   0,   0,   0, 255},
                                 {255,   0,   0,   0,   0,   0, 255},
                                 {255,   0,   0,   255, 0,   0, 255},
                                 {255,   0,   0,   0,   0,   0, 255},
                                 {255,   0,   0,   0,   0,   0, 255},
                                 {255, 255, 255, 255, 255, 255, 255}};


    int largura = input[0].size();
    int altura = input.size();

    vector<vector<int>> output(altura, vector<int>(largura, 0));
    vector<vector<int>> outputInvertido(altura, vector<int>(largura, 0)); // para exemplificar a espera do barrier


    filtroDeSobel(input, output, outputInvertido, largura, altura);

    printf("Matriz original");
    printf("\n\n");
    printMatrix(input, altura, largura);
    printf("\n\n");
    printf("Matriz bordas");
    printf("\n\n");
    printMatrix(output, altura, largura);
    printf("\n\n");
    printf("Matriz bordas invertidas");
    printf("\n\n");
    printMatrix(outputInvertido, altura, largura);
    printf("\n\n");
    printf("Magnitude total: %d", totalMagnitude);
    printf("\n\n");
    printf("Isaac F. Vieira e Thassiana C. A. Muller");
    printf("\n\n");

    
    return 0;
}

