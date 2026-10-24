#ifndef IMPORTED_MODEL_H
#define IMPORTED_MODEL_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "Model.h"
#include "ModelImporter.h"

/**
 * @class ImportedModel
 * @brief Clase para modelar un modelo OBJ
 */
class ImportedModel: public Model{
private:
    // Ruta del archivo obj
    const char * m_filePath;    
    // Vertices recuperados
    std::vector<float> m_vertices;
    // Valores maximos (x,y,z)
    std::vector<float> m_max;
    // Valores minimos (x,y,z)
    std::vector<float> m_min;
    // Cantidad de vertices
    int numVertices;    
    // Bandera para controlar la impresion de la informacion
    bool m_infoPrinted = false;
    // Ultimo registro de tiempo de renderizado
    double m_lastRenderTime = 0.0;

public:
    ImportedModel(ShaderProgram* program, const char *filePath)
        : Model(program), m_filePath{filePath}
    {
        initGeometry();
        init();
    }

    void initGeometry() override;
    void init() override;
    void renderModel(const glm::mat4& view, const glm::mat4& projection, int primitive, bool printInfo) override;
    void updateModel(float deltaTime, const glm::vec3& axis) override;
    void finish() override;

    /**
     * @brief Imprime la informacion del archivo obj (nombre, numero de vertices, maximos (x,y,z) y minimos (x,y,z))
     */
    void printModelInfo();
};

#endif