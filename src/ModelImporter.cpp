#include "ModelImporter.h"

int ModelImporter::resolveIndex(int idx, int size){
    if(idx > 0){
        return idx-1;
    } else if(idx < 0) {
        return size + idx;
    }
    return -1;
}

ModelImporter::ModelImporter(){}

void ModelImporter::parseOBJ(const char *filePath){
    float x, y, z;
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream){
        std::cerr << "PROBLEM OPENING THE OBJ FILE" << std::endl;
        return;
    }

    std::string line;
    while(std::getline(fileStream, line)){               
        if(line.compare(0, 2, "v ") == 0){ // Vertices               
            std::stringstream ss(line.erase(0, 2));
            ss >> x >> y >> z;
            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);
        } else if(line.compare(0, 2, "f ") == 0){ // Caras            
            std::stringstream ss(line.erase(0, 2));
            std::string oneCorner, v;

            // Estructuras temporales para guardar los vertices de esta cara especifica
            std::vector<int> faceV;

            // 1. Leer todas las esquinas de la cara
            while(std::getline(ss, oneCorner, ' ')){
                if(oneCorner.empty()) continue;             

                std::stringstream oneCornerSS(oneCorner);
                std::getline(oneCornerSS, v, '/');             

                faceV.push_back(resolveIndex(std::stoi(v), vertVals.size() / 3));
            }

            // 2. Triangulacion (Triangle Fan) para meter los datos a los vectores principales
            for(size_t i = 1; i < faceV.size() - 1; ++i){                
                // Para cada triangulo, necesitamos 3 esquinas: indice 0, indice i, e indice i+1
                int indicesDeEsquina[3] = {0, (int)i, (int)(i+1)};

                for(int idx : indicesDeEsquina){
                    int vIndex = faceV[idx];

                    // Vertices
                    triangleVerts.push_back(vertVals[vIndex * 3]);
                    triangleVerts.push_back(vertVals[vIndex * 3 + 1]);
                    triangleVerts.push_back(vertVals[vIndex * 3 + 2]);                    
                }                                
            }       
        }        
    }    
    
    min = {  1e9f,  1e9f,  1e9f };
    max = { -1e9f, -1e9f, -1e9f };

    for(int i = 0; i < (int)vertVals.size()-1; i+=3){        
        if(vertVals[i] > max[0])
            max[0] = vertVals[i];
        if(vertVals[i+1] > max[1])
            max[1] = vertVals[i+1];
        if(vertVals[i+2] > max[2])
            max[2] = vertVals[i+2];
        if(vertVals[i] < min[0])
            min[0] = vertVals[i];
        if(vertVals[i+1] < min[1])
            min[1] = vertVals[i+1];
        if(vertVals[i+2] < min[2])
            min[2] = vertVals[i+2];
    }
}

// Getters
int ModelImporter::getNumVertices(){ return (triangleVerts.size() / 3); }
std::vector<float> ModelImporter::getVertices(){ return triangleVerts; }
std::vector<float> ModelImporter::getMin(){ return min; }
std::vector<float> ModelImporter::getMax(){ return max; }