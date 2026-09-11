#ifndef INTERVAL_H
#define INTERVAL_H

#include "header.h"

class interval 
{
    public:
        double min,max;
        
        interval() : min(+infinity), max(-infinity) {}

        /**
         * @brief constructor con parámetros que inicializa los miembros min y max
         */
        interval(double min, double max)
        {
            this->min = min;
            this->max = max;
        }

        /**
         * @brief Regresa el tamaño del intervalo, es decir la difernecia entre max y min
         */
        double size() const
        {
            return max - min;
        }

        /**
         * @brief Método que indica si x pertenece al intervalo. 
         * No es exculyente, es decir que x puede valer lo que los extremos.
         * 
         * @param x el valor a probar
         * @returns true si pertenece, false en otro caso
         */
        bool contains(double x) const
        {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const 
        {
            return min < x && x < max;
        }

        /**
         * @brief Método que regresa x si está en el intervalo, max si x es mayor que el intervalo o min si x es menor que el intervalo
         */
        double clamp(double x) const
        {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif