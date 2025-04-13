#ifndef MOVMENT_HPP
#define MOVMENT_HPP

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <cmath>
#include <queue>
#include <algorithm>
#include <tuple>
#include <ctime>

namespace Movment
{
    class Mouse
    {
    public:
        /**
         * @brief Constructor de la clase Mouse. Inicializa las dimensiones de la pantalla y la semilla aleatoria.
         */
        Mouse()
        {
            WIDTH = GetSystemMetrics(SM_CXSCREEN);
            HEIGHT = GetSystemMetrics(SM_CYSCREEN);
            std::srand(static_cast<unsigned int>(std::time(0)));
        }

        /**
         * @brief Destructor de la clase Mouse.
         */
        ~Mouse() {}

        /**
         * @brief Obtiene la posición actual del cursor.
         *
         * @return std::tuple<int, int> Coordenadas del cursor (x, y).
         * Si no se puede obtener la posición, devuelve (-1, -1).
         */
        std::tuple<int, int> PosicionActualCursor()
        {
            POINT Cursor;
            if (GetCursorPos(&Cursor))
            {
                return std::make_tuple(Cursor.x, Cursor.y);
            }
            else
            {
                return std::make_tuple(-1, -1);
            }
        }

        /**
         * @brief Genera una posición aleatoria dentro de los límites de la pantalla.
         *
         * @return std::tuple<int, int> Coordenadas aleatorias (x, y) dentro de la pantalla.
         */
        std::tuple<int, int> PosicionAleatoriaPantalla()
        {
            return std::make_tuple(std::rand() % WIDTH, std::rand() % HEIGHT);
        }

        /**
         * @brief Genera una ruta entre dos puntos en la pantalla utilizando el algoritmo A*.
         *
         * @param InicioX Coordenada x del punto de inicio.
         * @param InicioY Coordenada y del punto de inicio.
         * @param FinalX Coordenada x del punto final.
         * @param FinalY Coordenada y del punto final.
         *
         * @return std::vector<std::pair<int, int>> Secuencia de coordenadas que conforman la ruta.
         */
        std::vector<std::pair<int, int>> GenerarRuta(int InicioX, int InicioY, int FinalX, int FinalY)
        {
            std::vector<std::vector<bool>> Visitado(HEIGHT, std::vector<bool>(WIDTH, false));

            struct Nodo
            {
                int x, y, g, h;
                std::shared_ptr<Nodo> previo;
                Nodo(int x, int y, int g, int h, std::shared_ptr<Nodo> previo = nullptr)
                    : x(x), y(y), g(g), h(h), previo(previo) {}
                int f() const { return g + h; }
            };

            struct NodoComparador
            {
                bool operator()(const std::shared_ptr<Nodo> &a, const std::shared_ptr<Nodo> &b)
                {
                    return a->f() > b->f();
                }
            };

            std::priority_queue<std::shared_ptr<Nodo>, std::vector<std::shared_ptr<Nodo>>, NodoComparador> Abierto;

            Abierto.push(std::make_shared<Nodo>(InicioX, InicioY, 0, Heuristica(InicioX, InicioY, FinalX, FinalY)));

            int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
            int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

            while (!Abierto.empty())
            {
                std::shared_ptr<Nodo> Actual = Abierto.top();
                Abierto.pop();

                if (Visitado[Actual->y][Actual->x])
                    continue;
                Visitado[Actual->y][Actual->x] = true;

                if (Actual->x == FinalX && Actual->y == FinalY)
                {
                    std::vector<std::pair<int, int>> Ruta;
                    for (std::shared_ptr<Nodo> p = Actual; p != nullptr; p = p->previo)
                    {
                        Ruta.push_back({p->x, p->y});
                    }
                    std::reverse(Ruta.begin(), Ruta.end());
                    return Ruta;
                }

                for (int i = 0; i < 8; i++)
                {
                    int nx = Actual->x + dx[i];
                    int ny = Actual->y + dy[i];

                    if (nx >= 0 && ny >= 0 && nx < WIDTH && ny < HEIGHT && !Visitado[ny][nx])
                    {
                        Abierto.push(std::make_shared<Nodo>(nx, ny, Actual->g + 1, Heuristica(nx, ny, FinalX, FinalY), Actual));
                    }
                }
            }

            return {}; // Si no hay ruta
        }

        /**
         * @brief Mueve el cursor de forma aleatoria desde su posición actual a una nueva posición generada aleatoriamente.
         * Utiliza el algoritmo A* para calcular el camino entre las posiciones.
         */
        void MoverAleatoriamenteCursor()
        {
            auto inicio = PosicionActualCursor();
            auto destino = PosicionAleatoriaPantalla();
            auto Camino = GenerarRuta(
                std::get<0>(inicio), std::get<1>(inicio),
                std::get<0>(destino), std::get<1>(destino));

            if (Camino.size() < 2)
                return;

            for (const auto &punto : Camino)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(1000));
                SetCursorPos(punto.first, punto.second);
            }
        }

        /**
         * @brief Mueve el cursor a las coordenadas especificadas.
         *
         * La función mueve el cursor a la posición (X, Y) si dichas coordenadas se encuentran dentro de los límites de la pantalla.
         *
         * @param X Coordenada x de destino.
         * @param Y Coordenada y de destino.
         * @return true si el cursor se mueve correctamente, false en caso contrario.
         */
        bool MoverCursor(int X, int Y)
        {
            if (X >= 0 && X <= WIDTH && Y >= 0 && Y <= HEIGHT)
            {
                return SetCursorPos(X, Y);
            }
            return false;
        }

    private:
        int WIDTH;  ///< Ancho de la pantalla.
        int HEIGHT; ///< Alto de la pantalla.

        /**
         * @brief Calcula el valor heurístico utilizando la distancia Manhattan entre dos puntos.
         *
         * @param x1 Coordenada x del primer punto.
         * @param y1 Coordenada y del primer punto.
         * @param x2 Coordenada x del segundo punto.
         * @param y2 Coordenada y del segundo punto.
         *
         * @return int Valor heurístico.
         */
        static int Heuristica(int x1, int y1, int x2, int y2)
        {
            return std::abs(x1 - x2) + std::abs(y1 - y2);
        }
    };
}

#endif // MOVMENT_HPP