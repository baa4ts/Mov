# Movment Library

Una pequeña librería en C++ para controlar el movimiento del cursor en sistemas Windows.

## Características

- **Obtener posición actual:** Recupera las coordenadas actuales del cursor.  
- **Posición aleatoria:** Genera coordenadas aleatorias dentro de la pantalla.  
- **Ruta con A\*:** Calcula una ruta entre dos puntos utilizando el algoritmo A\*.  
- **Mover cursor:** Mueve el cursor a lo largo de la ruta o a una posición fija.

## Requisitos

- **Sistema operativo:** Windows (uso de la API de Windows)
- **Compilador:** C++11 o superior

## Uso Rápido

Incluye el archivo `movment.hpp` en tu proyecto y utiliza la clase `Mouse`:

```cpp
#include "movment.hpp"
#include <iostream>

int main() {
    Movment::Mouse mouse;
    
    // Obtener y mostrar la posición actual del cursor
    auto [x, y] = mouse.PosicionActualCursor();
    std::cout << "Posición actual: (" << x << ", " << y << ")\n";
    
    // Mover el cursor a una posición aleatoria de la pantalla
    mouse.MoverAleatoriamenteCursor();
    
    // Mover el cursor a (100, 100)
    if (mouse.MoverCursor(100, 100)) {
        std::cout << "Cursor movido a (100, 100).\n";
    } else {
        std::cout << "Error al mover el cursor.\n";
    }
    
    return 0;
}
```

## Licencia

Distribuido bajo la [**BSD 3-Clause License**](./LICENSE).