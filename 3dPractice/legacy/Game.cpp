#include "Game.h"


Game::Game()
{
	windowWidth = 640;
	windowHeight = 480;
	frameCount = 0;


	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		throw std::exception();
	}
	window = SDL_CreateWindow("Triangle",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::exception();
	}

	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}

	input = new Input();
	camera = new Camera();
	bloom = new Bloom(windowWidth, windowHeight);
	motionBlur = new MotionBlur(windowWidth, windowHeight);

	noLightingShaderProgram = new ShaderProgram("../shaders/simple.vert", "../shaders/simpleNoLight.frag");
	shaderProgram = new ShaderProgram("../shaders/simple.vert", "../shaders/simple.frag");
	specularShaderProgram = new ShaderProgram("../shaders/simple.vert", "../shaders/specular.frag");
	rt = new RenderTexture(windowWidth, windowHeight);
	PreviousRt = new RenderTexture(windowWidth, windowHeight);
	PreviousRt2 = new RenderTexture(windowWidth, windowHeight);

	model = glm::mat4(1.0f);
	nextObstacle = 0;
	collision = false;
	quit = false;

	ship = new Ship("../images/starfox_diffuse.png", "../objects/starfox_ship.obj", specularShaderProgram, glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

	InitSkyBox();		 // Sets up the sky box
	InitObstacles();	 // Sets up the obstacles
	SetUpShaders();	

	lastTime = 0; /*!< The last frames time*/
	currentTime = 0; /*!< The current frames time */
	deltaTime = 0.0f; /*!< The delta between the last frame and current frame times */
}

Game::~Game()
{
	delete ship;
	delete skyBoxFront;
	delete skyBoxDown;
	delete skyBoxLeft;
	delete skyBoxRight;
	delete skyBoxUp;
	delete tempObstacle1;
	delete tempObstacle2;
	delete tempObstacle3;
	delete tempObstacle4;
	delete tempObstacle5;
	delete tempObstacle6;
	delete tempObstacle7;
	delete input;
	delete camera;
	delete bloom;
	delete motionBlur;
	delete rt;
	delete PreviousRt;
	delete PreviousRt2;

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run()
{
	while (!quit)
	{
		currentTime = SDL_GetTicks();		// Calculate deltatime
		deltaTime = (float)(currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		if ((frameCount % (int)(0.6f / (1.0f / 60.0f))) == 0) // Timer for every 0.6 seconds
		{
			AddObstacle(); // Adds an obstacle to the front
		}

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		if (frameCount % 4) // Ever 4 frames
		{
			PreviousRt2 = PreviousRt;
			PreviousRt = rt;
		}

		rt->clear(); // Clears the render texture

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model = glm::mat4(1.0f);

		UpdateObjects(deltaTime, model); // Updates every object in the scene

		Collision(); // Checks for collision

		DrawObjects(); // Runs through the draw function of each object

		bloom->BloomDraw(rt, windowWidth, windowHeight); // Adds the bloom post-processing effect

		//motionBlur->MotionBlurDraw(rt, PreviousRt, PreviousRt2, windowWidth, windowHeight);

		SDL_GL_SwapWindow(window);

		if (deltaTime < (1.0f / 60.0f))		//caps framerate
		{
			SDL_Delay((unsigned int)(((1.0f / 60.0f) - deltaTime)*1000.0f));
		}
		lastTime = currentTime;
		frameCount += 1;
	}
}


void Game::SetUpShaders()
{
	// Set the projection uniform
	noLightingShaderProgram->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 0.1f, 230.f));
	shaderProgram->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 0.1f, 230.f));
	specularShaderProgram->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 0.1f, 230.f));

	// Set the lighting uniforms
	shaderProgram->SetUniform("in_Emissive", glm::vec3(0.0, 0.0, 0.0));
	shaderProgram->SetUniform("in_Ambient", glm::vec3(0.3, 0.3, 0.3));
	shaderProgram->SetUniform("in_LightPos", glm::vec3(0, 10, 0));

	specularShaderProgram->SetUniform("in_Emissive", glm::vec3(0, 0, 0));
	specularShaderProgram->SetUniform("in_Ambient", glm::vec3(0.1, 0.1, 0.1));
	specularShaderProgram->SetUniform("in_LightPos", glm::vec3(10, 130, 60));
}

