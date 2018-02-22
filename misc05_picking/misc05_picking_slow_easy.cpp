// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Include AntTweakBar
#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include<Windows.h>
#include<stdio.h>
#include<io.h>
#include<fcntl.h>
#include<conio.h>
#include"common/al.h"
#include"common/alc.h"
#include"common\Framework.h"
#include<common\tga.h>
const int window_width = 1024, window_height = 768;
#define	LEFT_WAVE_FILE		"Avengers1.wav"
#define	RIGHT_WAVE_FILE		"Avengers1.wav"
char *left_file = "Avengers1.wav";
char *right_file = "Avengers2.wav";
typedef struct point {
	float x, y, z;
	point(const float x = 0, const float y = 0, const float z = 0) : x(x), y(y), z(z) {};
	point(float *coords) : x(coords[0]), y(coords[1]), z(coords[2]) {};
	point operator -(const point& a)const {
		return point(x - a.x, y - a.y, z - a.z);
	}
	point operator +(const point& a)const {
		return point(x + a.x, y + a.y, z + a.z);
	}
	point operator *(const float& a)const {
		return point(x*a, y*a, z*a);
	}
	point operator /(const float& a)const {
		return point(x / a, y / a, z / a);
	}
	float* toArray() {
		float array[] = { x, y, z, 1.0f };
		return array;
	}
};
typedef struct moive {
	char* imgpath;
	char* leftsound;
	char* rightsound;
	moive(char *imgpath, char* leftsound,char *rightsound) :imgpath(imgpath), leftsound(leftsound), rightsound(rightsound) {};
};
typedef struct uvpoint {
	float u, v;
	uvpoint(const float x = 0, const float y = 0) : u(x), v(y) {};
	uvpoint(float *coords) : u(coords[0]), v(coords[1]) {};
	uvpoint operator -(const uvpoint& a)const {
		return uvpoint(u - a.u, v - a.v);
	}
	uvpoint operator +(const uvpoint& a)const {
		return uvpoint(u + a.u, v + a.v);
	}
	uvpoint operator *(const float& a)const {
		return uvpoint(u*a, v*a);
	}
	uvpoint operator /(const float& a)const {
		return uvpoint(u / a, v / a);
	}
	float* toArray() {
		float array[] = { u,v };
		return array;
	}
};
#define pi 3.1415926
typedef struct Vertex {
	float Position[4];
	float Color[4];
	float Normal[3];
	float UV[2];
	void SetPosition(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = 1.0;
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
	void SetNormal(float *coords) {
		Normal[0] = coords[0];
		Normal[1] = coords[1];
		Normal[2] = coords[2];
	}
	void SetUV(float *coords) {
		UV[0] = coords[0];
		UV[1] = coords[1];
	}
};

// function prototypes
int initWindow(void);
void initOpenGL(void);
void loadObject(char*, glm::vec4, Vertex * &, GLushort* &, int);
void createVAOs(Vertex[], GLushort[], int);
void createObjects(void);
void pickObject(void);
void renderScene(void);
void renderTexture(void);
void cleanup(void);
static void keyCallback(GLFWwindow*, int, int, int, int);
static void mouseCallback(GLFWwindow*, int, int, int);
float oldxpos;
float oldypos;
float oldxposflag = false;
float oldyposflag = false;
// GLOBAL VARIABLES
GLFWwindow* window;
GLuint Texture;
bool subflag = false;
// Get a handle for our "myTextureSampler" uniform
GLuint TextureID;
glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;

GLuint gPickedIndex = -1;
std::string gMessage;

GLuint programID;
GLuint pickingProgramID;
GLuint textureProgramID;
const GLuint NumObjects = 9;	// ATTN: THIS NEEDS TO CHANGE AS YOU ADD NEW OBJECTS
GLuint VertexArrayId[NumObjects] = { 0,1,2,3,4,5,6,7,8 };
GLuint VertexBufferId[NumObjects] = { 0,1,2,3,4,5,6,7,8 };
GLuint IndexBufferId[NumObjects] = { 0,1,2,3,4,5,6,7,8 };

size_t NumIndices[NumObjects] = { 0,1,2,3,4,5,6,7,8 };
size_t VertexBufferSize[NumObjects] = { 0,1,2,3,4,5,6,7,8 };
size_t IndexBufferSize[NumObjects] = { 0,1,2,3,4,5,6,7,8 };

GLuint MatrixID;
GLuint ModelMatrixID;
GLuint ViewMatrixID;
GLuint ProjMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorID;
GLuint pickingColorArrayID;
GLuint textureMatrixID;
GLuint LightID;
GLuint LightID2;
glm::vec3 lightPos = glm::vec3(4, 4, 4);
glm::vec3 lightPos2 = glm::vec3(11, 10, 9);
glm::mat4 rotationMat(1);
GLint gX = 0.0;
GLint gZ = 0.0;
bool faceflag = true;
// animation control
bool animation = false;
GLfloat phi = 0.0;
GLuint TextureNumber;
bool cflag = true;
long *TexWidth = 0;        //texture width
long *TexHeight = 0;	   //texture width
float pickingColor[255] = { 0 };
Vertex GCTRVerts[442] = { 0 };              //Draw green control points
Vertex XCTRVerts[441] = { 0 };				//Draw mesh on X direction
Vertex YCTRVerts[441] = { 0 };				//Draw mesh on Y direction
Vertex ACTRVerts[1600] = { 0 };
Vertex SCTRVerts[3249] = { 0 };
Vertex STextureVerts[12544] = { 0 };
Vertex TextureVerts[2400] = { 0 };
unsigned short CTRIndices[] = { 0 };        //Indices for green control points
const size_t CTRIndexCount = sizeof(CTRIndices) / sizeof(unsigned short);
GLuint vertexPosition_modelspaceID;
GLuint vertexUVID;
bool flag = false;
int posIndex;
// Position of source  
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
ALfloat SourcePos2[] = { 0.0, 0.0, 0.0 };
// Speed of source  
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };


