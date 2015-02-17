
#include <windows.h>	   // Standard header for MS Windows applications
#include <glew.h>
#include <freeglut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <SOIL.h>
#include <unordered_map>
#include "Shader.h"
#include "MathHelper.h"
#include "Camera.h"
#include "Skybox.h"
#include "Cube.h"
#include "Quad.h"
#include "Text.h"
#include <iostream>
#include "Player.h"
using namespace std;

#pragma comment(lib, "glew32.lib")

#define KEY_ESCAPE 27

// struct to hold window and projection information
struct Window{
	GLuint id;       // glut window id
	char* title;     // window title

	int width;       // window width
	int height;      // window height
	float fovAngle;  // field of view for projection
	float zNear;     // near plane for projection
	float zFar;      // far plane for projection
};

char * VSFileName = "basicshader.vert";
char * FSFileName = "basicshader.frag";
char * VSTextureFileName = "textureShader.vert";
char * FSTextureFileName = "textureShader.frag";
char * VSSkyboxFileName = "skyboxShader.vert";
char * FSSkyboxFileName = "skyboxShader.frag";
char * VSSpriteShaderFileName = "spriteSheetVertexShader.vert";
char * FSSpriteShaderFileName = "spriteSheetFragmentShader.frag";
char * VSTextShaderFileName = "textShader.vert";
char * FSTextShaderFileName = "textShader.frag";



Window win1;
Shader  *colorShader = 0, *textureShader = 0, *cubeShader=0, *quadShader=0, *skyboxShader = 0, *textShader=0;
Matrix4 projectionMatrix = Matrix4::IDENTITY;
bool keys[256];
bool special[256];
//bool jump = false;
Vector3 playerPosition(0, 1, 0);
Vector3 cubePos(10, 1, 0);
float boxAccel = -0.28;
float boxVel = 300;

float XAxisRotationAngle = 0.0;
float YAxisRotationAngle = 0.0;
float ZAxisRotationAngle = 0.0;
Camera *camera1 = 0;
Skybox *skybox;
Cube *cube;
Quad * quad;
Text * text;

Player * player;

std::unordered_map<std::string, GLuint> images;


void updateProjection(Window win)
{
	glutSetWindow(win.id);                                                      // tell opengl which window you want to change
	win.height = (win.height < 1) ? 1 : win.height;                            // make sure we are not dividing by zero for the aspect
	glViewport(0, 0, win.width, win.height);								    // set the viewport

	GLfloat aspect = (GLfloat)win.width / win.height;               // calculate aspect ratio
	projectionMatrix = createPerspectiveProjectionMatrix(win.fovAngle, aspect, win.zNear, win.zFar); // set up a perspective projection matrix
}

void initShaders()
{

	colorShader = new Shader(VSFileName, FSFileName);
	textureShader = new Shader(VSTextureFileName, FSTextureFileName);
	skyboxShader = new Shader(VSSkyboxFileName, FSSkyboxFileName);
	textShader = new Shader(VSTextShaderFileName, FSTextShaderFileName);
	
	if (images["cubeTexture"]) cubeShader = textureShader;
	else cubeShader = colorShader;

	if (images["quadTexture"]) quadShader = new Shader(VSSpriteShaderFileName, FSSpriteShaderFileName);
	else quadShader = colorShader;

	
}

