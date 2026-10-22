#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/glm.hpp>
#include "Model.h"

class Grid : public Model
{
private:
    // Estructura auxiliar para los vertices de la malla
    struct Vertex {
        glm::vec3 position;

        void InitVertex(float x, float y, float z);        
    };

    // Dimensiones de la malla
    int m_width;
    int m_depth;
    float m_scale;
    float m_dominio;

    // Rangos para altura
    float m_y_min;
    float m_y_max;

    // Buffers de OpenGL
    GLuint VBO;
    GLuint EBO;    

    void InitVertices(std::vector<Vertex>& vertices);
    void InitIndices(std::vector<uint>& indices);    

public:
    Grid(ShaderProgram* program, int width = 100, int depth = 100, float scale = 3.0f, float dominio = 5.0f)
        : Model(program), m_width{width}, m_depth{depth}, m_scale{scale}, m_dominio{dominio}
    {
        init();
        initGeometry();        
    }

    void initGeometry() override;
    void init() override;
    void renderModel(const glm::mat4& view, const glm::mat4& projection, GLenum primitive,bool print_info) override;
    void updateModel(float deltaTime, const glm::vec3& axis) override;
    void finish() override;
};

#endif