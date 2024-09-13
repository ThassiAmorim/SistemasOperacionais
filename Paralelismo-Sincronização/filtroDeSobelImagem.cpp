#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <omp.h>

using namespace cv;
using namespace std;

// matriz de para convolução da formula
int Gx[3][3] = {{-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};

int Gy[3][3] = {{-1, -2, -1},
                { 0,  0,  0},
                { 1,  2,  1}};

void filtroDeSobel(const vector<vector<int>>& input, vector<vector<int>>& output, vector<vector<int>>& outputInvertido, int largura, int altura){


    #pragma omp parallel 
    {

        #pragma omp for collapse(2) 
        // O comando collapse(2) paraleliza os dois próximos for's em um único loop 
        // isso é útil quando se trabalha com laços aninhados que não dependem entre si pois permite a melhor distribuição das iterações entre as threads.
        // Na prática ele combina i e j em uma única sequência e faz o balanceamento de acordo com o número de threads.

        for(int i = 1; i < altura - 1; i++){
            for(int j = 1; j < largura - 1; j++){
                    int sumY = 0;
                    int sumX = 0;

                for(int k = -1; k <= 1; k++){
                    for(int l = -1; l <= 1; l++){
                        sumX += input[i + k][j + l] * Gx[k + 1][l + 1];
                        sumY += input[i + k][j + l] * Gy[k + 1][l + 1];
                    }           
                }
                int magnitude = static_cast<int>(sqrt(sumX*sumX + sumY*sumY));

                #pragma omp atomic write

                output[i][j] = min(max(magnitude, 0), 255);
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

vector<vector<int>> matToVector(const Mat& mat) {
    vector<vector<int>> vec(mat.rows, vector<int>(mat.cols));
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            vec[i][j] = mat.at<uchar>(i, j);
        }
    }
    return vec;
}

Mat vectorToMat(const vector<vector<int>>& vec) {
    Mat mat(vec.size(), vec[0].size(), CV_8UC1);
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            mat.at<uchar>(i, j) = vec[i][j];
        }
    }
    return mat;
}


int main() {
    Mat input = imread("imagem.jpg", IMREAD_GRAYSCALE);

    vector<vector<int>> inputVec = matToVector(input);
    vector<vector<int>> outputVec(input.rows, vector<int>(input.cols, 0));
    vector<vector<int>> outputInvertidoVec(input.rows, vector<int>(input.cols, 0));

    filtroDeSobel(inputVec, outputVec, outputInvertidoVec, input.cols, input.rows);

    Mat output = vectorToMat(outputVec);
    Mat outputInvertido = vectorToMat(outputInvertidoVec);

    imshow("Imagem Original", input);
    imshow("Filtro Sobel", output);
    imshow("Filtro Sobel Invertido", outputInvertido);

    imwrite("imagem_sobel.png", output);

    waitKey(0);
    return 0;
}