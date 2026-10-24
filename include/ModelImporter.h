#ifndef MODEL_IMPORTER_H
#define MODEL_IMPORTER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @class ModelImporter
 * @brief Clase que realiza el parser de un archivo .obj para extraer datos geometricos,
 *        resuelve referencias de los indices de las caras y obtiene sus valores maximos
 */
class ModelImporter 
{
private:
    // Coleccion de las posiciones (x,y,z) de cada vertice
    std::vector<float> m_triangleVertices;
    // Coleccion temporal de las posiciones de cada vertice
    std::vector<float> m_tempVertices;
    // Maximos valores de (x, y, z)
    std::vector<float> m_max;
    // Minimos valores de (x, y, z)
    std::vector<float> m_min;

    /**
     * @brief Resuelve los valores de los indices negativos de las caras
     * 
     * @param index Indice de una cara
     * @param num  Total de vertices 
     * @return Indice resuelto
     */
    int resolveIndex(int index, int num);

public:
    /**
     * @brief Constructor de ModelImporter
     */
    ModelImporter();

    /**
     * @brief Destructor de ModelImporter
     */
    ~ModelImporter() = default;

    /**
     * @brief Obtiene un archivo .obj, extrayendo las coordenadas de los vertices y 
     *        las conexiones entre las caras, actualizando los valores maximos y minimos
     * 
     * @param filePath Ruta del archivo .obj
     */
    void parseOBJ(const char* filePath);

    /**
     * @brief Devuelve la cantidad de vertices
     * 
     * @return Cantidad de vertices
     */
    int getNumVertices();

    /**
     * @brief Devuelve una coleccion de vertices
     *
     * @return Coleccion de vertices leidos
     */
    std::vector<float> getVertices();
    
    /**
     * @brief Devuelve los valores minimos (x,y,z)
     *
     * @return Valores minimos (x,y,z)
     */
    std::vector<float> getMin();

    /**
     * @brief Devuelve los valores maximos (x,y,z)
     *
     * @return Valores maximos (x,y,z)
     */
    std::vector<float> getMax();
};

#endif