#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

// Vertex Shader source code (GLSL 4.10)
const char* vertexShaderSource = R"(
    #version 410 core
    layout (location = 0) in vec3 aPos;
    out vec3 vertexColor;
    uniform float atime;

    void main()
    {
        gl_Position = vec4(aPos,1.0);
        vertexColor = (aPos * sin(atime) + 1.0)*0.5;
    }
)";

// Fragment Shader source code (GLSL 4.10)
const char* fragmentShaderSource = R"(
    #version 410 core
    out vec4 FragColor;
    in vec3 vertexColor;
    uniform float choose;

    vec3 cor(vec2 c)
    {
        float a1 = (c.x*c.x) + (c.y*c.y) -1.0;
        float a2 = (a1*a1*a1)-((c.x*c.x)*(c.y*c.y*c.y));
        if (a2<=0.0){
            return vertexColor;
        }
        return vec3(0.0,0.0,0.0);
    }

    // c es el pixel que estamos evaluando y z simpre será 0
    // zn = z*z + c
    // z0 = 0.0
    int mandelbrot(vec2 c, float max_iter)
    {
        int i = 0;
        vec2 z = vec2(0.0);

        while( length(z) <= 2.0 && (i < max_iter))
        {
        
          float a = z.x * z.x - z.y * z.y ;
          float b = 2.0 * z.x * z.y;
        
          z = vec2(a,b) + c;
          i++;
        }
        return i;    
    }

    void main()
    {
        // Domino (0,0) x (800,800) a (-2,-2) x (2,2)    
        float x = gl_FragCoord.x / 1600 ;
        float y = gl_FragCoord.y / 1600 ;

        float x1 = (x-0.5)*4;
        float y1 = (y-0.5)*4;

        if (choose == 0.0)
        {
            vec3 color = cor(vec2(x1,y1));
            FragColor = vec4(color,1.0);
        }
        else 
        {
            float max_iterations = 500.0;
            float iteraciones = mandelbrot(vec2(x1,y1), max_iterations);
            if(iteraciones < max_iterations)
            {
                float t = iteraciones/max_iterations;
                //float intensidad = smoothstep(0.1, 0.5, t);

                //FragColor = vec4(intensidad*vertexColor,1.0);
                FragColor = vec4(t,0.0,0.0,1.0);
            } 
            else 
            {
                FragColor = vec4(0.0,0.0,0.0,1.0);
            }
        }
    }
)";
       

int main()
{
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
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

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

    float vertices[] = {
        -1.f,  -1.f,  0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.f,  -1.f,  0.0f,
         1.f,   1.f,  0.0f,
         1.f,  -1.f,  0.0f
    };

    GLuint VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);

    // Variables Uniformes
    float choose = 0.0f;
    float time = 0.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Render
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);

        GLuint uniformChoose = glGetUniformLocation(shaderProgram, "choose");
        glUniform1f(uniformChoose,choose);
        GLuint uniformTime = glGetUniformLocation(shaderProgram, "atime");
        glUniform1f(uniformTime,time);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
            choose = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS){
            choose = 0.0f;
        }    
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
