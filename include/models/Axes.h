#ifndef AXES_H
#define AXES_H

#include <vector>
#include <glm/glm.hpp>
#include "Model.h"

/**
 * @class Axes
 * @brief Clase para modelar los ejes coordenados (X, Y, Z)
 */
class Axes : public Model
{
private:
    // Coleccion de vertices que contienen (x,y,z,r,g,b)
    GLfloat m_vertices[36];
    // Coleccion de indices
    GLuint m_indexes[6];  
    // Tamanio de cada eje
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
    void renderModel(const glm::mat4& view, const glm::mat4& projection, int primitive, bool printInfo) override;
    void updateModel(float deltaTime, const glm::vec3& axis) override;
    void finish() override;
};

#endif