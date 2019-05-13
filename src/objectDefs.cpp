#include "objectDefs.h"
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
//using std::min;
using glm::vec4;
using glm::length;
void updateLights(shared_ptr<Program> shader){
   glUniform3f(shader->getUniform("lightCol"), State::lightCol[0], State::lightCol[1], State::lightCol[2]);
   glUniform3f(shader->getUniform("lightPos"), State::lightPos[0], State::lightPos[1], State::lightPos[2]);
}
void setMaterial(shared_ptr<Program> p, int material){
   p->bind();
   switch (material) {
      case 0: //shiny blue plastic
         glUniform3f(p->getUniform("MatAmb"), 0.02, 0.04, 0.2);
         glUniform3f(p->getUniform("MatDif"), 0.0, 0.16, 0.9);
         glUniform3f(p->getUniform("MatSpec"), 0.14, 0.2, 0.8);
         glUniform1f(p->getUniform("shine"), 120.0);
         break;
      case 1: // flat grey
         glUniform3f(p->getUniform("MatAmb"), 0.13, 0.13, 0.14);
         glUniform3f(p->getUniform("MatDif"), 0.3, 0.3, 0.4);
         glUniform3f(p->getUniform("MatSpec"), 0.3, 0.3, 0.4);
         glUniform1f(p->getUniform("shine"), 4.0);
         break;
      case 2: //brass
         glUniform3f(p->getUniform("MatAmb"), 0.3294, 0.2235, 0.02745);
         glUniform3f(p->getUniform("MatDif"), 0.7804, 0.5686, 0.11373);
         glUniform3f(p->getUniform("MatSpec"), 0.9922, 0.941176, 0.80784);
         glUniform1f(p->getUniform("shine"), 27.9);
         break;
      case TREE_MATERIAL: //tree
         glUniform3f(p->getUniform("MatAmb"), 0.05, .05, 0.05);
         glUniform3f(p->getUniform("MatDif"), 0.03, 0.5, 0.01);
         glUniform3f(p->getUniform("MatSpec"), 0.01, 0.02, 0.01);
         glUniform1f(p->getUniform("shine"), .01);
         break;
      case TOPO_MATERIAL: // topo
         glUniform3f(p->getUniform("MatAmb"), 0.4, 0.4, 0.4);
         glUniform3f(p->getUniform("MatDif"), 0.2, 0.2, 0.2);
         glUniform3f(p->getUniform("MatSpec"), 0, 0, 0);
         glUniform1f(p->getUniform("shine"), 1.0);
         break;
   }
   p->unbind();
}

void readJPG(string filename, unsigned char*& data, int *width, int *height, int *n){
   cout << "reading " << filename << endl;
	stbi_set_flip_vertically_on_load(false);
   data = stbi_load(filename.c_str(), width, height, n, 1);
   if(data == NULL){
      perror(filename.c_str()); 
      return;
   }
}
void readObj(string fname, vector<shared_ptr<Shape>> &mesh){
   vector<tinyobj::shape_t> tss;
   vector<tinyobj::material_t> omm;
   string e;
   bool rcc = tinyobj::LoadObj(tss, omm, e, fname.c_str());
   if(!rcc){
      cout << e << endl;
   } else {
      for(unsigned int i = 0; i < tss.size(); i++){
         shared_ptr<Shape> s = make_shared<Shape>();
         s->createShape(tss[i]);
         s->measure();
         s->init();
         mesh.push_back(s);
      }
   }
}

/**********************/
/* BEGIN GROUND CLASS */
/**********************/
void GroundMap::init(string lcfile, string demfile){
   int demwidth, demheight, demn, lcwidth, lcheight, lcn;
   unsigned char *lcdata;
   unsigned char *demdata;
   readJPG(lcfile, lcdata, &lcwidth, &lcheight, &lcn);
   readJPG(demfile, demdata, &demwidth, &demheight, &demn);
   generateMap(lcdata, demdata, lcwidth, lcheight, demwidth, demheight); 
}
void GroundMap::generateMap(unsigned char *lcdata, 
      unsigned char *demdata, 
      int lcwidth, 
      int lcheight, 
      int demwidth, 
      int demheight){
   int DY = 10;
   int DX = 10;
   //unsigned char seen[width * height] = {};
   for(int y = 0; y < lcheight; y+=DY){
      for(int x = 0; x < lcwidth; x+=DX){
         float wx = (x / (float)lcwidth) * demwidth;
         float wy = (y / (float)lcheight) * demheight;
         int lcindex = x + y * lcwidth;
         int demindex = wx + wy * demwidth;
         unsigned char type = lcdata[lcindex];
         unsigned char elev = demdata[demindex];
         shared_ptr<LandCover> block = make_shared<LandCover>();
         block->init(
               vec3(0,0,0),
               vec3(1,1,1),
               vec3(0,0,0),
               vec3(wx-DX,elev,wy-DY),
               vec3(wx+DX,elev,wy+DX),
               vec3(.2,.2,.2),
               vec3(.2,.3,.2),
               vec3(0,0,0),
               vec3(0,0,0),
               2,
               type,
               demdata,
               demwidth,
               demheight,
               wx,wy
               );
         blocks.push_back(block);
      }
   }
}
void GroundMap::render(shared_ptr<MatrixStack> Projection,
      shared_ptr<MatrixStack> View,
      shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   for(unsigned int i = 0; i < blocks.size(); i++){
      if(length(blocks[i]->getPosition() * State::scaler + State::viewPosition) < 100 * State::scaler){
         blocks[i]->render(Projection, View, Model);
      }
   }
   Model->popMatrix();
}
void GroundMap::updateMaterial(){

}
/**********************/
/*  END GROUND CLASS  */
/**********************/


