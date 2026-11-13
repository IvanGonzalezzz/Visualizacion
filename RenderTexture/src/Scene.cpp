#include "Scene.h"

void Scene::mouseCallback(GLFWwindow *window, int button,int action,int mods)
{
    Scene* instance = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    mods = 0;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double currX,currY = mods;
        glfwGetCursorPos(window,&currX,&currY);
        instance->m_last = glm::vec2((float)currX,(float)currY);
        //std::cout << "("<< currX << "," << currY<<")"<<std::endl;
        instance->m_cameraMovement = true;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        instance->m_cameraMovement = false;
    }
}

void Scene::createRenderTexture()
{
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);    
}

void Scene::init(){
    //Inicializacion de ventana 
    m_window = new WindowGL();
    glfwSetWindowUserPointer(m_window->getWindow(), this);
    glfwSetMouseButtonCallback(m_window->getWindow(),Scene::mouseCallback);
    //Inicializacion de ProgramShader 
    ShaderProgram* m_shaderProgram = new ShaderProgram("shaders/vertex_shader.glsl","shaders/frag_shader.glsl");
    ShaderProgram* m_shaderProgram2 = new ShaderProgram("shaders/quad_vertex.glsl","shaders/quad_frag.glsl");


    m_shaderPrograms.push_back(m_shaderProgram);


    Model* obj = new CustomModel(m_shaderProgram, "assets/obj/rose.obj", "assets/textures/Rose_Albedo.jpg");
    Model* quad = new Quad(m_shaderProgram2);

    m_models.push_back(obj);
    m_models.push_back(quad);



    //Matriz de vista View Matrix
    //Posicion de la camara, centro de atencion, vector perpendicular de la Cámara
    m_camera = new OrbitCamera(glm::vec3(0.5f,3.0f,4.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
    m_view =  m_camera->getMatrix();
    //Matriz de projeccion
    m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRation(), 0.1f, 100.0f);
}

void Scene::render()  {

    createRenderTexture();
    Model* obj = m_models[0];
    Model* quad = m_models[1];


    //Ciclo de Renderizado
    glEnable(GL_DEPTH_TEST);  // Prueba de profundidad
    //glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    //glCullFace(GL_BACK);      // Culling de caras traseras
    //glFrontFace(GL_CCW);      // Las caras frontales son las que tienen vértices en sentido antihorario


    m_cameraMovement = false;
    while(!glfwWindowShouldClose(m_window->getWindow())){
        // Compute time  
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window->getWindow(), true);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            delete m_camera;
            m_camera = new OrbitCamera(glm::vec3(0.5f,3.0f,4.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glViewport(0, 0, 1280, 720);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // se dibujan los modelos
        obj->renderModel(m_view, m_projection);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window->getWindow(), &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        glDisable(GL_DEPTH_TEST);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        quad->renderModel(m_view,m_projection);
        //quad->updateModel(0.7f);


        // update models 
        glfwSwapBuffers(m_window->getWindow());
        glfwPollEvents();

        
        if (m_cameraMovement)
        {
            double currX,currY = 0;
            glfwGetCursorPos(m_window->getWindow(),&currX,&currY);
            m_current = glm::vec2((float)currX,(float)currY);
            m_camera->rotate(m_last,m_current);
            m_camera->setPos(m_last,m_current);

        }
        m_view = m_camera->getMatrix();
    }

}