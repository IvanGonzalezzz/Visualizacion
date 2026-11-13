#ifndef CUSTOMMODEL_H
#define CUSTOMMODEL_H

#include <vector>
#include <iostream>
#include "Model.h"
#include "TextureLoader.h"

#include "WindowGL.h"

// Definimos el número de Vertex Buffer Objects: Posición, Textura, Normales
#define NUM_VBOS 3

class CustomModel : public Model
{
private:
    // Configuración del modelo
    const char *m_filePath;

    // IDs de OpenGL
    GLuint m_VBO[NUM_VBOS];
    GLuint m_EBO; // Element Buffer Object para indexación

    // Datos de la malla (CPU)
    std::vector<float> m_vertices;
    std::vector<float> m_textCoords;
    std::vector<float> m_normalVecs;
    std::vector<unsigned int> m_indices; 

    
    // Contadores para el renderizado
    int m_numIndices;

    // Texture
    GLuint m_textureID;

    void RenderTexture() const;
     
public:
    /**
     * @brief Constructor: Carga y prepara el modelo para ser renderizado.
     */
    CustomModel(ShaderProgram* program, const char *filePath, const char *texturePath): 
        Model(program), 
        m_filePath(filePath), 
        m_numIndices(0)
    {
        m_model_mat = glm::mat4(1.0f);
        initGeometry(); 
        init();         
        m_textureID = TextureLoader::LoadTexture(texturePath);
    }

    /**
     * @brief Constructor sin textura
     */
    CustomModel(ShaderProgram* program, const char *filePath): 
        Model(program), 
        m_filePath(filePath), 
        m_numIndices(0)
    {
        m_model_mat = glm::mat4(1.0f);
        initGeometry(); 
        init();        
    }

    ~CustomModel();

    // Funciones de inicialización
    void initGeometry() override;

    void init() override;

    void renderModel(const glm::mat4 &view, const glm::mat4 &projection, bool texture) override;

    void updateModel(float deltaTime) override;

    void finish() override;

    int getNumIndices() const { return m_numIndices; }

};

#endif