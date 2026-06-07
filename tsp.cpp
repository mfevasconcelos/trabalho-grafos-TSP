#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <climits>
#include <algorithm>
#include <chrono>
#include <cstdlib>

using namespace std;

struct Cidade {
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

int calcularPeso(const vector<int> &rota) {
    int peso = 0;
    for (int i = 0; i < num_cidades - 1; i++) {
        peso += matriz_distancia[rota[i]][rota[i + 1]];
    }
    peso += matriz_distancia[rota[num_cidades - 1]][rota[0]];
    return peso;
}

void lerInstancia() {
    string linha;
    while (getline(cin, linha)) {
        if (linha.find("NAME") != string::npos) {
            size_t pos_dois_pontos = linha.find(":");
            if (pos_dois_pontos != string::npos) {
                nome_instancia = linha.substr(pos_dois_pontos + 1);
                size_t start = nome_instancia.find_first_not_of(" \t");
                if (start != string::npos) {
                    nome_instancia = nome_instancia.substr(start);
                }
            }
        }
        else if (linha.find("DIMENSION") != string::npos) {
            size_t pos_dois_pontos = linha.find(":");
            if (pos_dois_pontos != string::npos)
            {
                num_cidades = stoi(linha.substr(pos_dois_pontos + 1));
            }
        }
        else if (linha.find("NODE_COORD_SECTION") != string::npos) {
            break;
        }
    }

    vector<Cidade> coordenadas(num_cidades);
    matriz_distancia.assign(num_cidades, vector<int>(num_cidades, 0));

    for (int i = 0; i < num_cidades; i++) {
        cin >> coordenadas[i].id >> coordenadas[i].x >> coordenadas[i].y;
    }

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
            if (!visitado[j] && matriz_distancia[atual][j] < menor_distancia)
            {
                menor_distancia = matriz_distancia[atual][j];
                proxima_cidade = j;
            }
        }

        melhor_rota.push_back(proxima_cidade);
        visitado[proxima_cidade] = true;
        melhor_peso += menor_distancia;
        atual = proxima_cidade;
    }
    melhor_peso += matriz_distancia[atual][melhor_rota[0]];
}

void otimizar2Opt(vector<int> &rota, int &peso_rota) {
    bool houve_melhoria = true;
    vector<bool> dont_look(num_cidades, false);

    while (houve_melhoria) {
        houve_melhoria = false;
        for (int i = 1; i < num_cidades - 1; i++) {
            int A = rota[i - 1];
            if (dont_look[A])
                continue;

            bool melhoria_em_A = false;

            for (int j = i + 1; j < num_cidades; j++) {
                int B = rota[i];
                int C = rota[j];
                int D = rota[(j + 1) % num_cidades];

                int delta = (matriz_distancia[A][C] + matriz_distancia[B][D]) -
                            (matriz_distancia[A][B] + matriz_distancia[C][D]);

                if (delta < 0) {
                    reverse(rota.begin() + i, rota.begin() + j + 1);
                    peso_rota += delta;
                    houve_melhoria = true;
                    melhoria_em_A = true;

                    dont_look[A] = false;
                    dont_look[B] = false;
                    dont_look[C] = false;
                    dont_look[D] = false;
                }
            }
            if (!melhoria_em_A) {
                dont_look[A] = true;
            }
        }
    }
}

void perturbarRota(vector<int> &rota) {
    if (num_cidades < 8) return;

    int p1 = 1 + rand() % (num_cidades / 4);
    int p2 = p1 + 1 + rand() % (num_cidades / 4);
    int p3 = p2 + 1 + rand() % (num_cidades / 4);

    vector<int> nova_rota;
    nova_rota.reserve(num_cidades);

    nova_rota.insert(nova_rota.end(), rota.begin(), rota.begin() + p1);
    nova_rota.insert(nova_rota.end(), rota.begin() + p3, rota.end());
    nova_rota.insert(nova_rota.end(), rota.begin() + p2, rota.begin() + p3);
    nova_rota.insert(nova_rota.end(), rota.begin() + p1, rota.begin() + p2);

    rota = nova_rota;
}

void saida(const string &comentario, double tempo_total) {
    cout << "NAME: " << nome_instancia << "\n";
    cout << "COMMENT: " << comentario << " | Tempo de execucao: " << tempo_total << "s\n";
    cout << "TYPE: TOUR\n";
    cout << "DIMENSION: " << num_cidades << "\n";
    cout << "TOTAL_WEIGHT: " << melhor_peso << "\n";
    cout << "TOUR_SECTION\n";

    for (int i = 0; i < num_cidades; i++) {
        cout << melhor_rota[i] + 1 << "\n";
    }
    cout << "EOF\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(42);
    int fator = 10;
    string comentario = "ILS com limite de estagnacao = " + to_string(fator) + " - Fernanda V, Paula Laiz, Stephany";

    lerInstancia();

    auto inicio = chrono::high_resolution_clock::now();

    vizinhoMaisProximo();

    vector<int> rota_trabalho = melhor_rota;
    int peso_trabalho = melhor_peso;

    int iteracoes_sem_melhoria = 0;
    int limite_estagnacao = num_cidades * fator;

    while (iteracoes_sem_melhoria < limite_estagnacao) {

        otimizar2Opt(rota_trabalho, peso_trabalho);

        if (peso_trabalho < melhor_peso) {
            melhor_peso = peso_trabalho;
            melhor_rota = rota_trabalho;
            iteracoes_sem_melhoria = 0;
        }
        else {
            iteracoes_sem_melhoria++;
        }

        rota_trabalho = melhor_rota;
        perturbarRota(rota_trabalho);
        peso_trabalho = calcularPeso(rota_trabalho);
    }

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo = fim - inicio;

    saida(comentario, tempo.count());

    return 0;
}