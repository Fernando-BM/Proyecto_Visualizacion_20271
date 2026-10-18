#ifndef AXES_H
#define AXES_H

#include <vector>
#include <glm/glm.hpp>
#include "Model.h"

/**
 * @class Axes
 * @brief Eje coordenado (X rojo, Y verde, Z azul) dibujado con GL_LINES
 */
class Axes : public Model
{
private:
    std::vector<GLfloat> m_vertices;  // x,y,z,r,g,b por vértice
    std::vector<GLuint>  m_indices;   // cada pareja = una arista
    float m_length;

public:
    Axes(ShaderProgram* program, float length = 3.0f)
        : Model(program), m_length{length}
    {
        initGeometry();
        init();
    }

    void initGeometry() override;
    void init() override;
    void renderModel(const glm::mat4& view, const glm::mat4& projection) override;
    void updateModel(float deltaTime) override;
    void finish() override;
};

#endif