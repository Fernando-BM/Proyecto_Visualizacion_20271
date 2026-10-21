#ifndef IMPORTED_MODEL_H
#define IMPORTED_MODEL_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Model.h"
#include "ModelImporter.h"

#define num_VBOs 3

class ImportedModel: public Model{
private:
    const char * n_filePath;
    int numVertices;

    GLuint n_VBO[num_VBOs];

    std::vector<float> n_vertices;

    // Arreglos para la bounding box
    std::vector<float> max;
    std::vector<float> min;

    // Bandera para controlar la impresion
    bool m_infoPrinted = false;
    double m_lastRenderTime = 0.0;

public:
    ImportedModel(ShaderProgram* program, const char *filePath)
        : Model(program),
        n_filePath{filePath}
    {
        initGeometry();

        init();
    }

    void initGeometry() override;

    void init() override;

    void renderModel(const glm::mat4& view, const glm::mat4& projection, GLenum primitive) override;

    void updateModel(float deltaTime, const glm::vec3& axis) override;

    void finish() override;

    void printModelInfo();
};

#endif // IMPORTED_MODEL_H