#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/glm.hpp>

#include "Model.h"

/**
 * @class Grid
 * @brief Clase para modelar una malla de superficie, aplicando la funcion 
 *        f(x, z) = cos(x) + sin(z)
 */
class Grid : public Model
{
private:
    /**
     * @struct Vertex
     * @brief Estructura de datos que contiene la posicion de un vertice
     */
    struct Vertex 
    {
        // Posicion del vertice
        glm::vec3 position;

        /**
         * @brief Asigna los valores (x,y,z) para lo posicion del vertice
         */
        void InitVertex(float x, float y, float z);        
    };

    // Numero de vertices a lo largo del eje X
    int m_width;
    // Numero de vertices a lo largo del eje Z
    int m_depth;
    // Escalamiento de resultados de la funcion
    float m_scale;
    // Limites del dominio
    float m_domain;
    // Valor minimo de Y
    float m_minY;
    // Valor maximo de Y
    float m_maxY;

    /**
     * @brief Evalua con la funcion un rango de valores (x,z) para obtener
     *        una altura y generar vertices 
     * 
     * @param vertices Almacena posiciones de los vertices generados
     */
    void InitVertices(std::vector<Vertex>& vertices);

    /**
     * @brief Genera indices para pintarlo con triangulos
     * 
     * @param indices Almacena indices de los triangulos esperados
     */
    void InitIndexes(std::vector<uint>& indices);    

public:
    Grid(ShaderProgram* program, int width = 100, int depth = 100, float scale = 3.0f, float domain = 5.0f)
        : Model(program), m_width{width}, m_depth{depth}, m_scale{scale}, m_domain{domain}
    {
        init();
        initGeometry();        
    }

    void initGeometry() override;
    void init() override;
    void renderModel(const glm::mat4& view, const glm::mat4& projection, int primitive, bool printInfo) override;
    void updateModel(float deltaTime, const glm::vec3& axis) override;
    void finish() override;
};

#endif