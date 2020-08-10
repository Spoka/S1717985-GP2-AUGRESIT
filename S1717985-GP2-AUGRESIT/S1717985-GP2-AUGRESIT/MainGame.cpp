#include "MainGame.h"
#include <iostream>


Transform transform;

MainGame::MainGame()
{
	gameState = GameState::PLAY;
	Display* gameDisplay = new Display();
	Mesh* monkey();
	Mesh* bowl();
	Mesh* car();
	Texture* texture();
	Texture* texture1();
	Overlay* overlay();
	Shader* randColShader();
	Shader* skyboxShader();
	GameAudio* audioDevice();
}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initialiseSystem();
	gameLoop();
}

void MainGame::initialiseSystem()
{
	gameDisplay.initialiseDisplay();

	//Initialise 3D models
	monkey.loadModel("..\\res\\monkey.obj");
	car.loadModel("..\\res\\car.obj");
	bowl.loadModel("..\\res\\bowl.obj");

	//Initialise textures
	texture.initialiseTexture("..\\res\\bricks.jpg"); 
	texture1.initialiseTexture("..\\res\\water.jpg");

	//Initialise shaders
	skyboxShader.initialise("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");
	colourShader.initialise("..\\res\\shaderRandomColour.vert", "..\\res\\shaderRandomColour.frag");

	backgroundAudio = audioDevice.loadSound("..\\res\\purepu.wav");
	collisionAudio = audioDevice.loadSound("..\\res\\weesound.wav");

	camera.initialiseCamera(glm::vec3(0, 0, 0), 75.0f, (float)gameDisplay.getWidth() / (float)gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.Bind();
	skyboxShader.setInt("skybox", 0);
	skyboxShader.setMat4("view", camera.GetView());
	skyboxShader.setMat4("projection", camera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::gameLoop()
{
	while (gameState != GameState::EXIT)
	{
		processInput();
		drawGame();

		if (CollisionDetection(monkey.GetSpherePos(), monkey.GetSphereRadius(), bowl.GetSpherePos(), bowl.GetSphereRadius()))
		{
			whichWay = true;
			AudioPlay(collisionAudio, glm::vec3(0.0f, 0.0f, 5.0f));
		}
		else if (CollisionDetection(car.GetSpherePos(), car.GetSphereRadius(), bowl.GetSpherePos(), bowl.GetSphereRadius()))
		{
			whichWay = false;
			AudioPlay(collisionAudio, glm::vec3(0.0f, 0.0f, 5.0f));
		}

		AudioPlay(backgroundAudio, glm::vec3(0.0f, 0.0f, 5.0f)); 
	}

}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		}
	}
}

void MainGame::setColourShader()
{
	colourShader.setFloat("r", -counter);
	colourShader.setFloat("g", counter);
	colourShader.setFloat("b", counter * 2);
}

void MainGame::drawGame()
{


	gameDisplay.clearDisplay(0.0, 0.0, 0.0, 1.0);
	
	Skybox();

	/////////////////MONKEY//////////////////////////

	glm::vec3 monkeyPos = glm::vec3(20, 0, 100.0);
	glm::vec3 monkeyRot = glm::vec3(185, counter, 0.0);
	glm::vec3 monkeyScale = glm::vec3(2, 2, 2);

	transform.SetPosition(monkeyPos);
	transform.SetRotation(monkeyRot);
	transform.SetScale(monkeyScale);

	colourShader.Bind();
	setColourShader();
	colourShader.Update(transform, camera);
	monkey.draw();
	texture1.Bind(0);
	monkey.UpdateSphere(*transform.GetPosition(), 2.0f);

	////////////////BOWL/////////////////////////////

	glm::vec3 bowlPos = glm::vec3(counter * 10, 0, 100.0);
	glm::vec3 bowlRot = glm::vec3(90, 2, 1);
	glm::vec3 bowlScale = glm::vec3(0.1, 0.1, 0.1);

	transform.SetPosition(bowlPos);
	transform.SetRotation(bowlRot);
	transform.SetScale(bowlScale);

	colourShader.Bind();
	setColourShader();
	colourShader.Update(transform, camera);
	texture1.Bind(0);
	bowl.draw();
	bowl.UpdateSphere(*transform.GetPosition(), 2.0f);

	///////////////////CAR//////////////////////////

	glm::vec3 carPos = glm::vec3(-20, 0, 100.0);
	glm::vec3 carRot = glm::vec3(90, counter, 0);
	glm::vec3 carScale = glm::vec3(0.03, 0.03, 0.03);

	transform.SetPosition(carPos);
	transform.SetRotation(carRot);
	transform.SetScale(carScale);

	colourShader.Bind();
	setColourShader();
	colourShader.Update(transform, camera);
	texture.Bind(1);
	car.draw();
	car.UpdateSphere(*transform.GetPosition(), 2.0f);


	if (whichWay)
	{
		counter = counter - 0.01f;
	}
	else
	{
		counter = counter + 0.01f;
	}

	

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	gameDisplay.swapBuffer();
}

bool MainGame::CollisionDetection(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2)
{
	//calculate distance between two pheres
	float distance = glm::sqrt((pos2.x - pos1.x)*(pos2.x - pos1.x) +
							   (pos2.y - pos1.y)*(pos2.y - pos1.y) +
							   (pos2.z - pos1.z)*(pos2.z - pos1.z));

	//Check if distance is smaller than sum of radiuses
	if (distance < (rad1 + rad2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::AudioPlay(unsigned int source, glm::vec3 pos)
{
	//Audio state
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	if (AL_PLAYING != state)
	{
		audioDevice.playSound(source, pos);
	}
}