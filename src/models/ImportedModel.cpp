#include "models/ImportedModel.h"

void ImportedModel::initGeometry(){
    ModelImporter modelImporter = ModelImporter();
    modelImporter.parseOBJ(n_filePath);
    numVertices = modelImporter.getNumVertices();
    n_vertices = modelImporter.getVertices();
    max = modelImporter.getMax();
    min = modelImporter.getMin();

    // Calcula para el bounding box
    glm::vec3 centro = glm::vec3((max[0]+min[0])*0.5f, (max[1]+min[1])*0.5f, (max[2]+min[2])*0.5f);
    glm::vec3 dimension = glm::vec3((max[0]-min[0]), (max[1]-min[1]), (max[2]-min[2]));
    float escalaFactor = 1.0f/(float)(std::max({dimension.x, dimension.y, dimension.z}));
    glm::mat4 escalamiento = glm::scale(glm::mat4(1.0f), glm::vec3(escalaFactor));

    m_model_mat = glm::translate(escalamiento, -centro);
}

void ImportedModel::init(){
    glGenVertexArrays(1,&VAO);
    glGenBuffers(3, n_VBO);

    glBindVertexArray(VAO);

    if(n_vertices.size()>0){
        glBindBuffer(GL_ARRAY_BUFFER, n_VBO[0]); 
        glBufferData(GL_ARRAY_BUFFER, n_vertices.size() * sizeof(float), &n_vertices[0], GL_STATIC_DRAW); 
    } else {
        std::cerr<<"VERTICES ERROR"<<std::endl;
    }
}

void ImportedModel::renderModel(const glm::mat4& view, const glm::mat4& projection){    
    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, n_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);    
}

void ImportedModel::updateModel(float deltaTime ){  
    (void)deltaTime;     
}

void ImportedModel::finish(){
}