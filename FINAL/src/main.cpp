/* Lab 5 base code - transforms using local matrix functions 
   to be written by students - <2019 merge with tiny loader changes to support multiple objects>
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
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

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> treeShade;

	// Shape to be used (from  file) - modify to support multiple

	GLuint topoVertexArrayID;
   vector<GLfloat> topoVertex;
   vector<GLfloat> topoColor;

	vector<shared_ptr<Shape>> birdMesh;
	vector<shared_ptr<Shape>> treeMesh;
   vector<vector<GLfloat>> trees;

	void createPerspectiveMat(float *m, float fovy, float aspect, float zNear, float zFar)
	{
   // http://www-01.ibm.com/support/knowledgecenter/ssw_aix_61/com.ibm.aix.opengl/doc/openglrf/gluPerspective.htm%23b5c8872587rree
		float f = 1.0f/glm::tan(0.5f*fovy);
		m[ 0] = f/aspect;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0;
		m[ 5] = f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;
		m[ 9] = 0.0f;
		m[10] = (zFar + zNear)/(zNear - zFar);
		m[11] = -1.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 2.0f*zFar*zNear/(zNear - zFar);
		m[15] = 0.0f;
	}
   
   float moveDir[6] = {0,0,0,0,0,0}; //x,y,z,zrot,yrot,rot
   float scaler = 1;

   /*
    * escape - closes application
    * Z - turns on wireframe
    * X - turns off wireframe
    * O - makes the world larger
    * I - makes the world smaller
    * W - looks upwards
    * S - looks downwards
    * A - looks left
    * D - looks right
    * E - moves upwrads
    * Q - moves downwards
    * UP ARROW - moves forwards
    * DOWN ARROW - moves backwards
    * RIGHT ARROW - moves right
    * LEFT ARROW - moves left
    */
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
      float delta = .1;
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
         scaler*=2;
		}
		if (key == GLFW_KEY_I && action == GLFW_RELEASE) {
         scaler/=2;
		}
      //WASD
      if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
         moveDir[2]=-1*delta;
      }
      if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
         moveDir[0]=-1*delta;
      }
      if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
         moveDir[2]=delta;
      }
      if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
         moveDir[0]=delta;
      }
      if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
         moveDir[1]=-1*delta;
      }
      if (key == GLFW_KEY_E && action == GLFW_PRESS) {
         moveDir[1]=1*delta;
      }
      if(key == GLFW_KEY_D && action == GLFW_PRESS){
         moveDir[4]=delta;
      }
      if(key == GLFW_KEY_A && action == GLFW_PRESS){
         moveDir[4]=-1*delta;
      }
      if(key == GLFW_KEY_W && action == GLFW_PRESS){
         moveDir[3]=-1*delta;
      }
      if(key == GLFW_KEY_S && action == GLFW_PRESS){
         moveDir[3]=delta;
      }


      if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
         moveDir[2]=0;
      }
      if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
         moveDir[0]=0;
      }
      if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
         moveDir[2]=0;
      }
      if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
         moveDir[0]=0;
      }
      if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
         moveDir[1]=0;
      }
      if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
         moveDir[1]=0;
      }
      if(key == GLFW_KEY_D && action == GLFW_RELEASE){
         moveDir[4]=0;
      }
      if(key == GLFW_KEY_A && action == GLFW_RELEASE){
         moveDir[4]=0;
      }
      if(key == GLFW_KEY_W && action == GLFW_RELEASE){
         moveDir[3]=0;
      }
      if(key == GLFW_KEY_S && action == GLFW_RELEASE){
         moveDir[3]=0;
      }

      if(key == GLFW_KEY_R && action == GLFW_PRESS){
         VX = 0;
         VY = 0;
         VZ = 0;
         VXROT = 0;
         VYROT = 0;
      }
	}
   /*
    * clicking just moves forward by one unit right now
    */
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
         VZ++;
		}
	}
	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertCol");

      treeShade = make_shared<Program>();
		treeShade->setVerbose(true);
		treeShade->setShaderNames(resourceDirectory + "/tree_vert.glsl", resourceDirectory + "/tree_frag.glsl");
		treeShade->init();
		treeShade->addUniform("P");
		treeShade->addUniform("V");
		treeShade->addUniform("M");
		treeShade->addAttribute("vertPos");
		treeShade->addAttribute("vertNor");
	}
   /*
    * read DEM from file and turn it into openGL useable data.
    */
   void readTopo(const std::string& resourceDirectory){
      int width = 3;
      int height = 4;
      int n = 5;
      unsigned char *data = stbi_load((resourceDirectory + "/topo.jpg").c_str(), &width, &height, &n, 1);
      if(data == NULL){
         perror("NULL??");
         return;
      }
      fillTopoArrays(data, width, height); 
   }
   /*
    * insert a point into the topo data arrays. this needs to be modified to change the initial orientation.
    * (the x, y and z axis are flipped around currently)
    */
   void insertPoint(float minz, float maxz, unsigned char *data, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
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
   void fillTopoArrays(unsigned char *data, unsigned int width, unsigned int height){
      //topoVertex 
      float scale = 4;
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

      //send color data to GPU
      GLuint topoColorBufferID;
      glGenBuffers(1, &topoColorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, topoColorBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * topoColor.size(), topoColor.data(), GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

      //bind null
		glBindVertexArray(0);


   }
   // reads in an obj and adds it to the end of mesh
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
   // initializes all the trees and their locations
   void initTrees(const std::string& resourceDirectory){
      readObj(resourceDirectory + "/plants/tree.obj", treeMesh);
      for(int i =0; i < 200; i+=2){
         for(int j = 0; j < 40; j+=2){
            float x = i*100 + rand() % 500;
            float y = j*100 + rand() % 500;
            float z = rand() % 2 + 1;
            vector<GLfloat> treeInfo = {x,y,0,1,1,z};
            trees.push_back(treeInfo);
         }
      }
   }
	void initBird(const std::string& resourceDirectory){
      readObj(resourceDirectory + "/bunny.obj", birdMesh);
      readObj(resourceDirectory + "/sphere.obj", birdMesh);
   }
	void initGeom(const std::string& resourceDirectory)
	{
      initTrees(resourceDirectory);
      initBird(resourceDirectory);
      readTopo(resourceDirectory);
	}
   //view x,y,z,y rotation,x rotation
   float VX = 0;    
   float VY = 0;
   float VZ = 0; 
   float VYROT = 0;
   float VXROT = 0;
   //scale the trees to make them dance
   float treeScale = 1;
   //rotate the bynny to make it fly in circles
   float bunnyRot = 0;
   /*
    *
    */

   //updates the above variables according to how much time has passed since the last update.
   void moveView(double dt){
      dt*=1000;
      if(moveDir[0] != 0){
         VX-=dt*moveDir[0];
      }
      if(moveDir[1] != 0){
         VY-=dt*moveDir[1];
      }
      if(moveDir[2] != 0){
         VZ-=dt*moveDir[2];
      }
      if(moveDir[3] != 0){
         VXROT+=dt*moveDir[3]/100;
      }
      if(moveDir[4] != 0){
         VYROT+=dt*moveDir[4]/100;
      }
      if(moveDir[5] != 0){
         
      }
      treeScale+=dt/500;
      bunnyRot+=dt/100;
   }
	void render()
	{
      //setup view and things
      int width, height;
      glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float aspect = width/(float)height;
      auto Projection = make_shared<MatrixStack>();
      auto View = make_shared<MatrixStack>();
      auto Model = make_shared<MatrixStack>();
      Projection->pushMatrix();
      Projection->perspective(45.0f, aspect, 0.01f, 100000.0f);
      View->pushMatrix();
      View->loadIdentity();
      View->rotate(VXROT, vec3(1,0,0));
      View->rotate(VYROT, vec3(0,1,0));
      View->translate(vec3(VX,VY,VZ));
      //end setup

      //main Model. setup the land base then draw everything relative to that. 
      Model->pushMatrix();
      Model->loadIdentity();
      Model->scale(vec3(-1 * scaler, 1 * scaler, 1 * scaler));
      Model->translate(vec3(500,-40,30));
      //The rotations are funky because of how the data was read in. 
      //This will later be changed to happen on data read to reduce overhead
      Model->rotate(M_PI/2, vec3(0,1,0)); 
      Model->rotate(M_PI/-2, vec3(1,0,0));
      
      //treeShade uses the normals to color. (given shaders);
      treeShade->bind();
      glUniformMatrix4fv(treeShade->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
      glUniformMatrix4fv(treeShade->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
      
      //draw the dancing trees!
      Model->pushMatrix();
         Model->translate(vec3(50, 400, 0));
         Model->scale(vec3(.05, .05, .05));
         for(unsigned int i = 0; i < trees.size(); i++){ //trees is an array of [tx,ty,tx,sx,sy,sz] This will be abstracted to a class later.
               Model->pushMatrix();
                  //So this math is fun, just wanted to make dancing trees thats all. sin(cos(x)) seemed fun enough
                  Model->scale(vec3(trees[i][3], trees[i][4],  .5 + (abs(sin(cos((treeScale + trees[i][5])))))));
                  Model->translate(vec3(trees[i][0], trees[i][1], trees[i][2]));
                  glUniformMatrix4fv(treeShade->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
                  treeMesh[0]->draw(treeShade); 
               Model->popMatrix();
         }
      Model->popMatrix();

      //bird bunny model
      Model->pushMatrix();
         //fly in a circle
         Model->translate(vec3(50 + sin(bunnyRot/10) * 80, 500 + cos(bunnyRot/10) * 80,20));
         //rotate to be facing forward while flying a circle
         Model->rotate((M_PI * bunnyRot/-33) + (3 * M_PI/4), vec3(0,0,1));
         Model->rotate(M_PI/2, vec3(1,0,0)); 
         //wing
         Model->pushMatrix();
            Model->translate(vec3(0,10,0));
            Model->rotate(M_PI/2 + sin(bunnyRot), vec3(1,0,0));
            Model->translate(vec3(0,-8,0));
            Model->scale(vec3(2.5,10,1));
            glUniformMatrix4fv(treeShade->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
            birdMesh[1]->draw(treeShade);
         Model->popMatrix();
         //wing
         Model->pushMatrix();
            Model->translate(vec3(0,10,0));
            Model->rotate(M_PI/2 + sin(bunnyRot), vec3(-1,0,0));
            Model->translate(vec3(0,-8,0));
            Model->scale(vec3(2.5,10,1));
            glUniformMatrix4fv(treeShade->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
            birdMesh[1]->draw(treeShade);
         Model->popMatrix();

         Model->scale(vec3(10,10,10));
         glUniformMatrix4fv(treeShade->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
         birdMesh[0]->draw(treeShade);
      Model->popMatrix();

      
      treeShade->unbind();

      prog->bind(); //this shader uses predefined color values.
      glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
      glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
      glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));

      //draw the topo
      glBindVertexArray(topoVertexArrayID);
      glDrawArrays(GL_TRIANGLES, 0, topoVertex.size() * 3);
      glBindVertexArray(0);
		prog->unbind();

	}
};

int main(int argc, char *argv[])
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";
   	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(1300, 1300);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);
	// Loop until the user closes the window.
   double lastTime = glfwGetTime();
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.

		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
      
      //find how much time has passed and update positions accordingly
      double now = glfwGetTime(); 
      double delta = now - lastTime;
      lastTime = now;
      application->moveView(delta);
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
