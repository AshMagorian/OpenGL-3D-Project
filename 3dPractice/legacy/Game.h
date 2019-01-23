#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>

#include <exception>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "Texture.h"
#include "RenderTexture.h"
#include "Input.h"
#include "Camera.h"
#include "Bloom.h"
#include "MotionBlur.h"
#include "Entity.h"
#include "Ship.h"
#include "SkyPlane.h"
#include "Obstacle.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

	void InitSkyBox();
	void InitObstacles();
	void SetUpShaders();

	void UpdateObjects(float _deltaTime, glm::mat4 _model);
	void Collision();
	void AddObstacle();

	bool CheckaabbCollision(glm::vec3 _a_Max, glm::vec3 _a_Min, glm::vec3 _b_Max, glm::vec3 _b_Min);

	void DrawObjects();

private:

	int windowWidth;
	int windowHeight;
	int frameCount;


	SDL_Window *window;

	Input *input;
	Camera *camera;
	Bloom *bloom;
	MotionBlur *motionBlur;

	ShaderProgram *noLightingShaderProgram;
	ShaderProgram *shaderProgram;
	ShaderProgram *specularShaderProgram;

	RenderTexture *rt;
	RenderTexture *PreviousRt;
	RenderTexture *PreviousRt2;

	Entity* ship;

	Entity* skyBoxFront;
	Entity* skyBoxUp;
	Entity* skyBoxLeft;
	Entity* skyBoxRight;
	Entity* skyBoxDown;

	Entity* tempObstacle1;
	Entity* tempObstacle2;
	Entity* tempObstacle3;
	Entity* tempObstacle4;
	Entity* tempObstacle5;
	Entity* tempObstacle6;
	Entity* tempObstacle7;

	glm::mat4 model;

	bool collision;

	int nextObstacle;
	int numberOfObstacles;
	std::vector<Entity*> obstacles;
	std::vector<Entity*> skyBox;

	bool quit;

	unsigned int lastTime;
	unsigned int currentTime;
	float deltaTime;
};