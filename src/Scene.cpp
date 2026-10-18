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
        instance->m_cameraMovement = true;
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        instance->m_cameraMovement = false;
    }
}

void Scene::init(){
    //Inicializacion de ventana 
    m_window = new WindowGL();
    glfwSetWindowUserPointer(m_window->getWindow(), this);
    glfwSetMouseButtonCallback(m_window->getWindow(),Scene::mouseCallback);
  


    // Se inicializan shader de ejes y shader de modelos
    ShaderProgram* axesShader = new ShaderProgram("shaders/axes_vertex_shader.glsl",
                                                    "shaders/axes_frag_shader.glsl");
    m_shaderPrograms.push_back(axesShader);

    ShaderProgram* m_shaderProgram = new ShaderProgram("shaders/vertex_shader.glsl",
                                                        "shaders/frag_shader.glsl");
    m_shaderPrograms.push_back(m_shaderProgram);

    
    //Push en m_model posicion 0 los ejes
    Model* axes = new Axes(axesShader, 3.0f);
    m_models.push_back(axes);

    
    // Push en m_model posicion 1-6 los modelos
    const char* archivosOBJ[] = {
        "obj/bunny.obj",   // tecla 1
        "obj/car.obj",     // tecla 2
        "obj/casa.obj",    // tecla 3
        "obj/cat.obj",     // tecla 4
        "obj/pescado.obj", // tecla 5
        "obj/Teapot.obj"   // tecla 6
    };
    for (const char* archivo : archivosOBJ) {
        Model* obj = new ImportedModel(m_shaderProgram, archivo);
        m_models.push_back(obj);
    }

    //Matriz de vista View Matrix
    //Posicion de la camara, centro de atencion, vector perpendicular de la Cámara
    m_camera = new OrbitCamera(glm::vec3(7.0f,4.0f,7.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
    m_view =  m_camera->getMatrix();
    //Matriz de projeccion
    m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRation(), 0.1f, 100.0f);
}

void Scene::render()  {

    Model* axes   = m_models[0];
    //Modelo inicial   
    Model* model1 = m_models[1]; 

    //Ciclo de Renderizado
    glEnable(GL_DEPTH_TEST);  // Prueba de profundidad
    glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    glCullFace(GL_BACK);      // Culling de caras traseras
    glFrontFace(GL_CCW);      // Las caras frontales son las que tienen vértices en sentido antihorario

    m_cameraMovement = false;
    while(!glfwWindowShouldClose(m_window->getWindow())){
        // ESC 
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window->getWindow(), true);
        //ENTER
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            delete m_camera;
            m_camera = new OrbitCamera(glm::vec3(7.0f,4.0f,7.0f), glm::vec3(0.0f,0.5f,0.0f), glm::vec3(0.0,1.0,0.0));
        }

        // Selección del modelo con las teclas 1-6
        for (size_t i = 1; i < m_models.size() && i <= 9; ++i) {
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_1 + (int)(i - 1)) == GLFW_PRESS)
                model1 = m_models[i];
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // se dibujan los modelos
        axes->renderModel(m_view, m_projection);
        model1->renderModel(m_view, m_projection);

        // update models 
        glfwSwapBuffers(m_window->getWindow());
        glfwPollEvents();

        axes->updateModel(0.7f);
        model1->updateModel(0.7f);
        
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