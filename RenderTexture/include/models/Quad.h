#ifndef QUAD_H
#define QUAD_H

#include <vector>
#include <iostream>
#include "Model.h"
#include "TextureLoader.h"

#include "WindowGL.h"


class Quad : public Model
{

private:

    GLfloat vertices[30];

     
public:
    /**
     * @brief Constructor: Carga y prepara el modelo para ser renderizado.
     */
    Quad(ShaderProgram* program): 
        Model(program) 
    {
        m_model_mat = glm::mat4(1.0f);
        initGeometry(); // Extrae datos del OBJ a los vectores locales
        init();         // Carga los datos a la GPU (VAO, VBO, EBO)
    }

    ~Quad();

    // Funciones de inicialización
    void initGeometry() override;

    void init() override;

    void renderModel(const glm::mat4 &view, const glm::mat4 &projection) override;

    void updateModel(float deltaTime) override;

    void finish() override;


};

#endif