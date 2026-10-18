#ifndef MODEL_IMPORTER_H
#define MODEL_IMPORTER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class ModelImporter {
private:
    // Estructura de almacenamiento interno (procesado)
    std::vector<float> triangleVerts;

    // Vector temporal para lectura del archivo .obj
    std::vector<float> vertVals;

    // Bounding box / Rangos
    std::vector<float> max;
    std::vector<float> min;

    // Metodos auxiliares
    int resolveIndex(int idx, int size);

public:
    ModelImporter();
    ~ModelImporter() = default;

    // Carga y parseo del archivo .obj
    void parseOBJ(const char* filePath);

    // Getters
    int getNumVertices();
    std::vector<float> getVertices();
    std::vector<float> getMin();
    std::vector<float> getMax();
};

#endif // MODEL_IMPORTER_H