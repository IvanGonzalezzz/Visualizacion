#include "Scene.h"

/**
 * @brief Función para reflejar un punto dado un plano.
 * Es usado para calcular la posición de la cámara dentro del espejo
 */
glm::vec3 Scene::reflectPoint(glm::vec3 point, glm::vec3 planePoint, glm::vec3 planeNormal)
{
    glm::vec3 v = point - planePoint;
    float distance = glm::dot(v, planeNormal);
    return point - 2.0f * distance * planeNormal;
}

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

/**
 * @brief Función para crear el nuevo framebuffer y la textura
 */
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
    ShaderProgram* m_shaderProgram2 = new ShaderProgram("shaders/mirror_vertex.glsl","shaders/mirror_frag.glsl");


    m_shaderPrograms.push_back(m_shaderProgram);


    Model* ball = new CustomModel(m_shaderProgram, "assets/obj/ball.obj", "assets/textures/ball.jpg");
    Model* chair = new CustomModel(m_shaderProgram, "assets/obj/chair.obj", "assets/textures/Chair.jpg");
    Model* mesa = new CustomModel(m_shaderProgram, "assets/obj/nightstand.obj", "assets/textures/Mesa.png");
    Model* mirror = new CustomModel(m_shaderProgram, "assets/obj/mirror.obj", "assets/textures/Mirror.jpg");
    Model* plane = new CustomModel(m_shaderProgram2, "assets/obj/plane.obj");


    m_models.push_back(ball);
    m_models.push_back(chair);
    m_models.push_back(mesa);
    m_models.push_back(mirror);
    m_models.push_back(plane);




    //Matriz de vista View Matrix
    //Posicion de la camara, centro de atencion, vector perpendicular de la Cámara
    m_camera = new OrbitCamera(glm::vec3(6.55f,5.27f,1.21f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
    m_view =  m_camera->getMatrix();
    //Matriz de projeccion
    m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRation(), 0.1f, 100.0f);
}

void Scene::render()  {

    createRenderTexture();
    glm::vec3 planePoint(-2.765f, 0.576f, 0.476f);
    glm::vec3 planeNormal(0.9425f, 0.1435f, 0.3018f);
    Model* ball = m_models[0];
    Model* chair = m_models[1];
    Model* mesa = m_models[2];
    Model* mirror = m_models[3];
    Model* plane = m_models[4];

    //Ciclo de Renderizado
    glEnable(GL_DEPTH_TEST); 
    glDisable(GL_CULL_FACE);

    m_cameraMovement = false;
    while(!glfwWindowShouldClose(m_window->getWindow())){
        // Compute time  
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window->getWindow(), true);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            delete m_camera;
            m_camera = new OrbitCamera(glm::vec3(6.55f,5.27f,1.21f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
        }

        //Calculos para la nueva cámara dentro del espejo
        glm::vec3 camPos = m_camera->getPos();
        glm::vec3 camCoi = m_camera->getCoi();
        glm::vec3 camUp = m_camera->getUp();

        glm::vec3 refPos = reflectPoint(camPos, planePoint, planeNormal);
        glm::vec3 refTarget = reflectPoint(camCoi, planePoint, planeNormal);
        glm::vec3 refUp = reflectPoint(camUp, glm::vec3(0.0f), planeNormal);

        // la matriz de vista dentro el espejo
        glm::mat4 mirrorView = glm::lookAt(refPos, refTarget, refUp);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glViewport(0, 0, 1280, 720);  
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ball->renderModel(mirrorView, m_projection, true);
        chair->renderModel(mirrorView, m_projection, true);
        mesa->renderModel(mirrorView, m_projection, true);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window->getWindow(), &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        glFrontFace(GL_CCW);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ball->renderModel(m_view, m_projection,true);
        chair->renderModel(m_view, m_projection,true);
        mesa->renderModel(m_view, m_projection,true);
        mirror->renderModel(m_view, m_projection,true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        plane->renderModel(m_view, m_projection,false);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            ball->updateModel(0.02f);
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        {
            ball->updateModel(-0.02f);
        }
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