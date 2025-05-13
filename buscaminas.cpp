#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_FILAS = 10;
const int MAX_COLS = 10;

int tablero[MAX_FILAS][MAX_COLS];      // tablero real con minas y números
char visible[MAX_FILAS][MAX_COLS];     // tablero visible para el usuario
bool descubierta[MAX_FILAS][MAX_COLS]; // marca si una celda ha sido destapada

int filas, columnas, minas;

bool enRango(int x, int y) {
    return x >= 0 && x < filas && y >= 0 && y < columnas;
}

void inicializarMatrices() {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            tablero[i][j] = 0;
            visible[i][j] = '-';
            descubierta[i][j] = false;
        }
    }
}

void colocarMinas() {
    minas = columnas + 1;
    srand(time(NULL));
    int colocadas = 0;
    while (colocadas < minas) {
        int x = rand() % filas;
        int y = rand() % columnas;
        if (tablero[x][y] != -1) {
            tablero[x][y] = -1;
            colocadas++;

            // Sumar 1 a las celdas adyacentes
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (enRango(nx, ny) && tablero[nx][ny] != -1) {
                        tablero[nx][ny]++;
                    }
                }
            }
        }
    }
}

void generarTablero() {
    inicializarMatrices();
    colocarMinas();
}

void mostrarTablero(bool revelar = false) {
    cout << "    ";
    for (int j = 0; j < columnas; ++j)
        cout << j << " ";
    cout << endl;
    for (int i = 0; i < filas; ++i) {
        cout << i << " | ";
        for (int j = 0; j < columnas; ++j) {
            if (revelar) {
                if (tablero[i][j] == -1)
                    cout << "* ";
                else
                    cout << tablero[i][j] << " ";
            } else {
                cout << visible[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void destapar(int x, int y) {
    if (!enRango(x, y) || descubierta[x][y]) return;
    descubierta[x][y] = true;
    visible[x][y] = (tablero[x][y] == 0) ? '0' : '0' + tablero[x][y];

    if (tablero[x][y] == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx != 0 || dy != 0)
                    destapar(x + dx, y + dy);
            }
        }
    }
}

bool verificarVictoria() {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] != -1 && !descubierta[i][j])
                return false;
        }
    }
    return true;
}

void procesarJugada() {
    int x, y;
    cout << "\nIngrese fila y columna: ";
    cin >> x >> y;

    if (!enRango(x, y)) {
        cout << "Coordenadas fuera de rango.\n";
        return;
    }

    if (tablero[x][y] == -1) {
        cout << "\n💥 GAME OVER. Pisaste una mina.\n";
        mostrarTablero(true);
        exit(0);
    } else {
        destapar(x, y);
        if (verificarVictoria()) {
            cout << "\n🎉 ¡Has ganado!\n";
            mostrarTablero(true);
            exit(0);
        }
    }
}

int main() {
    cout << "Ingrese las dimensiones del tablero (filas columnas): ";
    cin >> filas >> columnas;

    generarTablero();

    

    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
    cout << string(50, '\n');

    while (true) {
        mostrarTablero();
        procesarJugada();
    }

    return 0;
}
