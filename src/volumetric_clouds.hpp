#ifndef VOLUMETRIC_CLOUDS_H
#define VOLUMETRIC_CLOUDS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.hpp"

class VolumetricClouds
{
    public:
        VolumetricClouds(glm::vec3 position, glm::vec3 size) :
            position(position),
            size(size),
            rotation(0.0f)
        {
            initRenderData();
        }

        ~VolumetricClouds() {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &VBO);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(2, &IBO);
        }

        void Update(GLfloat deltatime)
        {

        }

        void Draw(Shader shader)
        {
            // Prepare transformations
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, position);

            modelMat = glm::translate(modelMat, size * 0.5f);
            modelMat = glm::rotate(modelMat, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMat = glm::translate(modelMat, size * -0.5f);

            modelMat = glm::scale(modelMat, size);

            shader.Use();
            shader.SetMatrix4("model", modelMat);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
            // glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
            // glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid *)(4 * sizeof(GLushort)));
            // glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid *)(8 * sizeof(GLushort)));

            glDisableVertexAttribArray(0);
        }

    private:
        glm::vec3 position, size;
        GLfloat rotation;
        GLuint VBO, IBO;
        Texture3D* texture3D;

        void initRenderData()
        {
            // Configure VBO/IBO
            GLfloat vertices[] = {
                -1.0f, -1.0f, 0.0f,
                 0.0f, -1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f
            };
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            GLushort indices[] = {
                0, 3, 1,
                1, 3, 2,
                2, 3, 0,
                0, 2, 1
            };
            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        }      
};

#endif