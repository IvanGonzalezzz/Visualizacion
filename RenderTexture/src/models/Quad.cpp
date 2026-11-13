#include "models/Quad.h"

Quad::~Quad()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Quad::initGeometry()
{
    // Vértice 1
    vertices[0]  = -1.0f;
    vertices[1]  =  1.0f;
    vertices[2]  =  0.0f;
    vertices[3]  =  0.0f;
    vertices[4]  =  1.0f;

    // Vértice 2
    vertices[5]  = -1.0f;
    vertices[6]  = -1.0f;
    vertices[7]  =  0.0f;
    vertices[8]  =  0.0f;
    vertices[9]  =  0.0f;

    // Vértice 3
    vertices[10] =  1.0f;
    vertices[11] = -1.0f;
    vertices[12] =  0.0f;
    vertices[13] =  1.0f;
    vertices[14] =  0.0f;

    // Vértice 4
    vertices[15] = -1.0f;
    vertices[16] =  1.0f;
    vertices[17] =  0.0f;
    vertices[18] =  0.0f;
    vertices[19] =  1.0f;

    // Vértice 5
    vertices[20] =  1.0f;
    vertices[21] = -1.0f;
    vertices[22] =  0.0f;
    vertices[23] =  1.0f;
    vertices[24] =  0.0f;

    // Vértice 6
    vertices[25] =  1.0f;
    vertices[26] =  1.0f;
    vertices[27] =  0.0f;
    vertices[28] =  1.0f;
    vertices[29] =  1.0f;
}

/**
 * @brief Configura el VAO, VBOs y el EBO en la GPU.
 */
void Quad::init()
{
    // configure plane VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

}

void Quad::renderModel(const glm::mat4 &view, const glm::mat4 &projection)
{

    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);
    m_shaderProgram->setTextureUnit("screenTexture",0);

    // Al usar VAOs configurados en el init, solo bindeamos y dibujamos
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Quad::updateModel(float deltaTime)
{

    m_model_mat = glm::rotate(m_model_mat, glm::radians(deltaTime), glm::vec3(0.0f,1.0f,0.0f));
}

void Quad::finish(){}