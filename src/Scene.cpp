#include "Scene.h"

void Scene::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    Scene* instance = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    mods = 0;
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double currX,currY = mods;
        glfwGetCursorPos(window,&currX,&currY);
        instance->m_last = glm::vec2((float)currX,(float)currY);
        instance->m_cameraMovement = true;
    }   
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        instance->m_cameraMovement = false;
    }
}

void Scene::init()
{
    // Inicializa ventana 
    m_window = new WindowGL();
    glfwSetWindowUserPointer(m_window->getWindow(), this);
    glfwSetMouseButtonCallback(m_window->getWindow(),Scene::mouseCallback);
  
    // Inicializa ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // Inicializa shader de ejes 
    ShaderProgram* axesShader = new ShaderProgram("shaders/axes_vertex_shader.glsl", "shaders/axes_frag_shader.glsl");
    m_shaderPrograms.push_back(axesShader);

    // Inicializa shader de modelos
    ShaderProgram* m_shaderProgram = new ShaderProgram("shaders/vertex_shader.glsl", "shaders/frag_shader.glsl");
    m_shaderPrograms.push_back(m_shaderProgram);
    
    // Carga modelo de ejes
    Model* axes = new Axes(axesShader, 1.5f);
    m_models.push_back(axes);    
    
    // Carga modelos obj
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

    // Carga modelo de la funcion
    Model* grid = new Grid(m_shaderProgram, 1000, 1000, 0.3f, 20.0f);
    m_models.push_back(grid);

    // Posicion de la camara, centro de atencion y vector perpendicular de la camara
    m_camera = new OrbitCamera(glm::vec3(2.0f,1.2f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));
    // Matriz de vista
    m_view =  m_camera->getMatrix();
    
    // Tamanio del buffer de la ventana
    int height, width;
    glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
    // Escala de contenido de la ventana
    float contentScaleX, contentScaleY;
    glfwGetWindowContentScale(m_window->getWindow(), &contentScaleX, &contentScaleY);
    // Ajustan la visualizacion de la interfaz de usuario
    m_uiWidth = 320.0f;
    m_panelWidth = (int)(m_uiWidth * contentScaleX);
    float viewPortWidth = (float)(width - m_panelWidth);
    float viewPortHeight = (float)height;
    m_viewPortWidth = (int)viewPortWidth;
    m_viewPortHeight = (int)viewPortHeight;
    // Matriz de proyeccion
    m_projection = glm::perspective(glm::radians(45.0f), viewPortWidth/viewPortHeight, 0.1f, 100.0f);
}