/**********************/
/*  BEGIN TOPO CLASS  */
/**********************/
void Topo::updateMaterial(){
   setMaterial(shader, ++cMat % N_MATERIAL);
}
void Topo::readTopo(string filename){
   int width = 0;
   int height = 0;
   int n = 0;
   unsigned char *data;
   readJPG(filename, data, &width, &height, &n);
   fillTopoArrays(data, width, height); 
}
/*
 * insert a point into the topo data arrays. this needs to be modified to change the initial orientation.
 * (the x, y and z axis are flipped around currently)
 */
void Topo::insertPoint(float minz, float maxz, unsigned char *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
   float z = data[x + y * width] * State::zscale;
   topoVertex.push_back(x);
   topoVertex.push_back(z);
   topoVertex.push_back(y);
   topoTex.push_back((float)x / width);
   topoTex.push_back((float)y / height);
}
void Topo::generateNormals(){
   cout << "generating normals" << endl;
   vector<GLfloat> normals(topoVertex.size(),0); 
   for(unsigned int i = 0; i < topoVertex.size(); i+=9){
      vec3 p1 = vec3(topoVertex[i+0], topoVertex[i+1], topoVertex[i+2]);
      vec3 p2 = vec3(topoVertex[i+3], topoVertex[i+4], topoVertex[i+5]);
      vec3 p3 = vec3(topoVertex[i+6], topoVertex[i+7], topoVertex[i+8]);
      vec3 normal = glm::cross(p2 - p1, p3 - p1);
      normals[i + 0]+=normal[0];
      normals[i + 1]+=normal[1];
      normals[i + 2]+=normal[2];

      normals[i + 3]+=normal[0];
      normals[i + 4]+=normal[1];
      normals[i + 5]+=normal[2];

      normals[i + 6]+=normal[0];
      normals[i + 7]+=normal[1];
      normals[i + 8]+=normal[2];
   }
   int per5 = normals.size() * .05;
   for(unsigned int i = 0; i < normals.size(); i+=3){
      if(i % per5 == 0){
         cout << "\r";
         cout << (float)i / normals.size() * 100 << "%";
      }
      vec3 normalized = glm::normalize(vec3(normals[i+0], normals[i+1], normals[i+2]));
      normals[i + 0] = normalized[0];
      normals[i + 1] = normalized[1];
      normals[i + 2] = normalized[2];
   }
   cout << "\r" << "      " << "\r";
   topoNormals = normals;
}
/*
 * use the image data pointer and the width/height to fill topoColor and topoVertex
 */
