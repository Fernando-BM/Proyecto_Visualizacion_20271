#include "ModelImporter.h"

ModelImporter::ModelImporter(){}

int ModelImporter::resolveIndex(int index, int num){
    if(index > 0) // Indice positivo
    {
        return index-1;
    } else if(index < 0) // Indice negativo
    {
        return num + index; 
    }
    return -1;
}

void ModelImporter::parseOBJ(const char *filePath){    
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream)
    {
        std::cerr << "[ModelImporter Error] No se pudo abrir archivo OBJ: " << filePath << std::endl;
        return;
    }

    std::string line;
    float posX, posY, posZ;
    while(std::getline(fileStream, line))
    {               
        if(line.compare(0, 2, "v ") == 0) // Parser de los vertices
        {
            std::stringstream ss(line.erase(0, 2));
            ss >> posX >> posY >> posZ;
            m_tempVertices.push_back(posX);
            m_tempVertices.push_back(posY);
            m_tempVertices.push_back(posZ);
        } else if(line.compare(0, 2, "f ") == 0) // Parser de las caras
        {
            std::stringstream ss(line.erase(0, 2));
            std::string cornerToken, indVertex;
            std::vector<int> faceVertexIndexes;

            // Extrae todas las esquinas de la cara
            while(std::getline(ss, cornerToken, ' '))
            {
                if(cornerToken.empty()) continue;             

                std::stringstream cornerSS(cornerToken);
                std::getline(cornerSS, indVertex, '/');             

                faceVertexIndexes.push_back(resolveIndex(std::stoi(indVertex), m_tempVertices.size() / 3));
            }

            // Triangula los datos para que se guarden con ese orden
            for(size_t i = 1; i < faceVertexIndexes.size() - 1; ++i)
            {                
                int triangleCornerIndices[3] = {0, (int)i, (int)(i+1)};

                for(int cornerIdx : triangleCornerIndices)
                {
                    int vIndex = faceVertexIndexes[cornerIdx];

                    m_triangleVertices.push_back(m_tempVertices[vIndex * 3]);
                    m_triangleVertices.push_back(m_tempVertices[vIndex * 3 + 1]);
                    m_triangleVertices.push_back(m_tempVertices[vIndex * 3 + 2]);                    
                }                                
            }       
        }        
    }  
    
    fileStream.close();
    
    m_min = {  1e9f,  1e9f,  1e9f };
    m_max = { -1e9f, -1e9f, -1e9f };

    // Actualiza los valores minimos y maximos
    for(int i = 0; i < (int)m_tempVertices.size()-1; i+=3)
    {        
        if(m_tempVertices[i] > m_max[0])
            m_max[0] = m_tempVertices[i];
        if(m_tempVertices[i+1] > m_max[1])
            m_max[1] = m_tempVertices[i+1];
        if(m_tempVertices[i+2] > m_max[2])
            m_max[2] = m_tempVertices[i+2];
        if(m_tempVertices[i] < m_min[0])
            m_min[0] = m_tempVertices[i];
        if(m_tempVertices[i+1] < m_min[1])
            m_min[1] = m_tempVertices[i+1];
        if(m_tempVertices[i+2] < m_min[2])
            m_min[2] = m_tempVertices[i+2];
    }
}

int ModelImporter::getNumVertices()
{ 
    return(m_triangleVertices.size()/3); 
}

std::vector<float> ModelImporter::getVertices()
{ 
    return m_triangleVertices; 
}

std::vector<float> ModelImporter::getMin()
{ 
    return m_min; 
}

std::vector<float> ModelImporter::getMax()
{ 
    return m_max; 
}