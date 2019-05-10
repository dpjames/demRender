#include "objectDefs.h"
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::min;

using glm::vec4;
using glm::length;

void readJPG(string filename, unsigned char*& data, int *width, int *height, int *n){
   cout << "reading " << filename << endl;
   data = stbi_load(filename.c_str(), width, height, n, 1);
   if(data == NULL){
      perror(filename.c_str()); //TODO better error cmon;
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
   cout << demheight << "x" << demwidth << endl; 
   cout << lcheight << "x" << lcwidth << endl; 
   for(unsigned int y = 0; y < lcheight; y+=DY){
      for(unsigned int x = 0; x < lcwidth; x+=DX){
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
            vec3(wx-DX/2,0,wy-DY/2),
            vec3(wx+DX/2,0,wy+DX/2),
            vec3(.5,.5,.5),
            vec3(.5,1,.5),
            vec3(0,0,0),
            vec3(0,0,0),
            1,
            type,
            demdata,
            demwidth,
            demheight,
            wx,wy
         );
         blocks.push_back(block);
      }
      
   }
   cout << "exit" << endl;
}
void GroundMap::render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   for(int i = 0; i < blocks.size(); i++){
      if(length(blocks[i]->getPosition() + State::viewPosition) < 300){
         blocks[i]->render(Projection, View, Model);
      }
   }
   Model->popMatrix();
}
/**********************/
/*  END GROUND CLASS  */
/**********************/


/**********************/
/*  BEGIN TOPO CLASS  */
/**********************/
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
   float z = data[x + y * width];
   float zper = (z - minz) / (maxz-minz);
   topoVertex.push_back(x);
   topoVertex.push_back(z);
   topoVertex.push_back(y);
   //fun colors based on depth and things.
   topoColor.push_back(1 - (y / (float) height));
   topoColor.push_back(1 - x / (float)width);
   topoColor.push_back(1 - zper);
}
/*
 * use the image data pointer and the width/height to fill topoColor and topoVertex
 */
void Topo::fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height){
   //topoVertex 
   float scale = 10; //TODO make this passed in by init
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
         //TODO calculate texture point mapping. update colorArray to this instead
         if(x + scale < width){
            insertPoint(minZ,maxZ, data, x, y, width,height);
            insertPoint(minZ,maxZ, data, x, y + scale, width,height);
            insertPoint(minZ,maxZ, data, x + scale, y, width,height);
         }
         if(x >= scale){
            insertPoint(minZ,maxZ, data, x, y, width,height);
            insertPoint(minZ,maxZ, data, x, y + scale, width,height);
            insertPoint(minZ,maxZ, data, x - scale, y + scale, width,height);
         }
      }
   }
   //send triganle data to GPU
   GLuint topoVertexBufferID;
   glGenVertexArrays(1, &topoVertexArrayID);
   glBindVertexArray(topoVertexArrayID);
   glGenBuffers(1, &topoVertexBufferID);
   glBindBuffer(GL_ARRAY_BUFFER, topoVertexBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoVertex.size(), topoVertex.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
   //send color data to GPU //TODO calc the normals here instead of color vals!
   GLuint topoColorBufferID;
   glGenBuffers(1, &topoColorBufferID);
   glBindBuffer(GL_ARRAY_BUFFER, topoColorBufferID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoColor.size(), topoColor.data(), GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
   //bind null
   glBindVertexArray(0);
}
void Topo::init(string filename, shared_ptr<Program> p){
   prog = p;
   readTopo(filename);
}
void Topo::render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model){
   Model->pushMatrix();
   prog->bind();
   glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
   glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
   glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
   glBindVertexArray(topoVertexArrayID);
   glDrawArrays(GL_TRIANGLES, 0, topoVertex.size() * 3);
   glBindVertexArray(0);
   prog->unbind(); 
   
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
   readObj("../resources/plants/tree.obj", mesh); //TODO generalize to all mesh
   shader = make_shared<Program>();
	shader->setVerbose(true);
	shader->setShaderNames("../resources/tree_vert.glsl", "../resources/tree_frag.glsl");
	shader->init();
	shader->addUniform("P");
	shader->addUniform("V");
	shader->addUniform("M");
   shader->addUniform("MatDif");
   shader->addUniform("lightColor");
   shader->addUniform("MatAmb");
   shader->addUniform("MatSpec");
   shader->addUniform("shine");
   shader->addUniform("lightPos");
	shader->addAttribute("vertPos");
	shader->addAttribute("vertNor");

   shader->bind(); //TODO mess with this material def
   glUniform3f(shader->getUniform("lightColor"), 1, 1,1);
   glUniform3f(shader->getUniform("lightPos"), 0, 10, 0);
   glUniform3f(shader->getUniform("MatAmb"), 0.02, .03, 0.02);
   glUniform3f(shader->getUniform("MatDif"), 0.03, 0.5, 0.01);
   //glUniform3f(shader->getUniform("MatSpec"), 0.025, 0.025, 0.025);
   glUniform3f(shader->getUniform("MatSpec"), 0.01, 0.02, 0.01);
   glUniform1f(shader->getUniform("shine"), .01);
   shader->unbind();

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
      if((int)tx + (int)tz * width > width * height){
         cout << "oh no" << endl;
      }
      ty = elev[(int)tx + (int)tz * width];
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
      if(true || distance((vec3)Model->topMatrix()[3], -1.0f * State::viewPosition) <= 100 * State::scaler){
         glUniformMatrix4fv(shader->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
         for(unsigned int i = 0; i < mesh.size(); i++){
            mesh[i]->draw(shader);
         }
      }
   Model->popMatrix();

}
/**********************/
/*  END COVER  CLASS  */
/**********************/



/**********************/
/* Begin State CLASS  */
/**********************/
vec3 State::viewPosition = vec3(0,-13,-535);
vec3 State::viewRotation = vec3(0,M_PI/2,0);
float State::scaler = 1;
/**********************/
/*  END State  CLASS  */
/**********************/
