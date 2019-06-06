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
         glUniform3f(p->getUniform("MatAmb"), 0.2, 0.2, 0.2);
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
void readBin(string filename, uint32_t *&data, int *width, int *height){
   cout << "reading bin" << endl;
   vector<uint32_t> vdata;
   ifstream file;
   file.open(filename, std::ios::binary);
   //uint32_t oneData = 0;
   char oneData[4];
   file.read(oneData, sizeof(char) * 4);
   *width = *((uint32_t *)oneData);
   file.read(oneData, sizeof(char) * 4);
   *height = *((uint32_t *)oneData);
   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
   int npix = *width * *height;
   data = (uint32_t *) malloc(sizeof(uint32_t) * npix);
   for(unsigned int i = 0; i < buffer.size()/4; i++){
      data[i] = ((uint32_t *)buffer.data())[i];
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
void GroundMap::init(string lcfile, uint32_t *demdata, int demwidth, int demheight){
   int lcwidth, lcheight, lcn;
   unsigned char *lcdata;
   readJPG(lcfile, lcdata, &lcwidth, &lcheight, &lcn);
   generateMap(lcdata, demdata, lcwidth, lcheight, demwidth, demheight); 
}
void GroundMap::generateMap(unsigned char *lcdata, 
      uint32_t *demdata, 
      int lcwidth, 
      int lcheight, 
      int demwidth, 
      int demheight){
   cout << "generating ground map" << endl;
   int DY = 1;
   int DX = 1;
   float density = 8;
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
         block->init(type, elev, density, demdata, demwidth, demheight, wx, wy, DX/(float)lcwidth * demwidth, DY/(float)lcheight * demheight);
         blocks.push_back(block);
      }
   }
}
void GroundMap::render(shared_ptr<MatrixStack> Projection,
      mat4 View,
      shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   for(unsigned int i = 0; i < blocks.size(); i++){
      vec3 dvec = blocks[i]->getPosition() * State::scaler - State::viewPosition;
      float dist = sqrt(pow(dvec[0],2) + pow(dvec[2],2));
      if(dist < 50 * State::scaler){
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
   uint32_t *data;
   readBin(filename, data, &width, &height);
   fillTopoArrays(data, width, height); 
   elevationData = data;
}
/*
 * insert a point into the topo data arrays. this needs to be modified to change the initial orientation.
 * (the x, y and z axis are flipped around currently)
 */
void Topo::insertPoint(int minZ, uint32_t *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
   float z = (data[x + y * width] - minZ) * State::zscale;
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
         cout << (float)i / normals.size() * 100 << "%                ";
      }
      vec3 normalized = glm::normalize(vec3(normals[i+0], normals[i+1], normals[i+2]));
      normals[i + 0] = normalized[0];
      normals[i + 1] = normalized[1];
      normals[i + 2] = normalized[2];
   }
   cout << "\r" << "                       " << "\r";
   topoNormals = normals;
}
/*
 * use the image data pointer and the width/height to fill topoColor and topoVertex
 */
