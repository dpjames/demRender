#ifndef _OBJECT_DEF_HA
#define _OBJECT_DEF_HA
#define _USE_MATH_DEFINES
#define TREE_MATERIAL 3
#define TOPO_MATERIAL 4
#define N_MATERIAL 5
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <math.h>
#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "Texture.h"
#include "WindowManager.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

using std::vector;
using std::shared_ptr;
using std::string;
using glm::vec3;

class Renderable {
   public :
      virtual void render(shared_ptr<MatrixStack> Projection,shared_ptr<MatrixStack> View,shared_ptr<MatrixStack> Model) = 0;
      virtual void updateMaterial() = 0;
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
      vector<GLfloat> topoNormals;
      vector<GLfloat> topoTex;
	   shared_ptr<Program> shader;
      void readTopo(string filename);
      void insertPoint(float minz, float maxz, unsigned char *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
      void fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height);
      void generateNormals();
      void createShader();
      void createTexture();
      int cMat = TOPO_MATERIAL;
   public :
      void init(string filename);
      void render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model);
      void updateMaterial();
};

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

class LandCover {
   private :
      shared_ptr<Program> shader;
      vector<shared_ptr<Shape>> mesh;
      vector<shared_ptr<Cover>> items;
      GLfloat seperation;
      GLfloat nchildren;
      glm::vec3 maxTrans;
      glm::vec3 minTrans;
      glm::vec3 maxRotat;
      glm::vec3 minRotat;
      glm::vec3 maxScale;
      glm::vec3 minScale;
      glm::vec3 globalTrans;
      glm::vec3 globalScale;
      glm::vec3 globalRotat;
      void fillItems(unsigned char *elev, unsigned int width, unsigned int height,
                     unsigned int originx, unsigned int originy);
   public :
      void init(
                glm::vec3 tTrans,
                glm::vec3 tScale,
                glm::vec3 tRot,
                glm::vec3 mintrans, 
                glm::vec3 maxtrans, 
                glm::vec3 minscale, 
                glm::vec3 maxscale, 
                glm::vec3 minrot, 
                glm::vec3 maxrot, 
                GLfloat n,
                unsigned char landType,
                unsigned char *elev,
                unsigned int ewidth,
                unsigned int eheight,
                unsigned int originx,
                unsigned int originy);

      void render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model);
      vec3 getPosition();
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
      void updateMaterial();
};


class LandType {
   private :
      static vector<shared_ptr<Shape>> mesh;
   public :
	   static shared_ptr<Program> shader;

      static void getMeshByType(unsigned char type,   vector<shared_ptr<Shape>> &mesh);
      static void getShaderByType(unsigned char type, shared_ptr<Program> &shader);
      static void init();
};


class State {
   public :
      static vec3 initViewPosition;
      static vec3 initViewRotation;
      static vec3 initLightPos;
      static vec3 initLightCol;
      static float initScaler;

      static vec3 viewPosition;
      static vec3 viewRotation;
      static float scaler;
      static vec3 lightPos;
      static vec3 lightCol;
      static GLfloat zscale;
      static void reset();
      static int topoDetailLevel;
      static string resourceDirectory;
      static void generateOptionalMesh(string fname);
      static string placeDirectory;
};
#endif
