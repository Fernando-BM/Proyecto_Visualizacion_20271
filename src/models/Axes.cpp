#include "models/Axes.h"

void Axes::initGeometry(){
    // 6 vertices: 2 por cada eje (X:rojo, Y:verde, z:azul)
    m_vertices[0] = 0.0f; m_vertices[1] = 0.0f; m_vertices[2] = 0.0f; m_vertices[3] = 1.0f; m_vertices[4] = 0.0f; m_vertices[5] = 0.0f; // v0: origen
    m_vertices[6] = m_length; m_vertices[7] = 0.0f; m_vertices[8] = 0.0f; m_vertices[9] = 1.0f; m_vertices[10] = 0.0f; m_vertices[11] = 0.0f; // v1: extremo X
    m_vertices[12] = 0.0f; m_vertices[13] = 0.0f; m_vertices[14] = 0.0f; m_vertices[15] = 0.0f; m_vertices[16] = 1.0f; m_vertices[17] = 0.0f; // v2: origen
    m_vertices[18] = 0.0f; m_vertices[19] = m_length; m_vertices[20] = 0.0f; m_vertices[21] = 0.0f; m_vertices[22] = 1.0f; m_vertices[23] = 0.0f; // v3: extremo Y
    m_vertices[24] = 0.0f; m_vertices[25] = 0.0f; m_vertices[26] = 0.0f; m_vertices[27] = 0.0f; m_vertices[28] = 0.0f; m_vertices[29] = 1.0f; // v4: origen
    m_vertices[30] = 0.0f; m_vertices[31] = 0.0f; m_vertices[32] = m_length; m_vertices[33] = 0.0f; m_vertices[34] = 0.0f; m_vertices[35] = 1.0f; // v5: extremo Z

    // Pareja de indices que definen las aristas de los ejes
    m_indexes[0] = 0; m_indexes[1] = 1; // eje x
    m_indexes[2] = 2; m_indexes[3] = 3; // eje y
    m_indexes[4] = 4; m_indexes[5] = 5; // eje z
}

void Axes::init(){
    // Genera identificadores
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Vincula VAO
    glBindVertexArray(VAO);

    // Carga los vertices en el VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

    // Carga los indices en el EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indexes), m_indexes, GL_STATIC_DRAW);

    // Localizacion 0: posicion del vertice (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Localizacion 1: color del vertice (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Desvincula VAO
    glBindVertexArray(0);
}

void Axes::renderModel(const glm::mat4& view, const glm::mat4& projection, int /*primitive*/, bool /*printInfo*/){
    // Activa programa del shader
    m_shaderProgram->use();

    // Carga matriz de modelo
    m_shaderProgram->setMat4x4("model", m_model_mat);
    // Carga matriz de vista
    m_shaderProgram->setMat4x4("view", view);
    // Carga matriz de proyeccion
    m_shaderProgram->setMat4x4("projection", projection);

    glBindVertexArray(VAO);
    // Pinta ejes coordenados con la primitiva de lineas
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0); 

    glBindVertexArray(0);
}

void Axes::updateModel(float /*deltaTime*/, const glm::vec3& /*axis*/){
    // Los ejes permanecen fijos
}

void Axes::finish()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}