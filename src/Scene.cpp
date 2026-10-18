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
    Model* axes = new Axes(axesShader, 1.5f);
    m_models.push_back(axes);

    
    // m_models[1]: único OBJ cargado al inicio (bunny)
    Model* obj = new ImportedModel(m_shaderProgram, "obj/bunny.obj");
    m_models.push_back(obj);

    //Matriz de vista View Matrix
    //Posicion de la camara, centro de atencion, vector perpendicular de la Cámara
    m_camera = new OrbitCamera(glm::vec3(2.0f,1.2f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));
    m_view =  m_camera->getMatrix();
    //Matriz de projeccion
    m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRation(), 0.1f, 100.0f);
}

void Scene::render()  {

    Model* axes   = m_models[0];


    // Archivos OBJ disponibles (tecla 1 a 6)
    const char* archivosOBJ[] = {
        "obj/bunny.obj",   // tecla 1
        "obj/car.obj",     // tecla 2
        "obj/casa.obj",    // tecla 3
        "obj/cat.obj",     // tecla 4
        "obj/pescado.obj", // tecla 5
        "obj/Teapot.obj"   // tecla 6
    };
    const int NUM_OBJ = 6;

    // Modelos ya cargados (nullptr = todavía no se ha cargado)
    Model* cargados[NUM_OBJ] = {nullptr};
    cargados[0] = m_models[1];     // el bunny ya se cargó en init()

    Model* model1 = cargados[0];   // modelo que se muestra

    //Ciclo de Renderizado
    glEnable(GL_DEPTH_TEST);  // Prueba de profundidad
    glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    glCullFace(GL_BACK);      // Culling de caras traseras
    glFrontFace(GL_CCW);      // Las caras frontales son las que tienen vértices en sentido antihorario

    GLenum primitiva = GL_TRIANGLES; //primitiva default
    m_cameraMovement = false;
    while(!glfwWindowShouldClose(m_window->getWindow())){
        // Compute time  
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window->getWindow(), true);

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            delete m_camera;
            m_camera = new OrbitCamera(glm::vec3(2.0f,1.2f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));        }

        // Selección del modelo con las teclas 1-6 (carga solo la primera vez)
        for (int i = 0; i < NUM_OBJ; ++i) {
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_1 + i) == GLFW_PRESS) {
                if (cargados[i] == nullptr) {
                    cargados[i] = new ImportedModel(m_shaderPrograms[0], archivosOBJ[i]);
                    m_models.push_back(cargados[i]);
                }
                model1 = cargados[i];
            }
        }
        
        //opciones para primitivas
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) primitiva = GL_POINTS;
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_L) == GLFW_PRESS) primitiva = GL_LINES;
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_T) == GLFW_PRESS) primitiva = GL_TRIANGLES;


        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // se dibujan los modelos
        axes->renderModel(m_view, m_projection, GL_LINES); //primitiva para vertices
        model1->renderModel(m_view, m_projection,primitiva);

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