void handleInput(int deltatime)
{
	if (keys[' '] && !player->jumping )
	{
		player->jump = true;
	}


	float deltaseconds = deltatime / 1000.0f;
	if (keys['i']) player->position.z -= 10 * deltaseconds;
	if (keys['k']) player->position.z += 10 * deltaseconds;
	if (keys['j']) player->position.x -= 10 * deltaseconds;
	if (keys['l']) player->position.x += 10 * deltaseconds;

	if (keys['x']) XAxisRotationAngle += 30 * deltaseconds;
	if (keys['y']) YAxisRotationAngle += 30 * deltaseconds;
	if (keys['z']) ZAxisRotationAngle += 30 * deltaseconds;

	if (keys['n']) quad->advanceFrame();

	if (keys['w']) camera1->move(deltaseconds * 10);
	if (keys['s']) camera1->move(-deltaseconds * 10);
	if (keys['a']) camera1->strafe(-deltaseconds * 10);
	if (keys['d']) camera1->strafe(deltaseconds * 10);
	if (keys['r']) camera1->climb(deltaseconds * 10);
	if (keys['f']) camera1->climb(-deltaseconds * 10);

	

	if (keys['q']) camera1->roll(-deltaseconds * 100);
	if (keys['e']) camera1->roll(deltaseconds * 100);
	if (special[GLUT_KEY_LEFT]) camera1->yaw(deltaseconds * 100);
	if (special[GLUT_KEY_RIGHT]) camera1->yaw(-deltaseconds * 100);
	if (special[GLUT_KEY_UP]) camera1->pitch(deltaseconds * 100);
	if (special[GLUT_KEY_DOWN]) camera1->pitch(-deltaseconds * 100);
}


void renderWin1(Window win,int currentTime, int deltaTime)
{
	Matrix4 mvpMatrix, viewMatrix, modelMatrix;
	Matrix4 XAxisRotationMatrix, YAxisRotationMatrix, ZAxisRotationMatrix;

	handleInput(deltaTime);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer

	XAxisRotationMatrix = Matrix4::IDENTITY;
	YAxisRotationMatrix = Matrix4::IDENTITY;
	ZAxisRotationMatrix = Matrix4::IDENTITY;
	XAxisRotationMatrix.rotate(Vector3(1.0, 0.0, 0.0), XAxisRotationAngle);
	YAxisRotationMatrix.rotate(Vector3(0.0, 1.0, 0.0), YAxisRotationAngle);
	ZAxisRotationMatrix.rotate(Vector3(0.0, 0.0, 1.0), ZAxisRotationAngle);

	viewMatrix = camera1->getViewMatrix();

	//need to draw skybox before the quad with transparent parts
	skybox->render_self(projectionMatrix*viewMatrix, camera1->getPosition(), win1.zFar);


	if (player->jump)
		player->jumpPlayer();

	if ((player->position.x < (cube->position.x + 1) && player->position.x >(cube->position.x - 1))
		&&(player->position.y < 3.0/*&& player->position.y <= cube->position.y + 6*/) ){
		cout << cube->position.y + 3 << endl;
		player->landHeight = cube->position.y + 2;
	}
	else {
		//player->landHeight = 0;
	}
	//cout << "cubePos" << cube->position.y << endl;
	//cout << "playerPos" << player->position.y << endl;
	cout << "playerV" << player->velocity.y << endl;


	modelMatrix = translationMatrix(player->position);// *ZAxisRotationMatrix*YAxisRotationMatrix*XAxisRotationMatrix;
	mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	player->render_self(mvpMatrix);


	modelMatrix = translationMatrix(Vector3(cube->position));
	mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;


	cube->render_self(mvpMatrix);

	//quad->render_self(mvpMatrix);



	Matrix4 orthographicMatrix = Matrix4::IDENTITY;
	orthographicMatrix = createOrthographicProjectionMatrix(win.width, win.height, -1, 1);
	//text->setPosition();
	//text->render(orthographicMatrix, "HELLO BITCH");

	char buffer[256];
	sprintf(buffer, "Camera: %.2f, %.2f, %.2f",
		camera1->getPosition().x, camera1->getPosition().y, camera1->getPosition().z);
	std::string message(buffer);
	text->render(orthographicMatrix, message);
	
	glutSwapBuffers(); //works with GL_DOUBLE. use glFlush(); instead, if using GL_SINGLE
	player->updatePlayer(deltaTime);
}