// Position of listener
ALfloat ListenerPos[] = { 20.0, 0.0, 5.0 };

// Speed of listener
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Direction of listener (first 3 elements are "at", second 3 are "up")  
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
ALuint      uiBuffer;
ALuint      uiSource;
ALuint      uiBuffer2;
ALuint      uiSource2;
ALint       iState;
moive movies[] = {
	{"Avengers.bmp","Avengers1.wav","Avengers2.wav"},
	{"Shawshank.bmp","Shawshank1.wav","Shawshank2.wav"},
	{"Zootopia.bmp","Zootopia1.wav","Zootopia2.wav"}
	
};

void createObjects(void)
{
	//-- COORDINATE AXES --//
	Vertex CoordVerts[] =
	{
		{ { 0.0, 0.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, 0.0, 1.0 },{ 1.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 5.0, 0.0, 1.0 },{ 0.0, 1.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 5.0, 1.0 },{ 0.0, 0.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	};

	VertexBufferSize[0] = sizeof(CoordVerts);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(CoordVerts, NULL, 0);

	//-- GRID --//

	// ATTN: create your grid vertices here!
	Vertex *GridVerts = new Vertex[84];
	for (int i = 0; i < 84; i++) {
		float color[4] = { 1.0, 1.0, 1.0, 1.0 };
		float normal[3] = { 0.0, 0.0, 1.0 };
		GridVerts[i].SetColor(color);
		GridVerts[i].SetNormal(normal);
	}
	for (int i = 0; i < 21; i++) {
		float pos[4] = { -5 + 0.5*i,0,5,1 };
		GridVerts[2 * i].SetPosition(pos);
		pos[2] = -5.0;
		GridVerts[2 * i + 1].SetPosition(pos);
		float pos1[4] = { 5,0,-5 + 0.5*i,1 };
		GridVerts[2 * i + 42].SetPosition(pos1);
		pos1[0] = -5;
		GridVerts[2 * i + 43].SetPosition(pos1);
	}
	VertexBufferSize[1] = sizeof(GridVerts[0]) * 84;
	createVAOs(GridVerts, NULL, 1);
	//-- .OBJs --//
	

	Vertex screen[] = {
		{ { -5.0, 8.5, -8.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { -5.0, 1.0,-8.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 1.0, -8.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
		{ { 5.0, 8.5, -8.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	};
	VertexBufferSize[2] = sizeof(screen) ;
	createVAOs(screen, NULL, 2);


	//Texture = load_texture_TGA("akiyo_qcif.yuv", TexWidth, TexHeight, GL_CLAMP, GL_CLAMP);
	Texture = loadBMP_custom(movies[0].imgpath);
	Vertex screen2[] = {

		
		
		{ { 5.0, 8.5, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } ,{ 1,1 } },
		
		{ { -5.0, 8.5, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 0,1 } },
		{ { 5.0, 1.0, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 1,0 } },
		{ { -5.0, 1.0,-10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 0,0 } },
		
	};
	VertexBufferSize[3] = sizeof(screen2);
	createVAOs(screen2, NULL, 3);

	Vertex screen3[] = {


		
		{ { 5.0, 1.0, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 1,0 } },

		{ { 5.0, 8.5, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } ,{ 1,1 } },
		{ { -5.0, 8.5, -10.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 0,1 } },
		
		
	};
	VertexBufferSize[6] = sizeof(screen3);
	createVAOs(screen3, NULL, 6);
	// ATTN: load your models here
	//Vertex* Verts;
	//GLushort* Idcs;                     
	//loadObject("models/base.obj", glm::vec4(1.0, 0.0, 0.0, 1.0), Verts, Idcs, ObjectID);
	//createVAOs(Verts, Idcs, ObjectID);
	Vertex *SeatVerts = new Vertex[100];
	float color[4] = { 1.0, 1.0, 1.0, 1.0 };
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			SeatVerts[i * 10 + j].SetColor(color);
			float pos[4] = { j,9-i,0,1 };
			SeatVerts[i * 10 + j].SetPosition(pos);
		}
	}

	VertexBufferSize[4] = sizeof(SeatVerts[0])*100;	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(SeatVerts, NULL, 4);

	for (int i = 0; i < 100; i++) {
		SeatVerts[i].Color[0] = (float)(i+1) / 255;
	}
	VertexBufferSize[5] = sizeof(SeatVerts[0]) * 100;	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(SeatVerts, NULL, 5);
}
glm::vec3 convert(int index) {
	glm::vec3 a;
	int i = (index / 10)+1;
	int j = index % 10;
	
	if (j == 0) {
		i--;
		j = 10;
	}
	//printf("%d,%d", i, j);
	a.x = -5 + j - 1;
	a.z = -5 + i - 1;
	a.y = 1+(i-1)*0.41;
	return a;
}
glm::vec2 rowcol(int index) {
	glm::vec2 a;
	int i = (index / 10) + 1;
	int j = index % 10;

	if (j == 0) {
		i--;
		j = 10;
	}
	a.x = j;
	a.y = i;
	return a;
}
void renderScene(void)
{
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!


	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Re-clear the screen for real rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
		/**/
		glm::mat4x4 ModelMatrix = glm::mat4(1.0);

		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		
		if (flag) {
			glBindVertexArray(VertexArrayId[0]);	// draw CoordAxes
			//glDrawArrays(GL_LINES, 0, 6);
			glBindVertexArray(VertexArrayId[1]);	// draw CoordAxes
			//glDrawArrays(GL_LINES, 0, 84);
			glBindVertexArray(VertexArrayId[2]);	// draw CoordAxes
			//glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
		else {
			glBindVertexArray(VertexArrayId[4]);	// draw CoordAxes
			glEnable(GL_PROGRAM_POINT_SIZE);
			glDrawArrays(GL_POINTS, 0, 100);
		}
		
		//glDrawArrays(GL_LINE_LOOP, 396, 4);

		glBindVertexArray(0);

	}
	glUseProgram(0);
	// Draw GUI
	TwDraw();

	// Swap buffers
	
}
void renderTexture(void)
{
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!


	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(textureProgramID);
	{


		glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
		glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(textureMatrixID, 1, GL_FALSE, &MVP[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);
		if (flag) {
			glBindVertexArray(VertexArrayId[3]);	// draw CoordAxes
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(VertexArrayId[6]);	// draw CoordAxes
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 36); // 12*3 indices starting at 0 -> 12 triangles
		/*if (!subflag) {
			glBindVertexArray(VertexArrayId[4]);

			glDrawArrays(GL_TRIANGLES, 0, 2400);
		}
		else {
			glBindVertexArray(VertexArrayId[5]);
			for (int i = 0; i < 12544; i += 4) {
				glDrawArrays(GL_TRIANGLE_STRIP, i, 4); // 12*3 indices starting at 0 -> 12 triangles
			}

		}*/

		//glBindVertexArray(VertexArrayId[4]);

		//glDrawArrays(GL_TRIANGLES, 0, 2400); // 12*3 indices starting at 0 -> 12 triangles




		glBindVertexArray(0);

	}
	glUseProgram(0);
	// Draw GUI
	TwDraw();

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}
void pickObject()
{
	//flag = !flag;

	if (flag) {
		gViewMatrix = glm::lookAt(glm::vec3(4.5, 4.5, 20.0f),	// eye
			glm::vec3(4.5, 4.5, 0.0),	// center
			glm::vec3(0.0, 1.0, 0.0));	// up
		flag = !flag;
	}
	else {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(pickingProgramID);
		{
			glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
			glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, in the "MVP" uniform
			glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniform1fv(pickingColorArrayID, 441, pickingColor);
			// ATTN: DRAW YOUR PICKING SCENE HERE. REMEMBER TO SEND IN A DIFFERENT PICKING COLOR FOR EACH OBJECT BEFOREHAND
			if (!flag) {
				glBindVertexArray(VertexArrayId[5]);	// draw Grids
				glEnable(GL_PROGRAM_POINT_SIZE);
				glDrawArrays(GL_POINTS, 0, 100);
			}



			glBindVertexArray(0);

		}
		glUseProgram(0);
		// Wait until all the pending drawing commands are really done.
		// Ultra-mega-over slow ! 
		// There are usually a long time between glDrawElements() and
		// all the fragments completely rasterized.
		//glFlush();
		//glFinish();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glm::vec4 vp = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);

		// retrieve your cursor position
		// get your world coordinates
		// move points


		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		unsigned char data[4];

		glReadPixels(xpos, 768 - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

																			   // Convert the color back to an integer ID
		int pickedID = int(data[0]);
		float zdepth;
		glReadPixels(xpos, 768 - ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdepth);
		if (pickedID == 255) { // Full white, must be the background !
			gMessage = "No seat selected";
			return;
		}
		else {
			std::ostringstream oss;
			glm::vec2 a = rowcol(pickedID);
			oss << a.y<<"  "<<a.x;
			gMessage = oss.str();
		}
		gViewMatrix = glm::lookAt(convert(pickedID),	// eye
			glm::vec3(0.0, 4.75, -10.0),	// center
			glm::vec3(0.0, 1.0, 0.0));	// up
		flag = !flag;
		posIndex = pickedID;
		// Generate a Source to playback the Buffer
		SourcePos[0] = 0;
		SourcePos[1] = 5.0;
		SourcePos[2] = 6.0;
		SourcePos2[0] = 0;
		SourcePos2[1] = 5.0;
		SourcePos2[2] = -6.0;
		ListenerOri[0] = 0.0;
		ListenerOri[1] = 4.75;
		ListenerOri[2] = -10.0;
		ListenerOri[3] = 0.0;
		ListenerOri[4] = 1.0;
		ListenerOri[5] = 0.0;
		ListenerPos[0] = convert(pickedID).x;
		ListenerPos[1] = convert(pickedID).y;
		ListenerPos[2] = convert(pickedID).z;
		alGenSources(1, &uiSource);
		alGenSources(2, &uiSource2);
		// Attach Source to Buffer
		alSourcei(uiSource, AL_BUFFER, uiBuffer);
		alSourcef(uiSource, AL_PITCH, 1.0f);
		alSourcef(uiSource, AL_GAIN, 1.0f);
		alSourcefv(uiSource, AL_POSITION, SourcePos);
		alSourcefv(uiSource, AL_VELOCITY, SourceVel);
		alListenerfv(AL_POSITION, ListenerPos);
		alListenerfv(AL_VELOCITY, ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);

		alSourcei(uiSource2, AL_BUFFER, uiBuffer2);
		alSourcef(uiSource2, AL_PITCH, 1.0f);
		alSourcef(uiSource2, AL_GAIN, 1.0f);
		alSourcefv(uiSource2, AL_POSITION, SourcePos2);
		alSourcefv(uiSource2, AL_VELOCITY, SourceVel);


		alListenerfv(AL_POSITION, ListenerPos);
		alListenerfv(AL_VELOCITY, ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);
			

	}
	// Clear the screen in white
	


}

int initWindow(void)
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "3D Seat Selction", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Selection");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Seat: ", TW_TYPE_STDSTRING, &gMessage, "GUI resizable=true size='240 120'");
	TwDefine("GUI resizable=true size='240 120'");
	// Set up inputs
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}

void initOpenGL(void)
{

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45?? Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	gProjectionMatrix = glm::perspective(45.0f, 3.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	gViewMatrix = glm::lookAt(glm::vec3(4.5, 4.5, 20.0f),	// eye
		glm::vec3(4.5, 4.5, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up

									// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("Picking.vertexshader", "Picking.fragmentshader");
	textureProgramID = LoadShaders("Texture.vertexshader", "Texture.fragmentshader");
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ProjMatrixID = glGetUniformLocation(programID, "P");

	textureMatrixID = glGetUniformLocation(textureProgramID, "MVP");
	TextureID = glGetUniformLocation(textureProgramID, "myTextureSampler");

	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	// Get a handle for our "pickingColorID" uniform
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");
	pickingColorArrayID = glGetUniformLocation(pickingProgramID, "PickingColorArray");
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


	createObjects();
}

void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId) {

	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);
	const size_t Normaloffset = sizeof(Vertices[0].Color) + RgbOffset;
	const size_t UVoffset = sizeof(Vertices[0].Normal) + Normaloffset;
	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);	//
	glBindVertexArray(VertexArrayId[ObjectId]);		//

													// Create Buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create Buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)Normaloffset);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)UVoffset);
	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);	// normal
	glEnableVertexAttribArray(3);	// UV
									// Disable our Vertex Buffer Object 
	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
			);
	}
}

void cleanup(void)
{
	// Cleanup VBO and shader
	for (int i = 0; i < NumObjects; i++) {
		glDeleteBuffers(1, &VertexBufferId[i]);
		glDeleteBuffers(1, &IndexBufferId[i]);
		glDeleteVertexArrays(1, &VertexArrayId[i]);
	}
	glDeleteProgram(programID);
	glDeleteProgram(pickingProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ATTN: MODIFY AS APPROPRIATE

	int i;
	int index;
	float ucord, vcord = 1.0;
	glm::vec2 a;
	std::ostringstream oss;
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_SPACE:
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
			if (iState != AL_PLAYING)
			{
				// If there are Buffers in the Source Queue then the Source was starved of audio
				// data, so needs to be restarted (because there is more audio data to play)
				alSourcePlay(uiSource);
				alSourcePlay(uiSource2);
			}
			else {
				alSourceStop(uiSource);
				alSourceStop(uiSource2);
			}
			break;
		case GLFW_KEY_P:
			alSourcePlay(uiSource);
		
			break;
		case GLFW_KEY_1:
			if (!flag) {
				break;
			}
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
			if (iState == AL_PLAYING)
			{
				alSourceStop(uiSource);
				alSourceStop(uiSource2);
			}
			
			Texture = loadBMP_custom(movies[0].imgpath);
			alGenBuffers(1, &uiBuffer);
			alGenBuffers(2, &uiBuffer2);
			if (!ALFWLoadWaveToBuffer((char*)movies[0].leftsound, uiBuffer))
			{
				ALFWprintf("Failed to load %s\n", movies[0].leftsound);
			}
			if (!ALFWLoadWaveToBuffer((char*)movies[0].rightsound, uiBuffer2))
			{
				ALFWprintf("Failed to load %s\n", movies[0].rightsound);
			}
			
			// Attach Source to Buffer
			alSourcei(uiSource, AL_BUFFER, uiBuffer);
			alSourcef(uiSource, AL_PITCH, 1.0f);
			alSourcef(uiSource, AL_GAIN, 1.0f);
			alSourcefv(uiSource, AL_POSITION, SourcePos);
			alSourcefv(uiSource, AL_VELOCITY, SourceVel);
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);

			alSourcei(uiSource2, AL_BUFFER, uiBuffer2);
			alSourcef(uiSource2, AL_PITCH, 1.0f);
			alSourcef(uiSource2, AL_GAIN, 1.0f);
			alSourcefv(uiSource2, AL_POSITION, SourcePos2);
			alSourcefv(uiSource2, AL_VELOCITY, SourceVel);


			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_2:
			if (!flag) {
				break;
			}
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
			if (iState == AL_PLAYING)
			{
				alSourceStop(uiSource);
				alSourceStop(uiSource2);
			}
			Texture = loadBMP_custom(movies[1].imgpath);
			
			alGenBuffers(1, &uiBuffer);
			alGenBuffers(2, &uiBuffer2);
			
			if (!ALFWLoadWaveToBuffer((char*)movies[1].leftsound, uiBuffer))
			{
				ALFWprintf("Failed to load %s\n", movies[1].leftsound);
			}
			if (!ALFWLoadWaveToBuffer((char*)movies[1].rightsound, uiBuffer2))
			{
				ALFWprintf("Failed to load %s\n", movies[1].rightsound);
			}
		
			// Attach Source to Buffer
			alSourcei(uiSource, AL_BUFFER, uiBuffer);
			alSourcef(uiSource, AL_PITCH, 1.0f);
			alSourcef(uiSource, AL_GAIN, 1.0f);
			alSourcefv(uiSource, AL_POSITION, SourcePos);
			alSourcefv(uiSource, AL_VELOCITY, SourceVel);
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);

			alSourcei(uiSource2, AL_BUFFER, uiBuffer2);
			alSourcef(uiSource2, AL_PITCH, 1.0f);
			alSourcef(uiSource2, AL_GAIN, 1.0f);
			alSourcefv(uiSource2, AL_POSITION, SourcePos2);
			alSourcefv(uiSource2, AL_VELOCITY, SourceVel);


			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_3:
			if (!flag) {
				break;
			}
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
			if (iState == AL_PLAYING)
			{
				alSourceStop(uiSource);
				alSourceStop(uiSource2);
			}
			Texture = loadBMP_custom(movies[2].imgpath);
			alGenBuffers(1, &uiBuffer);
			alGenBuffers(2, &uiBuffer2);
			if (!ALFWLoadWaveToBuffer((char*)movies[2].leftsound, uiBuffer))
			{
				ALFWprintf("Failed to load %s\n", movies[2].leftsound);
			}
			if (!ALFWLoadWaveToBuffer((char*)movies[2].rightsound, uiBuffer2))
			{
				ALFWprintf("Failed to load %s\n", movies[2].rightsound);
			}
			

			// Attach Source to Buffer
			alSourcei(uiSource, AL_BUFFER, uiBuffer);
			alSourcef(uiSource, AL_PITCH, 1.0f);
			alSourcef(uiSource, AL_GAIN, 1.0f);
			alSourcefv(uiSource, AL_POSITION, SourcePos);
			alSourcefv(uiSource, AL_VELOCITY, SourceVel);
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);

			alSourcei(uiSource2, AL_BUFFER, uiBuffer2);
			alSourcef(uiSource2, AL_PITCH, 1.0f);
			alSourcef(uiSource2, AL_GAIN, 1.0f);
			alSourcefv(uiSource2, AL_POSITION, SourcePos2);
			alSourcefv(uiSource2, AL_VELOCITY, SourceVel);


			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_RIGHT:
			if (!flag) {
				break;
			}
			if (posIndex % 10 != 0) {
				posIndex++;
				a = rowcol(posIndex);
				oss << a.y << "  " << a.x;
				gMessage = oss.str();
			}
			else {
				break;
			}
			if (!flag) {
				break;
			}
			printf("%i\n", posIndex);
			gViewMatrix = glm::lookAt(convert(posIndex),	// eye
				glm::vec3(0.0, 4.75, -10.0),	// center
				glm::vec3(0.0, 1.0, 0.0));	// up
			//flag = !flag;			
			ListenerPos[0] = convert(posIndex).x;
			ListenerPos[1] = convert(posIndex).y;
			ListenerPos[2] = convert(posIndex).z;
			
			
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_LEFT:
			if (!flag) {
				break;
			}
			if (posIndex % 10 != 1) {
				posIndex--;
				a = rowcol(posIndex);
				oss << a.y << "  " << a.x;
				gMessage = oss.str();
			}
			else {
				break;
			}
			if (!flag) {
				break;
			}
			printf("%i\n", posIndex);
			gViewMatrix = glm::lookAt(convert(posIndex),	// eye
				glm::vec3(0.0, 4.75, -10.0),	// center
				glm::vec3(0.0, 1.0, 0.0));	// up
										
		

			ListenerPos[0] = convert(posIndex).x;
			ListenerPos[1] = convert(posIndex).y;
			ListenerPos[2] = convert(posIndex).z;
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_UP:
			if (!flag) {
				break;
			}
			if (posIndex / 10 > 0) {
				posIndex-=10;
				a = rowcol(posIndex);
				oss << a.y << "  " << a.x;
				gMessage = oss.str();
			}
			else {
				break;
			}
			
			//posIndex-=10;
			printf("%i\n", posIndex);
			gViewMatrix = glm::lookAt(convert(posIndex),	// eye
				glm::vec3(0.0, 4.75, -10.0),	// center
				glm::vec3(0.0, 1.0, 0.0));	// up
											//flag = !flag;

											// Generate a Source to playback the Buffer
			

			ListenerPos[0] = convert(posIndex).x;
			ListenerPos[1] = convert(posIndex).y;
			ListenerPos[2] = convert(posIndex).z;
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		case GLFW_KEY_DOWN:
			if (!flag) {
				break;
			}
			a = rowcol(posIndex);
			if (a.y<10) {
				posIndex += 10;
				a = rowcol(posIndex);
				oss << a.y << "  " << a.x;
				gMessage = oss.str();
			}
			else {
				break;
			}
			
			printf("%i\n", posIndex);
			gViewMatrix = glm::lookAt(convert(posIndex),	// eye
				glm::vec3(0.0, 4.75, -10.0),	// center
				glm::vec3(0.0, 1.0, 0.0));	// up
											//flag = !flag;

											// Generate a Source to playback the Buffer
			
			ListenerPos[0] = convert(posIndex).x;
			ListenerPos[1] = convert(posIndex).y;
			ListenerPos[2] = convert(posIndex).z;
			alListenerfv(AL_POSITION, ListenerPos);
			alListenerfv(AL_VELOCITY, ListenerVel);
			alListenerfv(AL_ORIENTATION, ListenerOri);
			break;
		default:
			break;
		}
	}
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickObject();
	}
}

