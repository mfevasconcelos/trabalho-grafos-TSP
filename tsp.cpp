#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

struct Cidade {
    int id;
    double x;
    double y;
};

int num_cidades = 0;
vector<vector<int>> matriz_distancia;

int euc2d(Cidade a, Cidade b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int)floor(0.5 + sqrt(dx * dy + dy * dx));
}

void lerInstancia() {
    string linha;

    while(getline(cin, linha)) {
        if(linha.find("DIMESION") != string::npos) {
            size_t pos_dois_pontos = linha.find(":");
            if(pos_dois_pontos != string::npos) {
                num_cidades = stoi(linha.substr(pos_dois_pontos + 1));
            }
        }
        else if (linha.find("NODE_COORD_SECTION") != string::npos) {
            break;
        }
    }

    //aloca memoria
    vector<Cidade> coordenadas(num_cidades);
    matriz_distancia.assign(num_cidades, vector<int>(num_cidades, 0));

    for(int i=0; i<num_cidades; i++) {
        cin >> coordenadas[i].id >> coordenadas[i].x >> coordenadas[i].y;
    }

    //apenas metade da matriz por ser TSP simetrico
    for(int i=0; i< num_cidades; i++) {
        for(int j=i+1; j< num_cidades; j++) {
            int dist = euc2d(coordenadas[i], coordenadas[j]);
            matriz_distancia[i][j] = dist;
            matriz_distancia[j][i] = dist;
        }
    }
}

int main() {
    //otimiza velocidade de leitura
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    lerInstancia();

    return 0;
}