void keyboardDown(unsigned char key, int mousePositionX, int mousePositionY)
{
	keys[key] = true;
	if (key == KEY_ESCAPE) glutExit();
}
void keyboardUp(unsigned char key, int mousePositionX, int mousePositionY)
{
	keys[key] = false;
}

void specialDown(int key, int mousePositionX, int mousePositionY)
{
	special[key] = true;
}
void specialUp(int key, int mousePositionX, int mousePositionY)
{
	special[key] = false;
}

void reshape(GLint newWidth, GLint newHeight) {
	win1.width = newWidth;
	win1.height = newHeight;

	updateProjection(win1);
}

// Function called when timer ends
void timer(int id) {
	glutPostRedisplay(); // Tell GLUT to call it's glutDisplayFunc
}

// Set new timer and render
void display(void)
{
	static int oldTime = 0;
	int currentTime = 0, deltaTime = 0;

	currentTime = glutGet(GLUT_ELAPSED_TIME);      //Time since beginning
	deltaTime = currentTime - oldTime;             //Time since last render
	oldTime = currentTime;                         //Set last render time

	glutTimerFunc(17, timer, 1);                   //Call timer function in at least 17 milliseconds
	glutSetWindow(win1.id);						   //Tell glut which window we are working on (only needed for multiple windows)
	renderWin1(win1,currentTime, deltaTime);            //Call our render function
}

// callback function when glut ends
void close()
{
	if (colorShader) delete colorShader;
	if (textureShader) delete textureShader;
	if (skyboxShader) delete skyboxShader;
	if (camera1) delete camera1;
	if (skybox) delete skybox;
	if (cube) delete cube;
	if (quad) delete quad;
	if (text) delete text;
	if (player) delete player;
}

void loadImages()
{
	images["cubeTexture"] = SOIL_load_OGL_texture("cubeTexture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["quadTexture"] = SOIL_load_OGL_texture("birdSheet.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["billboard"] = SOIL_load_OGL_texture("billboard.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
	images["skybox"] = SOIL_load_OGL_cubemap("skybox_xp.png", "skybox_xn.png", "skybox_yp.png", "skybox_yn.png", "skybox_zp.png", "skybox_zn.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		NULL
		);
	images["font1"] = SOIL_load_OGL_texture("courier.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);
}

void initObjects()
{
	text = new Text(textShader, images["font1"], 10, 15);
	text->setPosition(Vector3(-300, 175, 0));
	text->setColor(Color(0, 0.7, 0.3, 1.0));
	skybox = new Skybox(skyboxShader, images["skybox"]);
	player = new Player(cubeShader, images["cubeTexture"]);
	player->position = playerPosition;
	cube = new Cube(cubeShader, images["cubeTexture"]);
	cube->position = cubePos;
	int numberOfFrameInSprite = 8; //matches the birdSheet.png
	quad = new Quad(quadShader, images["quadTexture"], numberOfFrameInSprite);
}

void initialize()
{
	updateProjection(win1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.0, 0.0, 0.0, 1.0);											// specify clear values for the color buffers	
	camera1 = new Camera(Vector3(2, 2, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	
	loadImages();
	initShaders();
	initObjects();

	//needed for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char **argv)
{
	// set window values
	win1.width = 640;
	win1.height = 480;
	win1.title = "Tutorial 5";
	win1.fovAngle = 45;
	win1.zNear = 0.1f;
	win1.zFar = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  // Display Mode
	glutInitWindowSize(win1.width, win1.height);					// set window size
	win1.id = glutCreateWindow(win1.title);					    // create Window

	glutDisplayFunc(display);									// register Display Function
	glutKeyboardUpFunc(keyboardUp);								    // register Keyboard Handler
	glutKeyboardFunc(keyboardDown);								    // register Keyboard Handler
	glutSpecialUpFunc(specialUp);
	glutSpecialFunc(specialDown);
	glutCloseFunc(close);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}