void Game::InitSkyBox() // Initialise each plane of the box and push them back into an array
{
	skyBoxFront = new SkyPlane("../images/SkyboxFront.png", "../objects/bigPlane.obj", noLightingShaderProgram, glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.0f, -150.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
	skyBoxLeft = new SkyPlane("../images/SkyboxLeft.png", "../objects/bigPlane.obj", noLightingShaderProgram, glm::vec3(90.0f, 0.0f, -90.0f), glm::vec3(69.8f, -81.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1);
	skyBoxRight = new SkyPlane("../images/SkyboxRight.png", "../objects/bigPlane.obj", noLightingShaderProgram, glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(-69.8f, -81.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2);
	skyBoxUp = new SkyPlane("../images/SkyboxUp.png", "../objects/bigPlane.obj", noLightingShaderProgram, glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(0.0f, -81.0f, 69.8f), glm::vec3(1.0f, 1.0f, 1.0f), 3);
	skyBoxDown = new SkyPlane("../images/SkyboxDown.png", "../objects/bigPlane.obj", noLightingShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -81.0f, -69.8f), glm::vec3(1.0f, 1.0f, 1.0f), 4);

	skyBox.push_back(skyBoxFront);
	skyBox.push_back(skyBoxLeft);
	skyBox.push_back(skyBoxRight);
	skyBox.push_back(skyBoxUp);
	skyBox.push_back(skyBoxDown);
}

void Game::InitObstacles() // Initialise each obstacle and push them back into an array
{
	numberOfObstacles = 7;
	tempObstacle1 = new Obstacle("../images/small_asteroid_diffuse.png", "../objects/small_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
	tempObstacle2 = new Obstacle("../images/small_asteroid_diffuse.png", "../objects/small_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
	tempObstacle3 = new Obstacle("../images/large_asteroid_diffuse.png", "../objects/large_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1);
	tempObstacle4 = new Obstacle("../images/small_asteroid_diffuse.png", "../objects/small_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
	tempObstacle5 = new Obstacle("../images/large_asteroid_diffuse.png", "../objects/large_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1);
	tempObstacle6 = new Obstacle("../images/large_asteroid_diffuse.png", "../objects/large_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1);
	tempObstacle7 = new Obstacle("../images/small_asteroid_diffuse.png", "../objects/small_asteroid.obj", specularShaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
	obstacles.push_back(tempObstacle1);
	obstacles.push_back(tempObstacle2);
	obstacles.push_back(tempObstacle3);
	obstacles.push_back(tempObstacle4);
	obstacles.push_back(tempObstacle5);
	obstacles.push_back(tempObstacle6);
	obstacles.push_back(tempObstacle7);
}

void Game::UpdateObjects(float _deltaTime, glm::mat4 _model)
{
	input->Update(&quit); //reads inputs

	SDL_WarpMouseInWindow(window, 320, 240); // moves the mouse to the middle of the window
	SDL_ShowCursor(SDL_DISABLE); // Hides the cursor


	camera->Update(input, &_model, ship->GetPosition()); //updates the camera

	noLightingShaderProgram->SetUniform("in_View", glm::inverse(_model));
	shaderProgram->SetUniform("in_View", glm::inverse(_model));
	specularShaderProgram->SetUniform("in_View", glm::inverse(_model));// Update the view of the shader programs

	for (int i = 0; i < obstacles.size(); i++)
	{
		obstacles.at(i)->Update(_deltaTime);
	}

	for (int i = 0; i < 5; i++)
	{
		skyBox.at(i)->Update(ship);
	}

	ship->Update(input, deltaTime);
}

void Game::AddObstacle() // Moves an obstacle which has passed the camera and places it in front
{
	std::random_device generator;
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f); // Used to create a random float between -1.0f and 1.0f
	float xOffsetFromCenter = distribution(generator) * 20.0f;		 // Random offset in the X axis ranging from -20 and 20
	float yOffsetFromCenter = distribution(generator) * 20.0f;		// Random offset in the Y axis ranging from -20 and 20


	obstacles.at(nextObstacle)->SetPosition(glm::vec3(ship->GetPosition().x + xOffsetFromCenter,  // Sets the position for the obsacle to be placed (Using the offsets calculated)
													  ship->GetPosition().y + yOffsetFromCenter, 
													  -160.0f));
	obstacles.at(nextObstacle)->SetAngularVelocity(); // Gives the new obstacle a random angular velocity
	nextObstacle++;
	if (nextObstacle == numberOfObstacles) // Loops back round to 0
	{
		nextObstacle = 0;
	}
}

void Game::Collision()
{
	for (int i = 0; i < numberOfObstacles; i++) // runs through every obstacle
	{
		if (obstacles.at(i)->GetPosition().z < 20.0f && obstacles.at(i)->GetPosition().z > -20.0f) // Only checks for collision if the z value is between 20 and -20
		{
			//Check collision with ship
			if (CheckaabbCollision(ship->GetaabbMax(), ship->GetaabbMin(), obstacles.at(i)->GetaabbMax(), obstacles.at(i)->GetaabbMin()))
			{
				collision = true;
			}
			else { collision = false; }
		}
	}
}

bool Game::CheckaabbCollision(glm::vec3 _a_Max, glm::vec3 _a_Min, glm::vec3 _b_Max, glm::vec3 _b_Min)
{
	if (_a_Min.x > _b_Max.x) { return false; }
	if (_a_Min.y > _b_Max.y) { return false; }
	if (_a_Min.z > _b_Max.z) { return false; }

	if (_a_Max.x < _b_Min.x) { return false; }
	if (_a_Max.y < _b_Min.y) { return false; }
	if (_a_Max.z < _b_Min.z) { return false; }

	return true; // If none of these occur, then the bounding boxes of these objects are colliding
}

void Game::DrawObjects()
{
	for (int i = 0; i < 5; i++)
	{
		skyBox.at(i)->Draw(rt);
	}

	for (int i = 0; i < numberOfObstacles; i++)
	{
		obstacles.at(i)->Draw(rt);
	}

	if (collision) // Adds red emmisive light to the ship if colliding with an object
	{
		ship->SetEmissive(glm::vec3(100, 0, 0)); // Red
		ship->Draw(rt);
		ship->SetEmissive(glm::vec3(0, 0, 0)); // Back to normal
	}
	else
	{
		ship->Draw(rt);
	}
}