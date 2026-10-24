#include "models/ImportedModel.h"

void ImportedModel::printModelInfo(){
    // Extrae solo el nombre del archivo de la ruta
    std::string pathStr(m_filePath);
    size_t lastSlash = pathStr.find_last_of("/\\");
    std::string fileName = (lastSlash == std::string::npos) ? pathStr : pathStr.substr(lastSlash + 1);

    std::cout << "========================================" << std::endl;
    std::cout << "      INFORMACIÓN DEL MODELO OBJ        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Nombre del archivo : " << fileName << std::endl;
    std::cout << "  Número de vértices : " << numVertices << std::endl;
    std::cout << "  Mínimos (X, Y, Z) : [" << m_min[0] << ", " << m_min[1] << ", " << m_min[2] << "]" << std::endl;
    std::cout << "  Máximos (X, Y, Z) : [" << m_max[0] << ", " << m_max[1] << ", " << m_max[2] << "]" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
}

void ImportedModel::initGeometry(){
    ModelImporter modelImporter = ModelImporter();
    // Parser del archivo obj
    modelImporter.parseOBJ(m_filePath);

    numVertices = modelImporter.getNumVertices();
    m_vertices = modelImporter.getVertices();
    m_max = modelImporter.getMax();
    m_min = modelImporter.getMin();

    // Calcula bounding box
    glm::vec3 center = glm::vec3((m_max[0] + m_min[0]) * 0.5f, (m_max[1] + m_min[1]) * 0.5f, (m_max[2] + m_min[2]) * 0.5f);
    glm::vec3 dimension = glm::vec3((m_max[0] - m_min[0]), (m_max[1] - m_min[1]), (m_max[2] - m_min[2]));
    float scaleFactor = 1.0f/(float)(std::max({dimension.x, dimension.y, dimension.z}));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

    // Centra el modelo al origen y aplica escalamiento
    m_model_mat = glm::translate(scaleMatrix, -center);
}

void ImportedModel::init(){

    // Genera identificadores
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);

    // Vincula VAO
    glBindVertexArray(VAO);

    if(m_vertices.size() > 0)
    {
        // Carga los vertices en el VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);                 
    
    } else {
        std::cerr << "[ImportedModel Error] Buffer de vertices esta vacío: " << m_filePath <<std::endl;
    }

    // Desvincula VAO
    glBindVertexArray(0);
}

void ImportedModel::renderModel(const glm::mat4& view, const glm::mat4& projection, int primitive, bool printInfo){        
    // Imprime la informacion del archivo obj
    if (printInfo) 
    {
        printModelInfo();
    }  

    // Activa programa del shader
    m_shaderProgram->use();

    // Carga matriz de modelo
    m_shaderProgram->setMat4x4("model", m_model_mat);
    // Carga matriz de vista
    m_shaderProgram->setMat4x4("view", view);
    // Carga matriz de proyeccion
    m_shaderProgram->setMat4x4("projection", projection);
    // Carga el valor minimo y
    m_shaderProgram->setFloat("minY", m_min[1]);
    // Carga el valor maximo y
    m_shaderProgram->setFloat("maxY", m_max[1]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Pinta modelo obj con una primitiva (triangulos, lineas o puntos)
    glDrawArrays(primitive, 0, numVertices);    

    glBindVertexArray(0);
}

void ImportedModel::updateModel(float deltaTime, const glm::vec3& axis)
{
    // Sin tecla presionada no hay rotacion
    if (axis == glm::vec3(0.0f)) return;

    // deltaTime se usa como angulo en grados por frame
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime), axis);

    // Multiplica por la izquierda para rotar sobre los ejes, girando sobre si mismo
    m_model_mat = rotacion * m_model_mat;
}

void ImportedModel::finish()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


