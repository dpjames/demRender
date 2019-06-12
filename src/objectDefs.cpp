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
   for(int i = 0; i < N_LAND_TYPES; i++){
      buffers.push_back(NULL);
   }
   generateMap(lcdata, demdata, lcwidth, lcheight, demwidth, demheight); 
}
void GroundMap::clearBuffers(){
   for(int i = 0; i < N_LAND_TYPES; i++){
      buffers[i] = NULL;
   }
}
void GroundMap::generateMap(unsigned char *lcdata, 
      uint32_t *demdata, 
      int lcwidth, 
      int lcheight, 
      int demwidth, 
      int demheight){
   cout << "generating ground map" << endl;
   //int DY = 1;
   //int DX = 1;
   int DY = 2;
   int DX = 2;
   float density = 7;
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
         if(buffers[type] == NULL){
            buffers[type] = make_shared<TypeBuffer>();
            buffers[type]->init(type);
         }
         buffers[type]->children.push_back(block);
         buffers[type]->nElements+=density;
      }
   }
   for(int i = 0; i < buffers.size(); i++){
      if(buffers[i] != NULL){
         initBufferArrays(i);
      }
   }
}
void GroundMap::initBufferArrays(int type){
   shared_ptr<TypeBuffer> buf = buffers[type];
   GLuint vertexArrayID;
   glGenVertexArrays(1, &vertexArrayID);
   glBindVertexArray(vertexArrayID);
   buf->vaoID = vertexArrayID;
   
   glGenBuffers(1, &(buf->vertexBufferID));
   glBindBuffer(GL_ARRAY_BUFFER, buf->vertexBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * buf->verticies.size(), buf->verticies.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 0,(void *) 0);

   //setup the normals
   glGenBuffers(1, &(buf->normalBufferID));
   glBindBuffer(GL_ARRAY_BUFFER, buf->normalBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->normals.size(), buf->normals.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
   
   //setup the textur coord
   glGenBuffers(1, &(buf->textureBufferID));
   glBindBuffer(GL_ARRAY_BUFFER, buf->textureBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->textures.size(), buf->textures.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &(buf->indiciesBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->indiciesBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buf->indicies.size()*sizeof(unsigned int), buf->indicies.data(), GL_DYNAMIC_DRAW);

   
   glGenBuffers(1, &(buf->tBufferID));
   glGenBuffers(1, &(buf->sBufferID));

   glBindVertexArray(0);


   for(int i = 0; i < buf->children.size(); i++){
      for(int j = 0; j < buf->children[i]->items.size(); j++){
         shared_ptr<Cover> item = buf->children[i]->items[j];
         vec3 trans = item->trans;
         vec3 scale = item->scale;
         buf->translations.push_back(trans[0]); 
         buf->translations.push_back(trans[1]); 
         buf->translations.push_back(trans[2]); 
         buf->scales.push_back(scale[0]); 
         buf->scales.push_back(scale[1]); 
         buf->scales.push_back(scale[2]); 
      }
   }
}
double timeoutside = 0;
double exitTime = 0;
double enterTime = 0;
double incount = 0;
double inenter = 0;
double renderStart = 0;
double renderTime = 0;
float dcount = 0;
float dcstart = 0;
void GroundMap::render(shared_ptr<MatrixStack> Projection,
      mat4 View,
      shared_ptr<MatrixStack> Model){
   glDisable(GL_CULL_FACE);
   renderAll((Projection->topMatrix()), View, Model->topMatrix());
   glEnable(GL_CULL_FACE);
}
void GroundMap::updateMaterial(){

}
void GroundMap::renderAll(mat4 P, mat4 V, mat4 M){
   for(int i = 0; i < N_LAND_TYPES; i++){
      renderType(i, P, V, M);
   }
}
void GroundMap::renderType(int type, mat4 P, mat4 V, mat4 M){
   shared_ptr<TypeBuffer> buf = buffers[type];
   if(buf == NULL || buf->verticies.size() == 0){
      return;
   }
   glBindVertexArray(buf->vaoID);
   buf->shader->bind();
   

   //setup the M mats
//   GLuint *mBufferID = buf->mID;

//   glEnableVertexAttribArray(3);
//   glBindBuffer(GL_ARRAY_BUFFER, mBufferID[0]);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->m1.size(), NULL, GL_DYNAMIC_DRAW);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->m1.size(), buf->m1.data());
//   glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,0,(void *) 0);
//	glVertexAttribDivisor(3, 1);
//
//   glEnableVertexAttribArray(4);
//   glBindBuffer(GL_ARRAY_BUFFER, mBufferID[1]);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->m1.size(), NULL, GL_DYNAMIC_DRAW);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->m2.size(), buf->m2.data());
//   glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,0,(void *) 0);
//	glVertexAttribDivisor(4, 1);
//
//   glEnableVertexAttribArray(5);
//   glBindBuffer(GL_ARRAY_BUFFER, mBufferID[2]);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->m1.size(), NULL, GL_DYNAMIC_DRAW);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->m3.size(), buf->m3.data());
//   glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,0,(void *) 0);
//	glVertexAttribDivisor(5, 1);
//
//   glEnableVertexAttribArray(6);
//   glBindBuffer(GL_ARRAY_BUFFER, mBufferID[3]);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->m1.size(), NULL, GL_DYNAMIC_DRAW);
//   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->m4.size(), buf->m4.data());
//   glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE,0,(void *) 0);
//	glVertexAttribDivisor(6, 1);


   glEnableVertexAttribArray(3);
   glBindBuffer(GL_ARRAY_BUFFER, buf->tBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->translations.size(), NULL, GL_DYNAMIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->translations.size(), buf->translations.data());
   glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,(void *) 0);
	glVertexAttribDivisor(3, 1);
   

   glEnableVertexAttribArray(4);
   glBindBuffer(GL_ARRAY_BUFFER, buf->sBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buf->scales.size(), NULL, GL_DYNAMIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * buf->scales.size(), buf->scales.data());
   glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,0,(void *) 0);
	glVertexAttribDivisor(4, 1);
   
   
   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, buf->vertexBufferID);
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 0,(void *) 0);

   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, buf->normalBufferID);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   glEnableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, buf->textureBufferID);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->indiciesBufferID);


   //the render
   glUniformMatrix4fv(buf->shader->getUniform("P"), 1, GL_FALSE, value_ptr(P));
   glUniformMatrix4fv(buf->shader->getUniform("V"), 1, GL_FALSE, value_ptr(V));
   glUniformMatrix4fv(buf->shader->getUniform("M"), 1, GL_FALSE, value_ptr(M));
   updateLights(buf->shader);
   buf->texture.bind(buf->shader->getUniform("Texture0"));
   glDrawElementsInstanced(GL_TRIANGLES, buf->indicies.size(), GL_UNSIGNED_INT, (const void *) 0, buf->nElements);
   buf->texture.unbind();
   buf->shader->unbind();
   glBindVertexArray(0);
}
/**********************/
/*  END GROUND CLASS  */
/**********************/

