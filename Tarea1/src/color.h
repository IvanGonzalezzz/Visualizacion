#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "header.h"
#include "vec3.h"


using color = vec3;

/**
 * @brief Método que convierte un color en su forma lineal a gamma.
 * Sucede que si el componente lineal es mayor a 0, regresa la raiz cuadrada del mismo.
 * Si no, regresa 0.
 * 
 * @param linear_component el componente lineal
 * @returns la raiz cuadrada del componente o 0
 */
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream& out, const color& pixel)
{
    auto r = pixel.x();
    auto g = pixel.y();
    auto b = pixel.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000,0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif