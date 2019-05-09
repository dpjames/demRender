#include "objectDefs.h"
using namespace std;
using namespace glm;

void readJPG(string filename, unsigned char*& data, int *width, int *height, int *n){
   cout << "reading " << filename << endl;
   data = stbi_load(filename.c_str(), width, height, n, 1);
   if(data == NULL){
      perror("NULL??"); //TODO better error cmon;
      return;
   }
}
void readObj(string fname, vector<shared_ptr<Shape>> &mesh){
   vector<tinyobj::shape_t> tss;
   vector<tinyobj::material_t> omm;
   string e;
   bool rcc = tinyobj::LoadObj(tss, omm, e, fname.c_str());
   if(!rcc){
      cerr << e << endl;
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
   //unsigned char seen[width * height] = {};
   for(unsigned int y = 0; y < lcheight; y++){
      for(unsigned int x = 0; x < lcwidth; x++){
         int cindex = x + y * lcheight;
         unsigned char type = lcdata[cindex];
         unsigned char elev = demdata[cindex];
         shared_ptr<LandCover> block = make_shared<LandCover>();
         //block->init(
         //   vec3(0,0,0),
         //   vec3(1,1,1),
         //   vec3(10,elev,10),
         //   vec3(0,0,0),
         //   vec3(0,0,0),
         //   vec3(0,0,0),
         //   vec3(0,0,0),
         //   2,
         //   model
         //);
         blocks.push_back(block);
      }
   }
}
void GroundMap::render(shared_ptr<MatrixStack> Projection,
                  shared_ptr<MatrixStack> View,
                  shared_ptr<MatrixStack> Model){
   if(true){return;}
   Model->pushMatrix();
   for(int i = 0; i < blocks.size(); i++){
      blocks[i]->render(Projection, View, Model);
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
   topoVertex.push_back(y);
   topoVertex.push_back(z);
   //fun colors based on depth and things.
   topoColor.push_back(1 - (y / (float) height));
   topoColor.push_back(1 - zper);
   topoColor.push_back(1 - x / (float)width);
}
/*
 * use the image data pointer and the width/height to fill topoColor and topoVertex
 */
void Topo::fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height){
   //topoVertex 
   float scale = 4; //TODO make this passed in by init
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
   //TODO fix this here!!
   Model->scale(vec3(-1, 1, 1)); //TODO remove neg
   Model->pushMatrix();
   Model->translate(vec3(500,-40,30));
   Model->rotate(M_PI/2, vec3(0,1,0)); 
   Model->rotate(M_PI/-2, vec3(1,0,0));

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
/* BEGIN LANDCO CLASS */
/**********************/
void LandCover::init(glm::vec3 tTrans,   glm::vec3 tScale, glm::vec3 trans, 
                     glm::vec3 minscale, glm::vec3 maxscale, 
                     glm::vec3 minrot,   glm::vec3 maxrot, 
                     GLfloat n,          unsigned int landType){
   readObj(objname.c_str(), mesh);
   shader = shaderin;
   nchildren = n;
   maxTrans = trans;
   maxRotat = maxrot;
   minRotat = minrot;
   maxScale = maxscale;
   minScale = minscale;
   globalTrans = tTrans;
   globalScale = tScale;
   fillItems();
}
void LandCover::fillItems(){
   GLfloat tx,ty,tz,sx,sy,sz,rx,ry,rz;
   for(int i = 0; i < nchildren; i++){
      tx = maxTrans[0] * (rand() % 100000) / 100000; 
      ty = maxTrans[1] * (rand() % 100000) / 100000; 
      tz = maxTrans[2] * (rand() % 100000) / 100000; 
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
void Cover::init(glm::vec3 t, glm::vec3 s, glm::vec3 r){
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
