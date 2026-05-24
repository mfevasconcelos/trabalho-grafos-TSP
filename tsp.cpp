#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <climits>

using namespace std;

struct Cidade
{
    int id;
    double x;
    double y;
};

int num_cidades = 0;
vector<vector<int>> matriz_distancia;
vector<int> melhor_rota;
int melhor_peso = 0;
string nome_instancia;

int euc2d(Cidade a, Cidade b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return (int)floor(0.5 + sqrt(dx * dx + dy * dy));
}

void lerInstancia() {
    string linha;

    while (getline(cin, linha)) {
        if (linha.find("NAME") != string::npos) {
            size_t pos_dois_pontos = linha.find(":");
            if (pos_dois_pontos != string::npos) {
                nome_instancia = linha.substr(pos_dois_pontos + 1);
                size_t start = nome_instancia.find_first_not_of(" \t");
                if (start != string::npos)
                {
                    nome_instancia = nome_instancia.substr(start);
                }
            }
        }
        else if (linha.find("DIMENSION") != string::npos) {
            size_t pos_dois_pontos = linha.find(":");
            if (pos_dois_pontos != string::npos) {
                num_cidades = stoi(linha.substr(pos_dois_pontos + 1));
            }
        }
        else if (linha.find("NODE_COORD_SECTION") != string::npos) {
            break;
        }
    }

    // aloca memoria
    vector<Cidade> coordenadas(num_cidades);
    matriz_distancia.assign(num_cidades, vector<int>(num_cidades, 0));

    for (int i = 0; i < num_cidades; i++) {
        cin >> coordenadas[i].id >> coordenadas[i].x >> coordenadas[i].y;
    }

    // apenas metade da matriz por ser TSP simetrico
    for (int i = 0; i < num_cidades; i++) {
        for (int j = i + 1; j < num_cidades; j++) {
            int dist = euc2d(coordenadas[i], coordenadas[j]);
            matriz_distancia[i][j] = dist;
            matriz_distancia[j][i] = dist;
        }
    }
}

void vizinhoMaisProximo() {
    vector<bool> visitado(num_cidades, false);

    melhor_rota.reserve(num_cidades);

    int atual = 0;
    melhor_rota.push_back(atual);
    visitado[atual] = true;

    for (int i = 1; i < num_cidades; i++) {
        int proxima_cidade = -1;
        int menor_distancia = INT_MAX;

        for (int j = 0; j < num_cidades; j++) {
            if (!visitado[j] && matriz_distancia[atual][j] < menor_distancia) {
                menor_distancia = matriz_distancia[atual][j];
                proxima_cidade = j;
            }
        }

        melhor_rota.push_back(proxima_cidade);
        visitado[proxima_cidade] = true;
        melhor_peso += menor_distancia;
        atual = proxima_cidade;
    }

    melhor_peso += matriz_distancia[atual][melhor_rota.back()];
}

void saida() {
    cout << "NAME: " << nome_instancia << "\n";
    cout << "COMMENT: Heuristica construtiva do vizinho mais proximo - Fernanda V, Paula Laiz, Stephany\n";
    cout << "TYPE: TOUR\n";
    cout << "DIMENSION: " << num_cidades << "\n";
    cout << "TOTAL_WEIGHT: " << melhor_peso << "\n";
    cout << "TOUR_SECTION\n";

    for (int i = 0; i < num_cidades; i++) {
        cout << melhor_rota[i] + 1 << "\n";
    }
    cout << "EOF";
}

int main() {
    // otimiza velocidade de leitura
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    lerInstancia();
    vizinhoMaisProximo();
    saida();

    return 0;
}