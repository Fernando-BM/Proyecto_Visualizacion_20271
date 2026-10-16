#ifndef IMPORTED_MODEL_H
#define IMPORTED_MODEL_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
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

    void renderModel(const glm::mat4& view, const glm::mat4& projection) override;

    void updateModel(float deltaTime) override;

    void finish() override;
};

#endif // IMPORTED_MODEL_H