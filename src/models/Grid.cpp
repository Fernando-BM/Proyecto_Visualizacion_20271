#include "models/Grid.h"

void Grid::Vertex::InitVertex(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void Grid::InitVertices(std::vector<Vertex>& vertices)
{
    int ind = 0;
    double worldX, worldZ = 0.0;
    int halfDepth = m_depth/2;
    int halfWidth = m_width/2;

    float scalingFactor = ((float) halfDepth)/((float) m_domain);

    for(int z = -halfDepth; z < halfDepth; z++){
        for(int x = -halfWidth; x < halfWidth; x++){
            worldX = (double)x/scalingFactor;
            worldZ = (double)z/scalingFactor;

            // f(x, z) = cos(x) + sin(z)
            float y = cos(worldX) + sin(worldZ);

            // Obtiene valores minimo y maximo Y
            if (y < m_minY) m_minY = y;
            if (y > m_maxY) m_maxY = y;

            vertices[ind].InitVertex(x,y,z);
            ind++;
        }
    }
}

void Grid::InitIndexes(std::vector<uint>& indexes)
{
    int ind = 0;

    for(int z = 0; z < m_depth-1; z++){
        for(int x = 0; x < m_width-1; x++){
            uint indBottomLeft = z * m_width + x;
            uint indTopLeft = (z+1) * m_width + x;
            uint indTopRight = (z+1) * m_width + (x+1);
            uint indBottomRight = z * m_width + (x+1);

            // Triangulo superior izquierdo
            indexes[ind++] = indBottomLeft;
            indexes[ind++] = indTopLeft;
            indexes[ind++] = indTopRight;

            // Triangulo inferior derecho
            indexes[ind++] = indBottomLeft;
            indexes[ind++] = indTopRight;
            indexes[ind++] = indBottomRight;
        }
    }
}

void Grid::init()
{
    // Genera identificadores
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Grid::initGeometry()
{
    int totalQuads = (m_width-1)*(m_depth-1);

    std::vector<Grid::Vertex> vertices;
    vertices.resize(m_width * m_depth);
    InitVertices(vertices);

    std::vector<unsigned int> indexes;    
    // Cada quad tiene 6 vertices
    indexes.resize(totalQuads*6);
    InitIndexes(indexes);

    // Vincula VAO
    glBindVertexArray(VAO);

    // Carga los vertices en el VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Carga los indices en el EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes[0]) * indexes.size(), &indexes[0], GL_STATIC_DRAW);

    // Localizacion 0: posicion del vertice (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincula VAO
    glBindVertexArray(0);

    // Escalar modelo para poder visualizarla
    m_model_mat = glm::mat4(1.0f);
    m_model_mat = glm::translate(m_model_mat, glm::vec3(0.0f));
    m_model_mat = glm::scale(m_model_mat, glm::vec3(0.015f, m_scale, 0.015f));
}

void Grid::renderModel(const glm::mat4& view, const glm::mat4& projection, int primitive, bool /*print_info*/)
{
    // Activa programa del shader
    m_shaderProgram->use();

    // Carga matriz de modelo
    m_shaderProgram->setMat4x4("model", m_model_mat);
    // Carga matriz de vista
    m_shaderProgram->setMat4x4("view", view);
    // Carga matriz de proyeccion
    m_shaderProgram->setMat4x4("projection", projection);
    // Carga el valor minimo y
    m_shaderProgram->setFloat("minY", m_minY);
    // Carga el valor maximo y
    m_shaderProgram->setFloat("maxY", m_maxY);

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
    // Sin tecla presionada no hay rotacion
    if (axis == glm::vec3(0.0f)) return;

    // deltaTime se usa como angulo en grados por frame
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime), axis);

    // Multiplica por la izquierda para rotar sobre los ejes, girando sobre si mismo
    m_model_mat = rotacion * m_model_mat;
}

void Grid::finish(){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}