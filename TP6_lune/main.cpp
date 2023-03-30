#include "glimac/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include <glimac/sphere_vertices.hpp>
#include <iostream>

int main() {
  auto ctx = p6::Context{{1280, 720, "TP6 EX1"}};
  //   ctx.maximize_window();

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  const std::vector<glimac::ShapeVertex> vertices =
      glimac::sphere_vertices(1.f, 32, 16);

  // Chargement des shaders
  const p6::Shader shader =
      p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

  shader.use();
  // Obtention de l'id de la variable uniforme
  GLint uMVPMatrix_location = glGetUniformLocation(shader.id(), "uMVPMatrix");
  GLint uMVMatrix_location = glGetUniformLocation(shader.id(), "uMVMatrix");
  GLint uNormalMatrix_location =
      glGetUniformLocation(shader.id(), "uNormalMatrix");

  glEnable(GL_DEPTH_TEST);

  // Création d'un VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Bindind du vbo sur la cible
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Puis on envois les données à la CG
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex),
               vertices.data(), GL_STATIC_DRAW);

  // Débindind du vbo de la cible pour éviter de le remodifier
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Création du VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);

  // Binding du vao (un seul à la fois)
  glBindVertexArray(vao);

  // Dire à OpenGL qu'on utilise le VAO
  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_TEXCOORDS = 2;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

  // Indiquer à OpenGL où trouver les sommets
  // Bindind du vbo sur la cible
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Spécification du format de l'attribut de sommet position
  glVertexAttribPointer(
      VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));
  // Débindind du vbo de la cible pour éviter de le remodifier
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Débindind du vao de la cible pour éviter de le remodifier
  glBindVertexArray(0);

  std::vector<glm::vec3> AxesRotation;
  std::vector<glm::vec3> AxesTranslation;
  for (int i = 0; i < 32; ++i) {
    AxesRotation.push_back(glm::sphericalRand(1.0f));
    AxesTranslation.push_back(glm::sphericalRand(2.0f));
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glBindVertexArray(vao);

    // planète
    glm::mat4 ProjMatrix =
        glm::perspective(glm::radians(70.f), 800.f / 600.f, 0.1f, 100.f);
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE,
                       glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    // lune

    for (size_t i = 1; i < AxesRotation.size(); i++) {

      MVMatrix =
          glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f}); // Translation
      MVMatrix = glm::rotate(MVMatrix, ctx.time(),
                             AxesRotation.at(i)); // Translation * Rotation
      MVMatrix = glm::translate(
          MVMatrix,
          AxesTranslation.at(i)); // Translation * Rotation * Translation
      MVMatrix = glm::scale(
          MVMatrix,
          glm::vec3{0.2f}); // Translation * Rotation * Translation * Scale

      glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * MVMatrix));
      glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(MVMatrix));
      glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE,
                         glm::value_ptr(NormalMatrix));

      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }

    glBindVertexArray(0);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}