void Topo::fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height){
   //topoVertex 
   float scale = State::topoDetailLevel; 
   float minZ = data[0];
   float maxZ = data[0];
   //find a scale for the height data. This is usually 0-255, but could change to lager ranges in the future
   //(right now it is 1 byte. I might update it to be 2 bytes)
   for(unsigned int i = 0; i < width*height; i++){
      if(data[i] < minZ){
         minZ = data[i];
      }
      if(data[i] > maxZ){
         maxZ = data[i];
      }
   }
   for(unsigned int y = 0; y < height - scale; y+=scale){
      for(unsigned int x = 0; x < width; x+=scale){
         if(x + scale < width){
            insertPoint(minZ,maxZ, data, x, y, width,height);
            insertPoint(minZ,maxZ, data, x, y + scale, width,height);
            insertPoint(minZ,maxZ, data, x + scale, y, width,height);
         }
         if(x >= scale){
            insertPoint(minZ,maxZ, data, x, y, width,height);
            insertPoint(minZ,maxZ, data, x - scale, y + scale, width,height);
            insertPoint(minZ,maxZ, data, x, y + scale, width,height);
         }
      }
   }
   generateNormals();
   //send triganle data to GPU
   GLuint topoVertexBufferID;
   glGenVertexArrays(1, &topoVertexArrayID);
   glBindVertexArray(topoVertexArrayID);
   glGenBuffers(1, &topoVertexBufferID);
   glBindBuffer(GL_ARRAY_BUFFER, topoVertexBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoVertex.size(), topoVertex.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   GLuint topoNormalBufferID;
   glGenBuffers(1, &topoNormalBufferID);
   glBindBuffer(GL_ARRAY_BUFFER, topoNormalBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoNormals.size(), topoNormals.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   GLuint topoTextureBufferID;
   glGenBuffers(1, &topoTextureBufferID);
   glBindBuffer(GL_ARRAY_BUFFER, topoTextureBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoTex.size(), topoTex.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   createTexture(); 

   //bind null
   glBindVertexArray(0);
}
void Topo::init(string filename){
   createShader();
   readTopo(filename);
}
void Topo::createTexture(){
   shader->bind();
   Texture t;
   t.setFilename(State::resourceDirectory + "/topo.tex.jpg");
   t.init();
   t.setWrapModes(GL_REPEAT,GL_REPEAT);
   shader->unbind();
}
void Topo::createShader(){
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/topo_vert.glsl", State::resourceDirectory + "/topo_frag.glsl");
   shader->init();
   shader->addUniform("P");
   shader->addUniform("V");
   shader->addUniform("M");
   shader->addUniform("MatDif");
   shader->addUniform("lightCol");
   shader->addUniform("MatAmb");
   shader->addUniform("MatSpec");
   shader->addUniform("shine");
   shader->addUniform("lightPos");
   shader->addAttribute("vertPos");
   shader->addAttribute("vertNor");
   shader->addAttribute("vertTex");
   setMaterial(shader, TOPO_MATERIAL);
}
void Topo::render(shared_ptr<MatrixStack> Projection,
      shared_ptr<MatrixStack> View,
      shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   shader->bind();
   updateLights(shader);
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
   glBindVertexArray(topoVertexArrayID);
   glDrawArrays(GL_TRIANGLES, 0, topoVertex.size() * 3);
   glBindVertexArray(0);
   shader->unbind(); 

   Model->popMatrix();
}
/**********************/
/*   END TOPO CLASS   */
/**********************/

/**********************/
/* BEGIN LATYPE CLASS */
/**********************/
shared_ptr<Program> LandType::shader = make_shared<Program>(); //TODO generalize to all shaders
vector<shared_ptr<Shape>> LandType::mesh;
void LandType::getMeshByType(unsigned char type, vector<shared_ptr<Shape>> &meshdestination){
   if(type == 41 || type == 42 || type == 43){
      meshdestination = LandType::mesh; 
   } else {

   }
}
void LandType::getShaderByType(unsigned char type, shared_ptr<Program> &shaderdestination){
   shaderdestination = LandType::shader; 
}
void LandType::init(){
   readObj(State::resourceDirectory + "/plants/tree.obj", mesh); //TODO generalize to all mesh
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/tree_vert.glsl", State::resourceDirectory + "/tree_frag.glsl");
   shader->init();
   shader->addUniform("P");
   shader->addUniform("V");
   shader->addUniform("M");
   shader->addUniform("MatDif");
   shader->addUniform("lightCol");
   shader->addUniform("MatAmb");
   shader->addUniform("MatSpec");
   shader->addUniform("shine");
   shader->addUniform("lightPos");
   shader->addAttribute("vertPos");
   shader->addAttribute("vertNor");
   shader->addAttribute("vertTex");
   shader->bind();
   Texture t;
   t.setFilename(State::resourceDirectory + "/plants/tree.tex.jpg");
   t.init();
   t.setWrapModes(GL_REPEAT,GL_REPEAT);
   shader->unbind();
   setMaterial(shader, TREE_MATERIAL);
}

/**********************/
/*  END LATYPE CLASS  */
/**********************/

/**********************/
/* BEGIN LANDCO CLASS */
/**********************/


void LandCover::init(vec3 tTrans,          vec3 tScale, 
      vec3 tRot,            vec3 mintrans, vec3 maxtrans,
      vec3 minscale,        vec3 maxscale, 
      vec3 minrot,          vec3 maxrot, 
      GLfloat n,            unsigned char landType,
      unsigned char *elev,  unsigned int ewidth,
      unsigned int eheight, unsigned int originx, 
      unsigned int originy){
   LandType::getMeshByType(landType, mesh);
   LandType::getShaderByType(landType, shader);
   nchildren = n;
   minTrans = mintrans;
   maxTrans = maxtrans;
   maxRotat = maxrot;
   minRotat = minrot;
   maxScale = maxscale;
   minScale = minscale;
   globalTrans = tTrans;
   globalScale = tScale;
   globalRotat = tRot;
   fillItems(elev, ewidth, eheight, originx, originy);
}
vec3 LandCover::getPosition(){
   return minTrans;
}
void LandCover::fillItems(unsigned char *elev, unsigned int width, unsigned int height, unsigned int originx, unsigned int originy){
   GLfloat tx,ty,tz,sx,sy,sz,rx,ry,rz;
   for(int i = 0; i < nchildren; i++){
      tx = minTrans[0] + (maxTrans[0] - minTrans[0]) * (rand() % 100000) / 100000; 
      tz = minTrans[2] + (maxTrans[2] - minTrans[2]) * (rand() % 100000) / 100000; 
      if(tx < 0){
         tx = 0;
      }
      if(tx > width - 1){
         tx = width - 1;
      }
      if(tz < 0){
         tz = 0;
      }
      if(tz > height - 1){
         tz = height - 1;
      }
      ty = elev[(int)tx + (int)tz * width] * State::zscale;
      ty = 1 * ty;
      sx = minScale[0] + (maxScale[0] - minScale[0]) * (rand() % 100000) / 100000;  //+ 1 to allow both directions of scaling with one vector
      sy = minScale[1] + (maxScale[1] - minScale[1]) * (rand() % 100000) / 100000; 
      sz = minScale[2] + (maxScale[2] - minScale[2]) * (rand() % 100000) / 100000; 
      rx = minRotat[0] + (maxRotat[0] - minRotat[0]) * (rand() % 100000) / 100000; 
      ry = minRotat[1] + (maxRotat[1] - minRotat[1]) * (rand() % 100000) / 100000; 
      rz = minRotat[2] + (maxRotat[2] - minRotat[2]) * (rand() % 100000) / 100000; 
      shared_ptr<Cover> c = make_shared<Cover>();
      c->init(vec3(tx,ty,tz),vec3(sx,sy,sz),vec3(rx,ry,rz));
      items.push_back(c);
   }
}
void LandCover::render(shared_ptr<MatrixStack> Projection,
      shared_ptr<MatrixStack> View,
      shared_ptr<MatrixStack> Model){
   shader->bind();
   updateLights(shader);
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
   Model->pushMatrix();
   Model->translate(globalTrans);
   Model->scale(globalScale);
   Model->rotate(globalRotat[0], vec3(1,0,0));
   Model->rotate(globalRotat[1], vec3(0,1,0));
   Model->rotate(globalRotat[2], vec3(0,0,1));
   for(unsigned int i = 0; i < items.size(); i++){
      items[i]->render(Model, shader, mesh);
   }
   Model->popMatrix();
   shader->unbind();

}
/**********************/
/*  END LANDCO CLASS  */
/**********************/

/**********************/
/* BEGIN COVER  CLASS */
/**********************/
void Cover::init(vec3 t, vec3 s, vec3 r){
   trans = t;
   scale = s;
   rotat = r;
}
void Cover::render(shared_ptr<MatrixStack> Model,
      shared_ptr<Program> shader,
      vector<shared_ptr<Shape>> mesh){
   Model->pushMatrix();
   Model->translate(trans);
   Model->scale(scale);
   Model->rotate(rotat[0], vec3(1,0,0));
   Model->rotate(rotat[1], vec3(0,1,0));
   Model->rotate(rotat[2], vec3(0,0,1));
   glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
   for(unsigned int i = 0; i < mesh.size(); i++){
      mesh[i]->draw(shader);
   }
   Model->popMatrix();

}
/**********************/
/*  END COVER  CLASS  */
/**********************/



/**********************/
/* Begin State CLASS  */
/**********************/

//initial state
vec3  State::initViewPosition = vec3(0,-13,-535);
vec3  State::initViewRotation = vec3(0,M_PI/2,0);
vec3  State::initLightPos     = vec3(0,50,535);
vec3  State::initLightCol     = vec3(1,1,1);
float State::initScaler       = 1;

//current state
vec3 State::viewPosition = State::initViewPosition;
vec3 State::viewRotation = State::initViewRotation;
vec3 State::lightPos     = State::initLightPos;
vec3 State::lightCol     = State::initLightCol;
float State::scaler = 1;

//defaults
GLfloat State::zscale = 1;
int State::topoDetailLevel = 10;
string State::resourceDirectory = "../resources";

void State::reset(){
   State::viewPosition = State::initViewPosition;
   State::viewRotation = State::initViewRotation;
   State::lightPos     = State::initLightPos;
   State::lightCol     = State::initLightCol;
   State::scaler       = State::initScaler;
}


/**********************/
/*  END State  CLASS  */
/**********************/