int main(void)
{
	// initialize window
	int errorCode = initWindow();

	if (errorCode != 0)
		return errorCode;

	// initialize OpenGL pipeline
	initOpenGL();
	ALFWInit();
	ALFWInitOpenAL();
	alGenBuffers(1, &uiBuffer);
	alGenBuffers(2, &uiBuffer2);
	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)movies[0].leftsound, uiBuffer))
	{
		ALFWprintf("Failed to load %s\n", movies[0].leftsound);
	}
	if (!ALFWLoadWaveToBuffer((char*)movies[0].rightsound, uiBuffer2))
	{
		ALFWprintf("Failed to load %s\n", movies[0].rightsound);
	}
	
	
	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do {
		//// Measure speed
		//double currentTime = glfwGetTime();
		//nbFrames++;
		//if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
		//	// printf and reset
		//	printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		//	nbFrames = 0;
		//	lastTime += 1.0;
		//}
		
	
		/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{

			gViewMatrix = glm::rotate(gViewMatrix, (float)pi / 50, vec3(1, 0, 0));
			
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			gViewMatrix = glm::rotate(gViewMatrix, -(float)pi / 50, vec3(1, 0, 0));
			
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			gViewMatrix = glm::rotate(gViewMatrix, (float)pi / 50, vec3(0, 1, 0));
			
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			gViewMatrix = glm::rotate(gViewMatrix, -(float)pi / 50, vec3(0, 1, 0));
		

		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			//moveVertex();
		}*/

		if (animation) {
			phi += 0.01;
			if (phi > 360)
				phi -= 360;
		}
		
		// DRAWING POINTS
		renderScene();
		renderTexture();
		//pickObject();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}
