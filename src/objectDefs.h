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
#include <sys/types.h>
#include <sys/stat.h>
using std::vector;
using std::shared_ptr;
using std::string;
using std::ifstream;
using std::to_string;
using std::fstream;
using glm::vec3;
using glm::mat4;

class Renderable {
   public :
      virtual void render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model) = 0;
      virtual void updateMaterial() = 0;
};
class Updateable {
   public :
      virtual void update(double dt) = 0;
};

class Topo: public Renderable {
   private :
      Texture texture;
      GLuint topoVertexArrayID;
      vector<GLfloat> topoVertex;
      vector<GLfloat> topoColor;
      vector<GLfloat> topoNormals;
      vector<GLfloat> topoTex;
	   shared_ptr<Program> shader;
      void readTopo(string filename);
      void insertPoint(int minZ, uint32_t *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
      void fillTopoArrays(uint32_t *data, unsigned int width, unsigned int height);
      void generateNormals();
      void createShader();
      void createTexture();
      int cMat = TOPO_MATERIAL;
   public :
      uint32_t *elevationData;
      int width;
      int height;
      void init(string filename);
      void render(shared_ptr<MatrixStack> Projection,
                  mat4 View,
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
      Texture texture;
      float density;
      glm::vec3 maxTrans;
      glm::vec3 minTrans;
      glm::vec3 maxRotat;
      glm::vec3 minRotat;
      glm::vec3 maxScale;
      glm::vec3 minScale;
      glm::vec3 globalTrans;
      glm::vec3 globalScale;
      glm::vec3 globalRotat;
      void fillItems(uint32_t *elev, unsigned int width, unsigned int height,
                     unsigned int originx, unsigned int originy);
   public :
      void init(int landType, uint32_t elev, float density, uint32_t *dem, int demwidth, int demheight, int originx, int originy, int DX, int DY);
      void render(shared_ptr<MatrixStack> Projection,
                  mat4 View,
                  shared_ptr<MatrixStack> Model);
      vec3 getPosition();
};
class GroundMap : public Renderable {
   private :
      vector<shared_ptr<LandCover>> blocks;
      void generateMap(unsigned char *lcdata, 
                       uint32_t *demdata, 
                       int lcwidth, 
                       int lcheight, 
                       int demwidth, 
                       int demheight);
   public :
      void init(string lcfile, uint32_t *demdata, int demwidth, int demheight);
      void render(shared_ptr<MatrixStack> Projection,
                  mat4 View,
                  shared_ptr<MatrixStack> Model);
      void updateMaterial();
};

#define OPEN_WATER 11 
#define SNOW 12
#define DEVELOPED_OPEN_SPACE 21
#define DEVELOPED_LOW 22
#define DEVELOPED_MED 23
#define DEVELOPED_HIGH 24
#define BARREN_LAND 31
#define DECIDUOUS_FOREST 41
#define EVERGREEN_FOREST 42 
#define MIXED_FOREST 43
#define DWARF_SHRUB 51
#define SHRUB 52
#define GRASSLAND 71
#define SEDGE 72
#define LICHENS 73
#define MOSS 74
#define PASTURE 81
#define CROPS 82
#define WOODED_WETLANDS 90
#define WETLAND 95
#define N_LAND_TYPES 100
class LandDescription { //ehh this is more of a struct but hey.
   public : 
	   shared_ptr<Program> shader;
      vector<shared_ptr<Shape>> mesh;
      vec3 maxrotat;
      vec3 minrotat;
      vec3 minscale;
      vec3 maxscale;
      Texture texture;
};
class LandType {
   private :
      static vector<shared_ptr<LandDescription>> landDescs;
      static void readAllLandTypes();
      static shared_ptr<LandDescription> readLandDescription(int type);
      static void LandType::readMetaFile(string fname, shared_ptr<LandDescription> ld);
   public :
      static void fillTransforms(unsigned char type, vec3 &maxrotat, vec3 &minrotat, 
                                 vec3 &minscale, vec3 &maxscale);
      static void getDrawDataForType(unsigned char type, Texture &texdest, vector<shared_ptr<Shape>> &meshdest);
	   static shared_ptr<Program> shader;
      static void init();
      
};


class State {
   public :
      static vec3 initViewPosition;
      static float initPhi;
      static float initTheta;
      static vec3 initLightPos;
      static vec3 initLightCol;
      static float initScaler;
      
      static vec3 viewPosition;
      static float phi;
      static float theta;
      static float scaler;
      static vec3 lightPos;
      static vec3 lightCol;
      static GLfloat zscale;
      static void reset();
      static int topoDetailLevel;
      static string resourceDirectory;
      static void generateOptionalMesh(string fname);
      static string placeDirectory;
      static int ztrans;
};


class Skybox : public Renderable{
   private :
      void createSky(string dir, vector<string> faces);
      void createShader();
      unsigned int textureID;
	   shared_ptr<Program> shader;
      vector<shared_ptr<Shape>> mesh;
   public :
      void init();
      void render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model);
      void updateMaterial();
};


#endif