void Scene::render()
{
    // Modelo de ejes
    Model* axes = m_models[0];    
    // Model obj activo (default: conejo)
    Model* activeModel = m_models[1]; 
    // Eje de rotacion del modelo obj
    glm::vec3 rotationAxis(0.0f);
    // Primitiva seleccionada para pintar modelo (0:triangulos, 1:lineas, 2:puntos)
    int primitiveSelect = 0;
    // Primitiva para pintar modelo obj (default: triangulos)
    int renderMode = GL_TRIANGLES;
    // Modelo seleccionado (0:conejo, 1:carro, 2:casa, 3:gato, 4:pescado, 5:tetera)
    int modelSelect = 0;
    // Estado de la impresion de informacion del modelo obj
    bool printInfo = true;

    // Nombres de los modelos obj
    const char* modelNames[] = {"Conejo", "Carro", "Casa", "Gato", "Pescado", "Tetéra", "Función"};

    // Prueba de profundidad
    glEnable(GL_DEPTH_TEST); 
    // Culling de caras traseras
    glCullFace(GL_BACK);     
    // Las caras frontales con vertices en sentido antihorario
    glFrontFace(GL_CCW);     
                
    m_cameraMovement = false;    

    while(!glfwWindowShouldClose(m_window->getWindow()))
    {
        // Inicializa ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Configuracion del panel de la interfaz
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(m_uiWidth, viewport->WorkSize.y));

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        
        ImGui::Begin("Panel de control", nullptr, windowFlags);
        ImGui::Text("Panel de herramientas");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Modelo:");
        ImGui::SameLine();                
        if(ImGui::Combo("##Modelo", &modelSelect, modelNames, IM_ARRAYSIZE(modelNames)))
        {            
            activeModel = m_models[modelSelect+1];
            printInfo = true;
        }
        ImGui::Separator();

        ImGui::Text("Modo de dibujo:");
        if(ImGui::RadioButton("Triángulos", &primitiveSelect, 0))
        {
            renderMode = GL_TRIANGLES;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Líneas", &primitiveSelect, 0))
        {
            renderMode = GL_LINES;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Puntos", &primitiveSelect, 0))
        {
            renderMode = GL_POINTS;
        }
        ImGui::Separator();

        ImGui::Text("Modo de dibujo:");
        ImGui::Button("Eje X");        
        if(ImGui::IsItemActive())
        { 
            rotationAxis.x = 1.0f;
        } else if(ImGui::IsItemDeactivated())
        {
            rotationAxis.x = 0.0f;
        }
        ImGui::Button("Eje Y");
        if(ImGui::IsItemActive())
        { 
            rotationAxis.y = 1.0f;
        } else if(ImGui::IsItemDeactivated())
        {
            rotationAxis.y = 0.0f;
        }
        ImGui::Button("Eje Z");
        if(ImGui::IsItemActive())
        { 
            rotationAxis.z = 1.0f;
        } else if(ImGui::IsItemDeactivated())
        {
            rotationAxis.z = 0.0f;
        }
        ImGui::End();        
        
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
        if(!io.WantCaptureKeyboard)
        {
            // Cierra aplicacion
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_window->getWindow(), true);
            // Reinicia la posicion de la camara
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                delete m_camera;
                m_camera = new OrbitCamera(glm::vec3(2.0f,1.2f,2.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));        
            }
                
            // Seleccion de un modelo obj con las teclas 1-6 
            for(int i = 0; i < 7; ++i) 
            {
                if(glfwGetKey(m_window->getWindow(), GLFW_KEY_1 + i) == GLFW_PRESS && activeModel != m_models[i+1]) 
                {
                    activeModel = m_models[i+1];
                    printInfo = true;
                }
            }

            // Seleccion de modelo de la funcion con la tecla 7
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_7) == GLFW_PRESS) activeModel = m_models[7];

            // Seleccion de tipo de primitiva con las teclas (P:puntos, L:lineas, T:triangulos)
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) renderMode = GL_POINTS;
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_L) == GLFW_PRESS) renderMode = GL_LINES;
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_T) == GLFW_PRESS) renderMode = GL_TRIANGLES;
                         
            // Rotar modelo obj manteniendo las teclas X, Y o Z
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_X) == GLFW_PRESS)
            { 
                rotationAxis.x = 1.0f;
            } else {
                rotationAxis.x = 0.0f;
            }
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_Y) == GLFW_PRESS)
            { 
                rotationAxis.y = 1.0f;
            } else {
                rotationAxis.y = 0.0f;
            }
            if(glfwGetKey(m_window->getWindow(), GLFW_KEY_Z) == GLFW_PRESS)
            { 
                rotationAxis.z = 1.0f;
            } else {
                rotationAxis.z = 0.0f;
            }
        }

        // Limita la ventana de renderizado junto al panel de la interfaz de usuario
        glViewport(m_panelWidth, 0, m_viewPortWidth, m_viewPortHeight);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dibuja los modelos
        axes->renderModel(m_view, m_projection, GL_LINES, printInfo); 
        activeModel->renderModel(m_view, m_projection, renderMode, printInfo);
        
        // Actualiza los modelos
        axes->updateModel(0.7f, glm::vec3(0.0f));  
        activeModel->updateModel(0.7f, rotationAxis);                

        // Actualiza camara
        if(m_cameraMovement)
        {
            double currX,currY = 0;
            glfwGetCursorPos(m_window->getWindow(), &currX, &currY);
            m_current = glm::vec2((float)currX, (float)currY);
            m_camera->rotate(m_last, m_current);
            m_camera->setPos(m_last, m_current);
        }
        m_view = m_camera->getMatrix();

        //glViewport(0, 0, 1280, 720);
        glViewport(0, 0, m_panelWidth + m_viewPortWidth, m_viewPortHeight);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // update models 
        glfwSwapBuffers(m_window->getWindow());
        glfwPollEvents();
        
        printInfo = false;
    }
}