/**********************/
/* BEGIN TBUFF CLASS  */
/**********************/
void TypeBuffer::init(int t){
   type = t;
   LandType::getDrawDataForType(type, this->texture, this->mesh);
   this->shader = LandType::shader;
   fill();
   nElements = 0;
}
void TypeBuffer::fill(){
   int offset = 0;
   for(int j = 0; j < mesh.size(); j++){
      for(int i = 0; i < mesh[j]->posBuf.size(); i++){
         verticies.push_back(mesh[j]->posBuf[i]);
      }
      for(int i = 0; i < mesh[j]->norBuf.size(); i++){
         normals.push_back(mesh[j]->norBuf[i]);
      }
      for(int i = 0; i < mesh[j]->texBuf.size(); i++){
         textures.push_back(mesh[j]->texBuf[i]);
      }
      for(int i = 0; i < mesh[j]->eleBuf.size(); i++){
         indicies.push_back(mesh[j]->eleBuf[i] + offset);
      }
      offset = verticies.size();
   }
   if(textures.size() == 0 && mesh.size() != 0){ //if it doesnt have texture coordinates too bad.
      cout << "generating texture coordinates" << endl;
      for(int i = 0; i < verticies.size()/3 * 2; i++){
         textures.push_back(rand() % 100 / 100.0f);
      }
   }
}

