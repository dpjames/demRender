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

	// Shape to be used (from  file) - modify to support multiple

	vector<shared_ptr<Shape>> birdMesh;
	vector<shared_ptr<Shape>> treeMesh;
   vector<vector<GLfloat>> trees;

   vector<shared_ptr<Renderable>> renderables;
   vector<shared_ptr<Updateable>> updateables;

   float moveDir[6] = {0,0,0,0,0,0}; //x,y,z,zrot,yrot,rot

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
		if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
         State::scaler*=2;
		}
		if (key == GLFW_KEY_B && action == GLFW_RELEASE) {
         State::scaler/=2;
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
         State::reset();
      }

      if(key == GLFW_KEY_J && action == GLFW_PRESS){ //light things
         State::lightPos[0]-=20;
      }
      if(key == GLFW_KEY_L && action == GLFW_PRESS){
         State::lightPos[0]+=20;
      }
      if(key == GLFW_KEY_I && action == GLFW_PRESS){
         State::lightPos[2]+=20;
      }
      if(key == GLFW_KEY_K && action == GLFW_PRESS){
         State::lightPos[2]-=20;
      }
      if(key == GLFW_KEY_U && action == GLFW_PRESS){
         State::lightPos[1]-=20;
      }
      if(key == GLFW_KEY_O && action == GLFW_PRESS){
         State::lightPos[1]+=20;
      }

      if(key == GLFW_KEY_M && action == GLFW_PRESS){
         renderables[0]->updateMaterial();
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
		}
	}
	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void init()
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
      
      LandType::init(); 
	}
   GLfloat LY = 10;
	void initSceneObjects(){
      shared_ptr<Topo> ground = make_shared<Topo>();
      ground->init(State::resourceDirectory + "/topo.jpg"); 
      renderables.push_back((shared_ptr<Renderable>)ground);
      
      shared_ptr<GroundMap> groundMap = make_shared<GroundMap>();
      groundMap->init(State::resourceDirectory + "/landcover.jpg", State::resourceDirectory + "/topo.jpg");
      renderables.push_back((shared_ptr<Renderable>)groundMap);
      
   }
   /*
    *
    */

   //updates the above variables according to how much time has passed since the last update.
   void moveView(double dt){
      dt*=1000;
      vec3 viewPosMod = vec3(0,0,0);
      vec3 viewRotMod = vec3(0,0,0);
      if(moveDir[0] != 0){
         viewPosMod[0] = -1*dt*moveDir[0];
      }
      if(moveDir[1] != 0){
         viewPosMod[1] = -1 * dt *moveDir[1];
      }
      if(moveDir[2] != 0){
         viewPosMod[2] =-1*dt*moveDir[2];
      }
      if(moveDir[3] != 0){
         viewRotMod[0]=dt*moveDir[3]/100;
      }
      if(moveDir[4] != 0){
         viewRotMod[1]+=dt*moveDir[4]/100;
      }
      if(moveDir[5] != 0){
         
      }
      State::viewPosition+=viewPosMod;
      State::viewRotation+=viewRotMod;
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
      View->rotate(State::viewRotation[0], vec3(1,0,0));
      View->rotate(State::viewRotation[1], vec3(0,1,0));
      View->translate(State::viewPosition);
      Model->pushMatrix();
         Model->loadIdentity();
         Model->scale(vec3(State::scaler, State::scaler, State::scaler));
         for(unsigned int i = 0; i < renderables.size(); i++){
            renderables[i]->render(Projection,View,Model);
         }
      Model->popMatrix();
	}
};
void setOpt(char opt, char *value){
   switch(opt){
      case 'z':
         State::zscale = stof(value);
         break;
      case 'd':
         State::topoDetailLevel = stoi(value);
         break;
      case 'r':
         State::resourceDirectory = value;
         break;
      default:
         cout << "option: '" << opt << "' not found, ignoring" << endl;
         break;
   }
}
void parseArgs(int argc, char ** argv){
   for(int i = 1; i <  argc - 1; i+=2){
      if(argv[i][0] != '-'){
         cout << "bad usage for switch: " << argv[i] << endl;
         cout << "ignoring" << endl;
         continue;
      }
      char opt = argv[i][1];
      cout << opt << ":" << argv[i + 1] << endl;
      setOpt(opt, argv[i+1]);
   }
}
int main(int argc, char *argv[])
{
	// Where the resources are loaded from
   parseArgs(argc, argv);
	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(1300, 1300);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
   
	application->init();
	//application->initGeom();

   application->initSceneObjects();
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
	   //cout << delta / 1000 << endl;
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
