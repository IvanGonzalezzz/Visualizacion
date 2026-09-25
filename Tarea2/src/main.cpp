#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <vector>

// Vertex Shader (GLSL 4.10)
const char* vertexShaderSource = R"(
    #version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = aColor;
    }
)";

// Fragment Shader (GLSL 4.10)
const char* fragmentShaderSource = R"(
    #version 410 core
    out vec4 FragColor;
    in vec3 vertexColor;

    void main()
    {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

/**
 * @brief Estructura para almacenar los valores x,y de un punto.
 */
struct Point {
    float x, y;
};

/**
 * @brief Función auxiliar para regresar el punto medio.
 */
Point getMidpoint(Point p1, Point p2) {
    return {(p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f};
}

/**
 * @brief Función recursiva para calcular los puntos usados en el triangulo de sierpisnki
 * Dados tres puntos y el nivel del fractal, calcula recursivamente los puntos medios
 * hasta llegar al nivel 1. 
 * En ese caso, genera tres colores aleatorios y guarda los vertices de cada tríangulos
 * @param n el nivel del fractal
 * @param a,b,c los puntos
 * @param vertices la lista de vertices 
 */
void generateSierpinski(int n, Point a, Point b, Point c, std::vector<float>& vertices) {
    // Caso base:
    if (n == 1) {
        // Se generan los tres colores aleatorios (entre 0 y 1)
        float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float b_ = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        // Se guarda el primer vértice
        vertices.push_back(a.x);
        vertices.push_back(a.y);
        vertices.push_back(0.0f);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b_);

        // Se guarda el segundo vértice
        vertices.push_back(b.x);
        vertices.push_back(b.y);
        vertices.push_back(0.0f);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b_);

        // Se guarda el tercer vértice
        vertices.push_back(c.x);
        vertices.push_back(c.y);
        vertices.push_back(0.0f);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b_);

        return;
    }


    // Calculamos los puntos medios
    Point midAB = getMidpoint(a, b);
    Point midBC = getMidpoint(b, c);
    Point midCA = getMidpoint(c, a);

    // Recursión
    generateSierpinski(n - 1, a, midAB, midCA, vertices); // Superior
    generateSierpinski(n - 1, midAB, b, midBC, vertices); // Inferior Izquierdo
    generateSierpinski(n - 1, midCA, midBC, c, vertices); // Inferior Derecho
}

/**
 * @brief Función auxiliar para generar los vértices de todos los triángulos
 */
std::vector<float> getSierpinskiLevel(int n) {
    std::vector<float> vertices;
    if (n < 1) return vertices;
    // Se crea la semilla aleatoria
    std::srand(std::time(nullptr));

    //Puntos para el trángulo que ocupa toda la pantalla
    Point a = { 0.0f,  1.0f};
    Point b = {-1.0f, -1.0f};
    Point c = { 1.0f, -1.0f};

    generateSierpinski(n, a, b, c, vertices);
    return vertices;
}

int main()
{
    std::cout << "Seleccione el número del nivel del fractal: \n";
    int selection;
    std::cin >> selection;
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version to 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwFocusWindow(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Build and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Build and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders to a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Vértices a usar
    std::vector<float> vertices = getSierpinskiLevel(selection);

    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Atributo de posición (location = 0): 3 floats, stride de 6 floats, offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de color (location = 1): 3 floats, stride de 6 floats, offset 3 floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);


    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // Cada vértice ocupa 6 floats (posición + color)
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 6));

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
