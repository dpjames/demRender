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

   shared_ptr<Topo> ground;
   shared_ptr<Player> player; 
   shared_ptr<Collectables> coins;
   vector<shared_ptr<Renderable>> renderables;
   vector<shared_ptr<Updateable>> updateables;

   float moveDir[6] = {0,0,0,0,0,0}; //x,y,z,zrot,yrot,rot
   void keyCallback(GLFWwindow *window, int key, int scancode, int action, int modes){
      float delta = .1;
      if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
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
      if(key == GLFW_KEY_M && action == GLFW_PRESS){
         renderables[0]->updateMaterial();
      }
      if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
         State::grounded = !State::grounded;
      }
      //WASD EQ
      if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
         moveDir[1]=-1*delta;
      }
      if (key == GLFW_KEY_E && action == GLFW_PRESS) {
         moveDir[1]=1*delta;
      }
      if(key == GLFW_KEY_W && action == GLFW_PRESS){
         moveDir[2]=-1*delta;
      }
      if(key == GLFW_KEY_S && action == GLFW_PRESS){
         moveDir[2]=delta;
      }
      if(key == GLFW_KEY_D && action == GLFW_PRESS){
         moveDir[0]=delta;
      }
      if(key == GLFW_KEY_A && action == GLFW_PRESS){
         moveDir[0]=-1*delta;
      }
      if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
         moveDir[1]=0;
      }
      if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
         moveDir[1]=0;
      }
      if(key == GLFW_KEY_W && action == GLFW_RELEASE){
         moveDir[2]=0;
      }
      if(key == GLFW_KEY_S && action == GLFW_RELEASE){
         moveDir[2]=0;
      }
      if(key == GLFW_KEY_D && action == GLFW_RELEASE){
         moveDir[0]=0;
      }
      if(key == GLFW_KEY_A && action == GLFW_RELEASE){
         moveDir[0]=0;
      }
      if(key == GLFW_KEY_R && action == GLFW_PRESS){
         State::reset();
      }
   }
   /*
    * clicking just moves forward by one unit right now
    */
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
         State::capturedCursor = !State::capturedCursor;
         if(State::capturedCursor){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //TODO change this to diabled instead... documentation says this is better
         } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         }
		}
	}

   void cursorCallback(GLFWwindow *window, double xpos, double ypos){
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      double scale = 900; //State::sensitivity ?? TODO
      double px = width / 2.0;
      double py = height / 2.0;
      double dx = xpos - px;
      double dy = py - ypos;
      State::theta+=dx/scale;
      State::phi  +=dy/scale;
      double rad85 = .47 * M_PI;
      if(State::phi > rad85){
         State::phi = rad85;
      } else if(State::phi < -1 * rad85){
         State::phi = -1 * rad85; 
      }
      if(State::capturedCursor){
         glfwSetCursorPos(window, width/2, height/2);
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
		glClearColor(0,0,0,0);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE); 
      glCullFace(GL_BACK);
	}
   GLfloat LY = 10;
	void initSceneObjects(){

      LandType::init(); 

      ground = make_shared<Topo>();
      ground->init(State::resourceDirectory + State::placeDirectory + "/topo.bin"); 
      State::initViewPosition = vec3(ground->width/2, 0, ground->height/2);
      State::viewPosition = State::initViewPosition;
      renderables.push_back((shared_ptr<Renderable>)ground);


      shared_ptr<GroundMap> groundMap = make_shared<GroundMap>();
      groundMap->init(State::resourceDirectory + State::placeDirectory + "/landcover.jpg", ground->elevationData, ground->width, ground->height);
      renderables.push_back((shared_ptr<Renderable>)groundMap);


      shared_ptr<Skybox> sky = make_shared<Skybox>();
      sky->init();
      renderables.push_back((shared_ptr<Renderable>)sky);
      
      player = make_shared<Player>();
      player->init(State::viewPosition);

      coins = make_shared<Collectables>();
      coins->init(10, ground);
      renderables.push_back((shared_ptr<Renderable>) coins);
      updateables.push_back((shared_ptr<Updateable>) coins);
      
   }
   /*
    *
    */

   //updates the above variables according to how much time has passed since the last update.
   void moveView(double dt){
      dt*=1000;

      player->update(dt);
      vec3 viewPosMod = vec3(0,0,0);
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
         State::theta+=dt*moveDir[3]/100;
         //viewRotMod[0]=dt*moveDir[3]/100;
      }
      if(moveDir[4] != 0){
         State::phi+=dt*moveDir[4]/100;
         //viewRotMod[1]+=dt*moveDir[4]/100;
      }
      if(moveDir[5] != 0){
         
      }
      vec3 w = -1.0f * normalize(vec3(cos(State::phi) * cos(State::theta),sin(State::phi),cos(State::phi) * cos((M_PI/2) - State::theta)));
      vec3 v = vec3(0,1,0);
      vec3 u = cross(v,w);

      vec3 x = vec3(1,0,0);
      vec3 y = vec3(0,1,0);
      vec3 z = vec3(0,0,1);
      
      mat3 transmat = mat3(
         dot(u,x), dot(u,y), dot(u,z),
         dot(v,x), dot(v,y), dot(v,z),
         dot(w,x), dot(w,y), dot(w,z)
      );
      State::viewPosition-=(transmat * viewPosMod);
      //State::lightPos = worldRad * vec3(cos(phicounter) * cos(thetacounter),sin(phicounter),cos(phicounter) * cos((M_PI/2) - thetacounter));
      //thetacounter+=M_PI/dt/10;
      //phicounter+=M_PI/10000 * dt;
      //thetacounter+=M_PI/400000 * dt;
      radiusCounter += M_PI/1000 * dt;

      if(State::grounded){
         moveToGround();
      }

      for(unsigned int i = 0; i < updateables.size(); i++){
         updateables[i]->update(dt);
      }
   }
   void moveToGround(){
      int index = 
         ((int)(State::viewPosition[0] / State::scaler)) + 
         ((int)(State::viewPosition[2] / State::scaler) * ground->width);
      if(index >= ground->width * ground->height 
            || index < 0 
            || State::viewPosition[0] < 0 
            || State::viewPosition[0] >= ground->width * State::scaler
            || State::viewPosition[1] < 0
            || State::viewPosition[1] >= ground->height * State::scaler){
         return;
      } 
      State::viewPosition[1] = (ground->elevationData[index] + State::ztrans) * State::zscale * State::scaler + 3; //the 1 is the player height. right now 1 unit. TODO

   }
   float worldRad = 1000;
   float thetacounter = 0;
   float phicounter = 0;
   float radiusCounter = 1;
	void render()
	{
      //setup view and things
      int width, height;
      glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float aspect = width/(float)height;
      auto Projection = make_shared<MatrixStack>();
      auto Model = make_shared<MatrixStack>();
      Projection->pushMatrix();
      Projection->perspective(45.0f, aspect, 0.01f, 100000.0f * State::scaler);
      float radius = 2;
      vec3 viewDir = normalize(vec3(
         radius * cos(State::phi) * cos(State::theta),
         radius * sin(State::phi),
         radius * cos(State::phi) * cos((M_PI/2) - State::theta)
      ));

      mat4 View = glm::lookAt(State::viewPosition, State::viewPosition + viewDir, vec3(0,1,0));
      Model->pushMatrix();
         Model->loadIdentity();
         Model->scale(vec3(State::scaler, State::scaler, State::scaler));
         for(unsigned int i = 0; i < renderables.size(); i++){
            renderables[i]->render(Projection,View,Model);
            if(State::special){
               renderables[i]->updateMaterial();
            }
         }
         drawMinimap(width, height, Model, Projection, State::theta + M_PI / -2);
      Model->popMatrix();
	}
   void drawMinimap(int width, 
         int height, 
         shared_ptr<MatrixStack> Model,
         shared_ptr<MatrixStack> Projection,
         float angle){
		glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
      glViewport(0,0, width/6 + width/1000 * 2, height/6 + height/1000 * 2);
      glEnable(GL_SCISSOR_TEST);
      glScissor(0,0, width/6 + width/1000 * 2, height/6 + height/1000 * 2);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);
      glViewport(width/1000,width/1000, width/6, height/6);
      vec3 top = State::viewPosition;
      top[1] = 1000 * State::scaler;
      mat4 View = glm::lookAt(top, State::viewPosition,  vec3(0,0,1));
      Projection->ortho(ground->width/-2,ground->width/2,ground->height/-2,ground->height/2, .01, 10000 * State::scaler);
      Model->pushMatrix();
         Model->translate(State::viewPosition / State::scaler);
         Model->rotate(angle,vec3(0,1,0) );
         Model->translate(State::viewPosition * -1.0f / State::scaler);
         ground->render(Projection, View, Model);
         player->setRadius(.2 + .1 * (1 + cos(radiusCounter)));
         player->render(Projection, View, Model);
         coins->render(Projection,View,Model);
      Model->popMatrix();
      glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
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
      case 'p':
         State::placeDirectory = "/" + string(value);
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
