#include "ui/mesh/Mesh3D.h"

Mesh3D::Mesh3D(VertexArray *array) 
  : Mesh(3, array)
{}

// Mesh3D::Mesh3D(std::initializer_list<float> _vertices, std::initializer_list<float> _normals)
//   : Mesh(3, _vertices),
//     normals(new float[_vertices.size()])
// {
//   int i = 0;
//   for (float n : _normals) normals[i++] = n;
//   InitializeBuffers();
// }

// Mesh3D::Mesh3D(std::initializer_list<float> _vertices, std::initializer_list<float> _normals, std::initializer_list<float> _texture) 
//   : Mesh(3, _vertices, _texture),
//     normals(new float[_vertices.size()])
// {
//   int i = 0;
//   for (float n : _normals) normals[i++] = n;
//   InitializeBuffers();
// }

// Mesh3D::Mesh3D(std::vector<float> _vertices, std::vector<float> _normals)
//   : Mesh(3, _vertices),
//     normals(new float[_vertices.size()])
// {
//   int i = 0;
//   for (float n : _normals) normals[i++] = n;
//   InitializeBuffers();
// }

// Mesh3D::Mesh3D(std::vector<float> _vertices, std::vector<float> _normals, std::vector<float> _texture) 
//   : Mesh(3, _vertices, _texture),
//     normals(new float[_vertices.size()])
// {
//   int i = 0;
//   for (float n : _normals) normals[i++] = n;
//   InitializeBuffers();
// }

// Copy constructor
Mesh3D::Mesh3D(const Mesh3D &original)
  : Mesh(original)
{

}

Mesh3D::~Mesh3D() {} // Nothing really needs to be done...

// Factory to produce a Mesh from a .OBJ file
Mesh3D *Mesh3D::FromOBJ(const char *path) {
  // Open the file
  std::ifstream obj;
  obj.open(path);
  if (!obj) {
    std::cout << "Could not open model at path: " << path << std::endl;
    return nullptr;
  }

  // Parse each line: <command> <*arguments>
  std::string line, command;
  
  std::vector<VertexAttribute *> v;
  std::vector<VertexAttribute *> uv;
  std::vector<VertexAttribute *> vn;
  std::vector<Vertex *> faces;
  // std::vector<float> faceNormals;
  // std::vector<float> textureMap;

  while (getline(obj, line)) {
    float x, y, z;
    std::istringstream iss(line);
    iss >> command;
    if (command == "#") { // comment
      continue;
    }
    else if (command == "v") { // vertex
      iss >> x >> y >> z;
      VertexAttribute *position = new VertexAttribute({ x, y, z });
      v.push_back(position);
    }
    else if (command == "vt") { // vertex texture
      iss >> x >> y;
      VertexAttribute *texture = new VertexAttribute({ x, y });
      uv.push_back(texture);
    }
    else if (command == "vn") { // vertex normal
      iss >> x >> y >> z;
      VertexAttribute *normal = new VertexAttribute({ x, y, z });
      vn.push_back(normal);
    }
    else if (command == "f") { // face
      std::string vertexBundle;
      while (iss >> vertexBundle) {
        std::string vertexAttribute;
        std::stringstream s(vertexBundle);
        Vertex *vertex = new Vertex();
        faces.push_back(vertex);
        // vertices
        getline(s, vertexAttribute, '/');
        std::stringstream(vertexAttribute) >> x;
        // Match with v array
        vertex->attributes.push_back(v[x - 1]);
        
        // uv
        getline(s, vertexAttribute, '/');
        std::stringstream(vertexAttribute) >> y;
        vertex->attributes.push_back(uv[y - 1]);

        // normals
        getline(s, vertexAttribute, '/');
        std::stringstream(vertexAttribute) >> z;
        vertex->attributes.push_back(vn[z - 1]);
      }
    }
    else {
      // std::cout << "Unknown command: " << command << std::endl;
    }
  }
  obj.close();
  // Create Vertex Array
  VertexArray *array = new VertexArray(faces);
  // Free unneeded stuff
  for (Vertex *v : faces) delete v;
  for (VertexAttribute *v : v) delete v;
  for (VertexAttribute *v : uv) delete v;
  for (VertexAttribute *v : vn) delete v;
  
  return new Mesh3D(array);
}

void Mesh3D::Render(ShaderProgram *shader) const {
  glBindTexture(GL_TEXTURE_3D, 0);
  vertices->Draw( { (int)shader->positionAttribute, -1, (int)shader->normalAttribute } );
}