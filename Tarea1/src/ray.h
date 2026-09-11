#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
    private:
        point3 orig;
        vec3 dir;

    public:
        ray(){}

        /**
         * @brief Constructor con parámetros,
         * Inicializa los miembros de la clase
         */
        ray(const point3& origin, const vec3& direction) //:
        {
            orig = origin;
            dir = direction;
        }

        /**
         * @brief Método que regresa el origen del rayo.
         */
        const point3& origin() const
        { 
            return orig; 
        }

        /**
         * @brief Método que devuleve la dirección del rayo
         */
        const vec3& direction() const 
        { 
            return dir; 
        }

        /**
         * @brief Método para conocer el valor del rayo en el punto t.
         * Se calcula realizando origen + t * direction
         */
        point3 at(double t) const 
        {
            return orig + t*dir;
        }
};

#endif