void Topo::fillTopoArrays(uint32_t *data, unsigned int width, unsigned int height){
   //topoVertex 
   cout << "creating triangles" << endl;
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
   State::ztrans = -1 * minZ;
   for(unsigned int y = 0; y < height - scale; y+=scale){
      for(unsigned int x = 0; x < width; x+=scale){
         if(x + scale < width){
            insertPoint(minZ, data, x, y, width,height);
            insertPoint(minZ, data, x, y + scale, width,height);
            insertPoint(minZ, data, x + scale, y, width,height);
         }
         if(x >= scale){
            insertPoint(minZ, data, x, y, width,height);
            insertPoint(minZ, data, x - scale, y + scale, width,height);
            insertPoint(minZ, data, x, y + scale, width,height);
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
   glBindVertexArray(0);
   
   createTexture(); 
}
void Topo::init(string filename){
   createShader();
   readTopo(filename);
}
void Topo::createTexture(){
   texture.setFilename(State::resourceDirectory + State::placeDirectory + "/topo.tex.jpg");
   texture.init();
   texture.setWrapModes(GL_REPEAT,GL_REPEAT);
}
void Topo::createShader(){
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/topo_vert.glsl", State::resourceDirectory + "/topo_frag.glsl");
   shader->init();
   shader->addUniform("P");
   shader->addUniform("V");
   shader->addUniform("M");
   shader->addUniform("Texture0");
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
      mat4 View,
      shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   shader->bind();
   updateLights(shader);
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
   glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
   glBindVertexArray(topoVertexArrayID);
   texture.bind(shader->getUniform("Texture0"));
   glDrawArrays(GL_TRIANGLES, 0, topoVertex.size() * 3);
   glBindVertexArray(0);
   texture.unbind();
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
void LandType::getDrawDataForType(unsigned char type, Texture &texdest, vector<shared_ptr<Shape>> &meshdest){
   if(landDescs[type] != NULL){
      texdest = landDescs[type]->texture;
      meshdest = landDescs[type]->mesh;
   }
}
void LandType::fillTransforms(unsigned char type, vec3 &maxrotat, vec3 &minrotat, vec3 &maxscale, vec3 &minscale){
   //maxrotat = vec3(0,0,0);
   //minrotat = vec3(0,0,0);
   //minscale = vec3(.2,.2,.2);
   //maxscale = vec3(.2,.3,.2);
   //if(type != EVERGREEN_FOREST){
   //   minscale = vec3(1,1,1);
   //   maxscale = vec3(1,1,1);
   //}
   if(landDescs[type] != NULL){
      maxrotat = landDescs[type]->maxrotat;
      minrotat = landDescs[type]->minrotat;
      maxscale = landDescs[type]->maxscale;
      minscale = landDescs[type]->minscale;
   }
}
vector<shared_ptr<LandDescription>> LandType::landDescs;
void LandType::readAllLandTypes(){
   LandType::landDescs.reserve(N_LAND_TYPES);
   for(int i = 0; i < N_LAND_TYPES; i++){
      LandType::landDescs.push_back(readLandDescription(i));
   }
}
shared_ptr<LandDescription> LandType::readLandDescription(int type){
   string pathname = State::resourceDirectory + "/LANDCOVER/" + to_string(type) + "/";
   struct stat info;
   if(stat(pathname.c_str(), &info) == -1){
      return NULL;
   }
   if(info.st_mode & S_IFDIR){  // S_ISDIR() doesn't exist on my windows 
      string obj = pathname + "mesh.obj";
      string tex = pathname + "texture.jpg";
      string meta = pathname + "metadata.txt";
      shared_ptr<LandDescription> ld = make_shared<LandDescription>();
      readObj(obj, ld->mesh);
      ld->texture.setFilename(tex);
      ld->texture.init();
      ld->texture.setWrapModes(GL_REPEAT,GL_REPEAT);
      readMetaFile(meta, ld);
      return ld;
   }
   return NULL;
}
void LandType::readMetaFile(string fname, shared_ptr<LandDescription> ld){
   fstream meta;
   meta.open(fname);
   vec3 *locs[4] = {
      &(ld->minrotat),
      &(ld->maxrotat),
      &(ld->minscale),
      &(ld->maxscale)
   };
   for(int i = 0; i < 4; i++){ //read in 4 vec3
      string v1,v2,v3;
      meta >> v1;
      meta >> v2;
      meta >> v3;
      //cout << v1 << "," << v2 << "," << v3 << endl;
      *(locs[i]) = vec3(stof(v1), stof(v2), stof(v3));
   }
}
void LandType::init(){
   readAllLandTypes();
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/land_vert.glsl", State::resourceDirectory + "/land_frag.glsl");
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
   shader->addUniform("Texture0");
   shader->addAttribute("vertPos");
   shader->addAttribute("vertNor");
   shader->addAttribute("vertTex");
   setMaterial(shader, TOPO_MATERIAL);
}

/**********************/
/*  END LATYPE CLASS  */
/**********************/

/**********************/
/* BEGIN LANDCO CLASS */
/**********************/

void LandCover::init(int landType, uint32_t elev, float indensity, uint32_t *dem, int demwidth, int demheight, int originx, int originy, int DX, int DY){
   shader = LandType::shader;
   LandType::getDrawDataForType(landType, texture, mesh);
   density = indensity;  
   minTrans = vec3(originx - DX, elev, originy - DY);
   maxTrans = vec3(originx + DX, elev, originy + DY);
   LandType::fillTransforms(landType, maxRotat, minRotat, maxScale, minScale);
   globalTrans = vec3(0,0,0); // this will prob be removed  TODO
   globalScale = vec3(1,1,1); // this will prob be removed
   globalRotat = vec3(0,0,0); // this will prob be removed
   fillItems(dem, demwidth, demheight, originx, originy);
}
vec3 LandCover::getPosition(){
   return (minTrans + maxTrans) / 2.0f;
}
void LandCover::fillItems(uint32_t *elev, unsigned int width, unsigned int height, unsigned int originx, unsigned int originy){
   GLfloat tx,ty,tz,sx,sy,sz,rx,ry,rz;
   for(int i = 0; i < density; i++){
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
      ty = (elev[(int)tx + (int)tz * width] + State::ztrans) * State::zscale;
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
      mat4 View,
      shared_ptr<MatrixStack> Model){
   shader->bind();
   updateLights(shader);
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
   Model->pushMatrix();
   texture.bind(shader->getUniform("Texture0"));
   for(unsigned int i = 0; i < items.size(); i++){
      items[i]->render(Model, shader, mesh);
   }
   texture.unbind();
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
/* Begin Player Class */
/**********************/
void Player::init(int x, int y, int z){
   velocity = vec3(0,0,1);
   acceleration = vec3(0,0,0);
   position = vec3(x,y,z)

}
void Player::updateMaterial(){}
void Player::render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model){
}


/**********************/
/* End Player Class   */
/**********************/

/**********************/
/* Begin State CLASS  */
/**********************/

//initial state
vec3  State::initViewPosition = vec3(0,0,0);
vec3  State::initLightPos     = vec3(-1000,10000,-1000);
vec3  State::initLightCol     = vec3(1.2,1.2,1.2);
float State::initScaler       = 1;
float State::initPhi = 0;
float State::initTheta = 0;




//current state
vec3 State::viewPosition = State::initViewPosition;
vec3 State::lightPos     = State::initLightPos;
vec3 State::lightCol     = State::initLightCol;
float State::phi         = State::initPhi; 
float State::theta       = State::initTheta; 
float State::scaler = 1;

//defaults
GLfloat State::zscale = 1;
int State::ztrans = 0;
int State::topoDetailLevel = 10;
string State::resourceDirectory = "../resources";
string State::placeDirectory = "/.";
bool State::grounded = false;
bool State::capturedCursor = true;
void State::reset(){
   State::lightPos     = State::initLightPos;
   State::lightCol     = State::initLightCol;
   State::scaler       = State::initScaler;
   State::phi          = State::initPhi;
   State::theta        = State::initTheta;
   State::viewPosition = State::initViewPosition;
}


/**********************/
/*  END State  CLASS  */
/**********************/

/**********************/
/* BEGIN SKYBO  CLASS */
/**********************/
void Skybox::init(){
   vector<std::string> faces {
      "right.jpg",
      "left.jpg",
      "up.jpg",
      "down.jpg",
      "front.jpg",
      "back.jpg"
   }; 
   readObj(State::resourceDirectory + "/skybox/cube.obj", mesh);
   createSky(State::resourceDirectory + "/skybox/", faces);
   createShader();
}
void Skybox::createSky(string dir, vector<string> faces) {
   glGenTextures(1, &textureID);
   glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
   int width, height, nrChannels;
   stbi_set_flip_vertically_on_load(false);
   for(GLuint i = 0; i < faces.size(); i++) {
      unsigned char *data =
         stbi_load((dir+faces[i]).c_str(), &width, &height, &nrChannels, 0);
      if (data) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
               0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      } else {
         cout << "failed to load: " << (dir+faces[i]).c_str() << endl;
      }
   }
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
   cout << " creating cube map any errors : " << glGetError() << endl;
} 
void Skybox::createShader(){
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/skybox/skybox_vert.glsl", State::resourceDirectory + "/skybox/skybox_frag.glsl");
   shader->init();
   shader->addUniform("P");
   shader->addUniform("V");
   shader->addUniform("M");
   shader->addUniform("skybox");
   shader->addAttribute("vertPos");
   shader->addAttribute("vertNor");
   shader->addAttribute("vertTex");
}
void Skybox::render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
      shader->bind();
      Model->scale(vec3(10000*State::scaler,10000*State::scaler,10000*State::scaler)); 
      glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
      glDepthFunc(GL_LEQUAL);
      glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
      glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
      mesh[0]->draw(shader);
      shader->unbind();
      glDepthFunc(GL_LESS);
   Model->popMatrix();
}
void Skybox::updateMaterial(){

}
/**********************/
/*  END SKYBO  CLASS  */
/**********************/