void TypeBuffer::addMat(mat4 M){
   for(int i = 0; i < 4; i++){
      //m1.push_back(M[0][i]);
      //m2.push_back(M[1][i]);
      //m3.push_back(M[2][i]);
      //m4.push_back(M[3][i]);
   }
   nElements++;
}
/**********************/
/*  END TBUFF  CLASS  */
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
   glDrawArrays(GL_TRIANGLES, 0, topoVertex.size());
   glBindVertexArray(0);
   texture.unbind();
   shader->unbind(); 

   Model->popMatrix();
}
float Topo::getElevation(int x, int y){
   return (elevationData[x + y * width] + State::ztrans) * State::zscale;
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
      ld->texture.setUnit(0);
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
   shader->addAttribute("translation");
   shader->addAttribute("scale");
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
   minTrans = vec3(originx - DX * DX, elev, originy - DY * DY);
   maxTrans = vec3(originx + DX * DX, elev, originy + DY * DY);
   LandType::fillTransforms(landType, maxRotat, minRotat, maxScale, minScale);
   globalTrans = vec3(0,0,0); // this will prob be removed  TODO
   globalScale = vec3(1,1,1); // this will prob be removed
   globalRotat = vec3(0,0,0); // this will prob be removed
   fillItems(dem, demwidth, demheight, originx, originy);
   type = landType;
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
      shared_ptr<MatrixStack> Model,
      shared_ptr<TypeBuffer> buffer){
   //shader->bind();
   //updateLights(shader);
   //glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   //glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
   //texture.bind(shader->getUniform("Texture0"));
   for(unsigned int i = 0; i < items.size(); i++){
      items[i]->render(Model, shader, buffer);
   }
   //texture.unbind();
   //shader->unbind();

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
      shared_ptr<TypeBuffer> buffer){
   Model->pushMatrix();
   Model->translate(trans);
   Model->scale(scale);
   //Model->rotate(rotat[0], vec3(1,0,0));
   //Model->rotate(rotat[1], vec3(0,1,0));
   //Model->rotate(rotat[2], vec3(0,0,1));
   buffer->addMat(Model->topMatrix());
   //glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
   //for(unsigned int i = 0; i < mesh.size(); i++){
   //   mesh[i]->draw(shader);
   //}
   Model->popMatrix();

}
/**********************/
/*  END COVER  CLASS  */
/**********************/

