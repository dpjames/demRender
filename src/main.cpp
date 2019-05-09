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

#include "objectDefs.h"

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

	vector<shared_ptr<Shape>> birdMesh;
	vector<shared_ptr<Shape>> treeMesh;
   vector<vector<GLfloat>> trees;

   vector<shared_ptr<Renderable>> renderables;
   vector<shared_ptr<Updateable>> updateables;

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
      if(key == GLFW_KEY_L && action == GLFW_PRESS){
         cout << VX << "," << VY << "," << VZ << endl;
      }
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

      if(key == GLFW_KEY_Y && action == GLFW_PRESS){
         LY*=2;
         treeShade->bind(); 
         glUniform3f(treeShade->getUniform("lightPos"), 0, LY, 0);
         treeShade->unbind(); 
         cout << LY << endl;
      }
      if(key == GLFW_KEY_T && action == GLFW_PRESS){
         LY/=2;
         treeShade->bind(); 
         glUniform3f(treeShade->getUniform("lightPos"), 0, LY, 0);
         treeShade->unbind(); 
         cout << LY << endl;
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
      treeShade->addUniform("MatDif");
      treeShade->addUniform("lightColor");
      treeShade->addUniform("MatAmb");
      treeShade->addUniform("MatSpec");
      treeShade->addUniform("shine");
      treeShade->addUniform("lightPos");
		treeShade->addAttribute("vertPos");
		treeShade->addAttribute("vertNor");

      treeShade->bind(); //TODO mess with this material def
      glUniform3f(treeShade->getUniform("lightColor"), 1, 1,1);
      glUniform3f(treeShade->getUniform("lightPos"), 0, LY, 0);
      glUniform3f(treeShade->getUniform("MatAmb"), 0.02, .03, 0.02);
      glUniform3f(treeShade->getUniform("MatDif"), 0.03, 0.5, 0.01);
      //glUniform3f(treeShade->getUniform("MatSpec"), 0.025, 0.025, 0.025);
      glUniform3f(treeShade->getUniform("MatSpec"), 0.01, 0.02, 0.01);
      glUniform1f(treeShade->getUniform("shine"), .01);
      treeShade->unbind();


	}
   GLfloat LY = 10;
	void initSceneObjects(const std::string& resourceDirectory){
      shared_ptr<Topo> ground = make_shared<Topo>();
      ground->init(resourceDirectory + "/topo.jpg", prog); //TODO make this a cli arg
      renderables.push_back((shared_ptr<Renderable>)ground);
      
      shared_ptr<GroundMap> groundMap = make_shared<GroundMap>();
      groundMap->init(resourceDirectory + "/landcover.jpg", resourceDirectory + "/topo.jpg");
      renderables.push_back((shared_ptr<Renderable>)groundMap);
      
      //tree forest thing: 
      shared_ptr<LandCover> lc = make_shared<LandCover>();
      lc->init(
         vec3(30,-35,0),
         vec3(-1,1,1),
         vec3(100,0,-600),
         vec3(.5,.5,.5),
         vec3(.7,1.5,.7),
         vec3(0,0,0),
         vec3(0,0,0),
         600,
         treeShade,
         resourceDirectory + "/plants/tree.obj"
      );
      renderables.push_back((shared_ptr<Renderable>)lc);

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
      Model->pushMatrix();
         Model->loadIdentity();
         Model->scale(vec3(scaler, scaler, scaler)); //TODO remove neg
         for(unsigned int i = 0; i < renderables.size(); i++){
            renderables[i]->render(Projection,View,Model);
         }
      Model->popMatrix();
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
	//application->initGeom(resourceDir);

   application->initSceneObjects(resourceDir);
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
