#include "Axes.h"

void Axes::initGeometry(){
    // 6 vértices: posición (x,y,z) + color (r,g,b)
    m_vertices = {
        // Eje X (rojo)
        0.0f,     0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   // v0: origen
        m_length, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   // v1: extremo X
        // Eje Y (verde)
        0.0f, 0.0f,     0.0f,   0.0f, 1.0f, 0.0f,   // v2: origen
        0.0f, m_length, 0.0f,   0.0f, 1.0f, 0.0f,   // v3: extremo Y
        // Eje Z (azul)
        0.0f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,   // v4: origen
        0.0f, 0.0f, m_length,   0.0f, 0.0f, 1.0f    // v5: extremo Z
    };

    // Cada pareja de índices es una arista
    m_indices = {
        0, 1,   // eje X
        2, 3,   // eje Y
        4, 5    // eje Z
    };
}

void Axes::init(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat),
                 m_vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 m_indices.data(), GL_STATIC_DRAW);

    // location 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // location 1: color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Axes::renderModel(const glm::mat4& view, const glm::mat4& projection){
    m_shaderProgram->use();

    m_shaderProgram->setMat4x4("model", m_model_mat);
    m_shaderProgram->setMat4x4("view", view);
    m_shaderProgram->setMat4x4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);  // GL_LINES en vez de GL_TRIANGLES
    glBindVertexArray(0);
}

void Axes::updateModel(float deltaTime){
    // Por ahora no se hace nada
}

void Axes::finish(){
}