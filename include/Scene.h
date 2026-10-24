#ifndef SCENE_H 
#define SCENE_H

#include <GL/glew.h>  
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
// Bibliotecas para interfaz de usuario
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "models/ImportedModel.h"
#include "models/Axes.h"
#include "models/Grid.h"
#include "Model.h"
#include "ModelImporter.h"
#include "ShaderProgram.h"
#include "WindowGL.h"
#include "OrbitCamera.h"

/**
 * @class Scene
 * @brief Clase para definir una escena que contiene una ventana, shaders,
 *        modelos y la interfaz de usuario
 */
class Scene
{
private:
    // La ventana donde se crea la escena
    WindowGL* m_window; 
    // Coleccion de shaders para los diferentes modelos
    std::vector<ShaderProgram*>  m_shaderPrograms;
    // Coleccion de modelos
    std::vector<Model*> m_models;

    // Camara orbital
    OrbitCamera* m_camera;
    // Matriz de la vista
    glm::mat4 m_view;
    // Matriz de proyeccion
    glm::mat4 m_projection; 

    // Ultimas coordenadas del cursor del mouse (x,y)
    glm::vec2 m_last;
    // Coordenadas actuales del cursor del mouse (x,y)
    glm::vec2 m_current;
    // Estado de la camara si esta en movimiento
    bool m_cameraMovement;    

    // Anchura de la interfaz de usuario
    float m_uiWidth;
    // Anchura del panel lateral
    int m_panelWidth;
    // Anchura del marco de renderizado
    int m_viewPortWidth;
    // Altura del marco de renderizado
    int m_viewPortHeight;

    /**
     * @brief Inicializa la configuracion 
     */
    void init();    

public:
    /**
     * @brief Constructor de escena y lo inicializa
     */
    Scene()
    {
        init();
    }

    /**
     * @brief Destructor de escena
     */
    ~Scene()
    {
        delete m_window;
        for (ShaderProgram* sh:m_shaderPrograms) delete sh;
    }

    /**
     * @brief Ciclo de renderizado general 
     */
    void render(); 

    /**
     * @brief Gestiona el movimiento de la camara a partir de los botones del mouse
     * 
     * @param window Puntero al contexto de la ventana
     * @param button Boton del mouse
     * @param action Accion que se lleva a cabo con el mouse
     * @param mods Bits de las teclas modificadoras
     */
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
};

#endif