/**********************/
/* Begin Player Class */
/**********************/
void Player::init(vec3 pos){
   velocity = vec3(0,0,.001);
   acceleration = vec3(0,0,0);
   position = pos;

   //model
   readObj(State::resourceDirectory + "/player/mesh.obj", mesh); //TODO cahnge  this
   readObj(State::resourceDirectory + "/player/ringMesh.obj", ringMesh); //TODO cahnge  this

   //shader
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/player_vert.glsl", State::resourceDirectory + "/player_frag.glsl");
   shader->init();
   shader->addUniform("P");
   shader->addUniform("V");
   shader->addUniform("M");
   shader->addAttribute("vertPos");
   shader->addAttribute("vertNor");
   shader->addAttribute("vertTex");
}
void Player::setRadius(float r){
   radius = r;
}
void Player::updateMaterial(){}
void Player::render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model){
   shader->bind(); 
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
   Model->pushMatrix();
      Model->translate(State::viewPosition / State::scaler);
      Model->pushMatrix();
         Model->translate(vec3(0,900,0));
         Model->scale(vec3(radius * 100) / State::scaler);
         glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
         ringMesh[0]->draw(shader);
      Model->popMatrix();

      Model->scale(vec3(10) / State::scaler);
      glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
      mesh[0]->draw(shader);
   Model->popMatrix();
   shader->unbind(); 
}
void Player::update(double dt){
   float fdt = (float) dt;
   position = position + (velocity * fdt + .5f * acceleration * fdt * fdt);
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
bool State::special = false;

//defaults
GLfloat State::zscale = .001;
int State::ztrans = 0;
int State::topoDetailLevel = 10;
string State::resourceDirectory = "../resources";
string State::placeDirectory = "/YOSEMITE_HD";
bool State::grounded = true;
bool State::capturedCursor = true;
void State::reset(){
   State::lightPos     = State::initLightPos;
   State::lightCol     = State::initLightCol;
   State::scaler       = State::initScaler;
   State::phi          = State::initPhi;
   State::theta        = State::initTheta;
   State::viewPosition = State::initViewPosition;
   State::special = false;
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
   glDisable(GL_CULL_FACE);
   Model->pushMatrix();
      shader->bind();
      Model->scale(vec3(100000 * State::scaler));
      glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
      glDepthFunc(GL_LEQUAL);
      glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
      glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
      mesh[0]->draw(shader);
      shader->unbind();
      glDepthFunc(GL_LESS);
   Model->popMatrix();
   glEnable(GL_CULL_FACE);
}
void Skybox::updateMaterial(){

}
/**********************/
/*  END SKYBO  CLASS  */
/**********************/


/**********************/
/*  END COLLEC CLASS  */
/**********************/

void Collectables::init(int number, shared_ptr<Topo> ground){
   readObj(State::resourceDirectory + "/coins/mesh.obj", mesh);
   for(int i = 0; i < number; i++){
      int x = rand() % ground->width;
      int z = rand() % ground->height;
      int y = ground->getElevation(x,z) + 5;
      positions.push_back(vec4(x,y,z,rand()%(int)(M_PI * 200) / 200.0f));
      baseHeights.push_back(y);
   }
   shader = make_shared<Program>();
   shader->setVerbose(true);
   shader->setShaderNames(State::resourceDirectory + "/coins/vert.glsl", State::resourceDirectory + "/coins/frag.glsl");
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
   setMaterial(shader, 2);
   texture.setFilename(State::resourceDirectory + "/coins/texture.jpg");
   texture.init();
   texture.setWrapModes(GL_REPEAT,GL_REPEAT);
}
//void Collectables::renderMinimap(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model){
//
//}
void Collectables::render(shared_ptr<MatrixStack> Projection,mat4 View,shared_ptr<MatrixStack> Model){
   shader->bind();
   texture.bind(shader->getUniform("Texture0"));
   updateLights(shader);
   Model->pushMatrix();
   glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(shader->getUniform("V"), 1, GL_FALSE, value_ptr(View));
   for(int i = 0; i < positions.size(); i++){
      Model->pushMatrix();
         Model->translate(vec3(positions[i]));
         Model->scale(vec3(.1,.1,.1));
         Model->rotate(rotationCounter + positions[i][3], vec3(0,1,0));
         Model->rotate(M_PI/2,vec3(0,0,1));
         glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
         mesh[0]->draw(shader);
      Model->popMatrix();
   }
   Model->popMatrix();
   texture.unbind();
   shader->unbind();
}
void Collectables::update(double dt){
   rotationCounter+=M_PI/3000 * dt;
   for(int i = 0; i < positions.size(); i++){
      positions[i][1] = baseHeights[i] + cos(rotationCounter + positions[i][3]) * 4;
      if(length(State::viewPosition / State::scaler - vec3(positions[i])) < 2){
         positions.erase(positions.begin()+i); 
         baseHeights.erase(baseHeights.begin()+i); 
      }
   }
   if(positions.size() == 0){
      State::special = true;
   }
}
void Collectables::updateMaterial(){

}
/**********************/
/* BEGIN COLLEC CLASS */
/**********************/
