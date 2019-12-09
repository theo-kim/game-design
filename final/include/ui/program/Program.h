#ifndef PROGRAM_DEF
#define PROGRAM_DEF

#include <fstream>
#include <sstream>

#include "standard.h"
#include "ui/camera/Camera.h"
#include "ui/light/Light.h"

class Camera;

class ShaderProgram {
public:	
  void Load(const char *vertexShaderFile, const char *fragmentShaderFile);
  void Cleanup();

  void SetColor(float r, float g, float b, float a);
  void SetCamera(Camera *c);
  void SetLight(Light *l);

  void GenerateMVP(glm::mat4 model);

  void EnableLighting();

  GLuint programID;

  GLuint projectionMatrixUniform;
  GLuint modelMatrixUniform;
  GLuint viewMatrixUniform;
  GLuint colorUniform;
  GLuint lightingPositionUniform;
  GLuint lightingColorUniform;
  GLuint lightingPowerUniform;

  GLuint positionAttribute;
  GLuint texCoordAttribute;
  GLuint normalAttribute;
  
  Camera *camera;
  Light *light;
private:
  GLuint vertexShader;
  GLuint fragmentShader;

  GLuint LoadShaderFromString(const std::string &shaderContents, GLenum type);
  GLuint LoadShaderFromFile(const std::string &shaderFile, GLenum type);

  void SetProjectionMatrix(const glm::mat4 &matrix);
  void SetViewMatrix(const glm::mat4 &matrix);
  void SetModelMatrix(const glm::mat4 &matrix);
};

class TexturedShader : public ShaderProgram {};

class UntexturedShader : public ShaderProgram {};

#endif