#include "models/Grid.h"

void Grid::initGeometry(){
    std::vector<Grid::Vertex> vertices;

    vertices.resize(m_width * m_depth);

    InitVertices(vertices);

    std::vector<unsigned int> indices;

    int numQuads = (m_width-1)*(m_depth-1);

    // Cada quad tiene 6 vertices
    indices.resize(numQuads*6);

    InitIndices(indices);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Escalar malla para calcularla en el update
    m_model_mat = glm::mat4(1.0f);
    m_model_mat = glm::translate(m_model_mat, glm::vec3(0.0f));
    m_model_mat = glm::scale(m_model_mat, glm::vec3(0.015f, m_scale, 0.015f));
}

void Grid::init(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Grid::Vertex::InitVertex(float x, float y, float z){
    position = glm::vec3(x,y,z);
}

void Grid::InitVertices(std::vector<Vertex>& vertices){
    int ind{0};
    double x_f, z_f = 0;
    int depth = m_depth/2;
    int width = m_width/2;

    float factor = ((float) depth)/((float) m_dominio);

    m_y_min = std::numeric_limits<float>::max();
    m_y_max = -std::numeric_limits<float>::max();

    for(int z = -depth; z < depth; z++){
        for(int x = -width; x < width; x++){
            x_f = (double)x/factor;
            z_f = (double)z/factor;
            float y = cos(x_f) + sin(z_f);

            if(y < m_y_min) m_y_min = y;
            if(y > m_y_max) m_y_max = y;

            vertices[ind].InitVertex(x,y,z);
            ind++;
        }
    }

    //m_scale = 1.5f/(m_y_max - m_y_min);
}

void Grid::InitIndices(std::vector<uint>& indices){
    int ind{0};

    for(int z = 0; z < m_depth-1; z++){
        for(int x = 0; x < m_width-1; x++){
            uint indBottomLeft = z * m_width + x;
            uint indTopLeft = (z+1) * m_width + x;
            uint indTopRight = (z+1) * m_width + (x+1);
            uint indBottomRight = z * m_width + (x+1);

            // Agrega triangulo superior izquierdo
            indices[ind++] = indBottomLeft;
            indices[ind++] = indTopLeft;
            indices[ind++] = indTopRight;

            // Agrega triangulo inferior derecho
            indices[ind++] = indBottomLeft;
            indices[ind++] = indTopRight;
            indices[ind++] = indBottomRight;
        }
    }
}

void Grid::renderModel(const glm::mat4& view, const glm::mat4& projection, GLenum primitive){
    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);
    m_shaderProgram->setFloat("minY", m_y_min);
    m_shaderProgram->setFloat("maxY", m_y_max);

    glBindVertexArray(VAO);

    if (primitive == GL_POINTS) {
        glDrawArrays(GL_POINTS, 0, m_width * m_depth);
    } 
    else {
        glDrawElements(primitive, (m_width-1)*(m_depth-1)*6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void Grid::updateModel(float deltaTime, const glm::vec3& axis){
    // Sin tecla presionada no hay rotación
    if (axis == glm::vec3(0.0f)) return;

    // deltaTime se usa como ángulo en grados por frame
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime), axis);

    // Se multiplica por la IZQUIERDA para rotar sobre los ejes del mundo.
    // El modelo ya está centrado en el origen (initGeometry), así que gira sobre sí mismo.
    m_model_mat = rotacion * m_model_mat;
}

void Grid::finish(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}