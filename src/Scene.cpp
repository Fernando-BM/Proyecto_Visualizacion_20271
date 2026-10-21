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
  
    // Inicializacion de ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

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
    obj = new ImportedModel(m_shaderProgram, "obj/car.obj");
    m_models.push_back(obj);
    obj = new ImportedModel(m_shaderProgram, "obj/casa.obj");
    m_models.push_back(obj);
    obj = new ImportedModel(m_shaderProgram, "obj/cat.obj");
    m_models.push_back(obj);
    obj = new ImportedModel(m_shaderProgram, "obj/pescado.obj");
    m_models.push_back(obj);
    obj = new ImportedModel(m_shaderProgram, "obj/Teapot.obj");
    m_models.push_back(obj);

    //Push en m_model posicion 1 la malla
    Model* grid = new Grid(m_shaderProgram, 1000, 1000, 0.3f, 20.0f);
    m_models.push_back(grid);


    //Matriz de vista View Matrix
    //Posicion de la camara, centro de atencion, vector perpendicular de la Cámara
    m_camera = new OrbitCamera(glm::vec3(2.0f,1.2f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));
    m_view =  m_camera->getMatrix();
    //Matriz de projeccion
    int height, width;
    glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
    float contentScaleX, contentScaleY;
    glfwGetWindowContentScale(m_window->getWindow(), &contentScaleX, &contentScaleY);
    m_uiWidth = 320.0f;
    m_panelWidth = (int)(m_uiWidth * contentScaleX);
    float viewPortWidth = (float)(width - m_panelWidth);
    float viewPortHeight = (float)height;
    m_viewPortWidth = (int)viewPortWidth;
    m_viewPortHeight = (int)viewPortHeight;
    //m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRation(), 0.1f, 100.0f);
    m_projection = glm::perspective(glm::radians(45.0f), viewPortWidth/viewPortHeight, 0.1f, 100.0f);
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
    //glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    glCullFace(GL_BACK);      // Culling de caras traseras
    glFrontFace(GL_CCW);      // Las caras frontales son las que tienen vértices en sentido antihorario
    
    GLenum primitiva = GL_TRIANGLES; //primitiva default
    glm::vec3 ejeRotacion(0.0f);
    int render_mode = 0;
    int model_select = 0;
    m_cameraMovement = false;

    const char* model_names[] = {"Conejo", "Carro", "Casa", "Gato", "Pescado", "Tetéra", "Función"};

    while(!glfwWindowShouldClose(m_window->getWindow())){
        // Compute time  

        // GUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(m_uiWidth, viewport->WorkSize.y));
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::Begin("Panel de control", nullptr, window_flags);
        ImGui::Text("Panel de herramientas");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Modelo:");
        ImGui::SameLine();
                
        if(ImGui::Combo("##Modelo", &model_select, model_names, IM_ARRAYSIZE(model_names))){            
            model1 = m_models[model_select+1];
        }
        ImGui::Separator();
        ImGui::Text("Modo de dibujo:");
        if(ImGui::RadioButton("Triángulos", &render_mode, 0)){
            primitiva = GL_TRIANGLES;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Líneas", &render_mode, 0)){
            primitiva = GL_LINES;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Puntos", &render_mode, 0)){
            primitiva = GL_POINTS;
        }
        ImGui::Separator();
        ImGui::Text("Modo de dibujo:");
        ImGui::Button("Eje X");        
        if(ImGui::IsItemActive()){ 
            ejeRotacion.x = 1.0f;
        } else if(ImGui::IsItemDeactivated()){
            ejeRotacion.x = 0.0f;
        }
        ImGui::Button("Eje Y");
        if(ImGui::IsItemActive()){ 
            ejeRotacion.y = 1.0f;
        } else if(ImGui::IsItemDeactivated()){
            ejeRotacion.y = 0.0f;
        }
        ImGui::Button("Eje Z");
        if(ImGui::IsItemActive()){ 
            ejeRotacion.z = 1.0f;
        } else if(ImGui::IsItemDeactivated()){
            ejeRotacion.z = 0.0f;
        }
        ImGui::End();

        glViewport(m_panelWidth, 0, m_viewPortWidth, m_viewPortHeight);
        
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        if(!io.WantCaptureKeyboard){
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
                        cargados[i] = new ImportedModel(m_shaderPrograms[1], archivosOBJ[i]);
                        m_models.push_back(cargados[i]);
                    }
                    model1 = cargados[i];
                }
            }

            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_7) == GLFW_PRESS) model1 = m_models[7];

            //opciones key para primitivas
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) primitiva = GL_POINTS;
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_L) == GLFW_PRESS) primitiva = GL_LINES;
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_T) == GLFW_PRESS) primitiva = GL_TRIANGLES;

                         
            // Rotación: mantener X, Y o Z presionada para rotar sobre ese eje
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_X) == GLFW_PRESS){ 
                ejeRotacion.x = 1.0f;
            } else {
                ejeRotacion.x = 0.0f;
            }
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Y) == GLFW_PRESS){ 
                ejeRotacion.y = 1.0f;
            } else {
                ejeRotacion.y = 0.0f;
            }
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Z) == GLFW_PRESS){ 
                ejeRotacion.z = 1.0f;
            } else {
                ejeRotacion.z = 0.0f;
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // se dibujan los modelos
        axes->renderModel(m_view, m_projection, GL_LINES); //primitiva para vertices
        model1->renderModel(m_view, m_projection,primitiva);
        

        axes->updateModel(0.7f, glm::vec3(0.0f));  
        model1->updateModel(0.7f, ejeRotacion);                

        if (m_cameraMovement)
        {
            double currX,currY = 0;
            glfwGetCursorPos(m_window->getWindow(),&currX,&currY);
            m_current = glm::vec2((float)currX,(float)currY);
            m_camera->rotate(m_last,m_current);
            m_camera->setPos(m_last,m_current);
        }
        m_view = m_camera->getMatrix();

        glViewport(0, 0, 1280, 720);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // update models 
        glfwSwapBuffers(m_window->getWindow());
        glfwPollEvents();
    }
}