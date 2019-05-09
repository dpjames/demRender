#ifndef _OBJECT_DEF_HA
#define _OBJECT_DEF_HA

#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <math.h>
#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

using std::vector;
using std::shared_ptr;
using std::string;

class Renderable {
   public :
      virtual void render(shared_ptr<MatrixStack> Projection,shared_ptr<MatrixStack> View,shared_ptr<MatrixStack> Model) = 0;
};
class Updateable {
   public :
      virtual void update(double dt) = 0;
};

class Topo: public Renderable {
   private :
      GLuint topoVertexArrayID;
      vector<GLfloat> topoVertex;
      vector<GLfloat> topoColor;
	   shared_ptr<Program> prog;
      void readTopo(string filename);
      void insertPoint(float minz, float maxz, unsigned char *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
      void fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height);
   public :
      void init(string filename, shared_ptr<Program> p);
      void render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model);
};


//class Tree { //renderable, but a slightly different render function
//   private :
//      glm::vec3 loc;
//      glm::vec3 scale;
//      static vector<shared_ptr<Shape>> mesh;
//   public :
//      void render(shared_ptr<MatrixStack> Model,
//                  shared_ptr<Program> prog);
//      void init(glm::vec3 l, glm::vec3 s);
//};
//class Forest : public Renderable {
//   private :
//      int width;
//      int len;
//      int seperation;
//      shared_ptr<Program> prog;
//      vector<shared_ptr<Tree>> trees;
//      void fillTrees();
//   public :
//      void init(int w, int l, int sep, shared_ptr<Program> p);
//      void render(shared_ptr<MatrixStack> Projection,
//                  shared_ptr<MatrixStack> View,
//                  shared_ptr<MatrixStack> Model);
//};
class Cover {
   private :
      glm::vec3 trans;
      glm::vec3 scale;
      glm::vec3 rotat;
   public : 
      void init(glm::vec3 t, glm::vec3 s, glm::vec3 r);
      void render(shared_ptr<MatrixStack> Model,
                  shared_ptr<Program> prog,
                   vector<shared_ptr<Shape>> mesh);
};

class LandCover : public Renderable {
   private :
      shared_ptr<Program> shader;
      vector<shared_ptr<Shape>> mesh;
      vector<shared_ptr<Cover>> items;
      GLfloat seperation;
      GLfloat nchildren;
      glm::vec3 maxTrans;
      glm::vec3 maxRotat;
      glm::vec3 minRotat;
      glm::vec3 maxScale;
      glm::vec3 minScale;
      glm::vec3 globalTrans;
      glm::vec3 globalScale;
      void fillItems();
   public :
      void init(
                glm::vec3 tTrans,
                glm::vec3 tScale,
                glm::vec3 trans, 
                glm::vec3 minscale, 
                glm::vec3 maxscale, 
                glm::vec3 minrot, 
                glm::vec3 maxrot, 
                GLfloat n, 
                shared_ptr<Program> shaderin, 
                string objname);
      void render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model);
};
class GroundMap : public Renderable {
   private :
      vector<shared_ptr<LandCover>> blocks;
      void generateMap(unsigned char *lcdata, 
                       unsigned char *demdata, 
                       int lcwidth, 
                       int lcheight, 
                       int demwidth, 
                       int demheight);
   public :
      void init(string lcfile, string demfile);
      void render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model);
};
#endif
