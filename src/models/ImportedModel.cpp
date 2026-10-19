#include "models/ImportedModel.h"
#include <vector>

void ImportedModel::printModelInfo(){
    // Extraer solo el nombre del archivo de la ruta
    std::string pathStr(n_filePath);
    size_t lastSlash = pathStr.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? pathStr : pathStr.substr(lastSlash + 1);

    std::cout << "========================================" << std::endl;
    std::cout << "      INFORMACIÓN DEL MODELO OBJ        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Nombre del archivo : " << fileName << std::endl;
    std::cout << "  Número de vértices : " << numVertices << std::endl;
    std::cout << "  Mínimos (X, Y, Z) : [" << min[0] << ", " << min[1] << ", " << min[2] << "]" << std::endl;
    std::cout << "  Máximos (X, Y, Z) : [" << max[0] << ", " << max[1] << ", " << max[2] << "]" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
}

void ImportedModel::initGeometry(){
    ModelImporter modelImporter = ModelImporter();
    modelImporter.parseOBJ(n_filePath);
    numVertices = modelImporter.getNumVertices();
    n_vertices = modelImporter.getVertices();
    max = modelImporter.getMax();
    min = modelImporter.getMin();

    printModelInfo();

    // Calcula para el bounding box
    glm::vec3 centro = glm::vec3((max[0]+min[0])*0.5f, (max[1]+min[1])*0.5f, (max[2]+min[2])*0.5f);
    glm::vec3 dimension = glm::vec3((max[0]-min[0]), (max[1]-min[1]), (max[2]-min[2]));
    float escalaFactor = 1.0f/(float)(std::max({dimension.x, dimension.y, dimension.z}));
    glm::mat4 escalamiento = glm::scale(glm::mat4(1.0f), glm::vec3(escalaFactor));

    m_model_mat = glm::translate(escalamiento, -centro);
}

void ImportedModel::init(){

    /*Los modelos importados se guardan como una lista de triángulos sin índices.
    Para dibujarlos con GL_LINES hace falta indicar qué pares de vértices forman 
    cada arista, así que en init se construye un EBO con las tres aristas de cada 
    triángulo (a‑b, b‑c, c‑a). En renderModel, según la primitiva recibida, 
    se dibuja con glDrawElements (líneas) o glDrawArrays (puntos y triángulos). */
    glGenVertexArrays(1,&VAO);
    glGenBuffers(3, n_VBO);
    glGenBuffers(1, &EBO);    // crea el EBO (heredado de Model)

    glBindVertexArray(VAO);

    if(n_vertices.size()>0){
        glBindBuffer(GL_ARRAY_BUFFER, n_VBO[0]); 
        glBufferData(GL_ARRAY_BUFFER, n_vertices.size() * sizeof(float), &n_vertices[0], GL_STATIC_DRAW); 
        
        //índices de aristas a partir de los triángulos.
        // Cada triángulo t usa los vértices (3t, 3t+1, 3t+2) y genera
        // 3 aristas: a-b, b-c, c-a
        std::vector<GLuint> lineIdx;
        lineIdx.reserve(numVertices * 2);
        for(int t = 0; t < numVertices / 3; ++t){
            GLuint a = 3*t, b = 3*t + 1, c = 3*t + 2;
            lineIdx.insert(lineIdx.end(), {a, b,  b, c,  c, a});
        }

        // sube los índices al EBO (queda asociado al VAO activo)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIdx.size() * sizeof(GLuint),
                     lineIdx.data(), GL_STATIC_DRAW);
    
    } else {
        std::cerr<<"VERTICES ERROR"<<std::endl;
    }
}

void ImportedModel::renderModel(const glm::mat4& view, const glm::mat4& projection, GLenum primitive){    
    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, n_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    if(primitive == GL_LINES){
        //
        glDrawElements(GL_LINES, numVertices * 2, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(primitive, 0, numVertices);   // GL_POINTS o GL_TRIANGLES
    }
}

void ImportedModel::updateModel(float deltaTime, const glm::vec3& axis){
    // Sin tecla presionada no hay rotación
    if (axis == glm::vec3(0.0f)) return;

    // deltaTime se usa como ángulo en grados por frame
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime), axis);

    // Se multiplica por la IZQUIERDA para rotar sobre los ejes del mundo.
    // El modelo ya está centrado en el origen (initGeometry), así que gira sobre sí mismo.
    m_model_mat = rotacion * m_model_mat;
}

void ImportedModel::finish(){
}