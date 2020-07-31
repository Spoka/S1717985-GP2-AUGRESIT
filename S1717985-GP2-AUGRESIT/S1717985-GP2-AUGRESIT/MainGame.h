#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Overlay.h"
#include "GameAudio.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initialiseSystem();
	void processInput();
	void gameLoop();
	void drawGame();

	void AudioPlay(unsigned int source, glm::vec3 pos);

	void setRandColShader();
	
	bool CollisionDetection(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2);
	bool whichWay = false;

	void Skybox();
	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;
	Texture skybox;
	Shader skyboxShader;

	Shader rimShader;
	Shader toonShader;
	Shader explosionShader;
	Shader randColShader;

	Display gameDisplay;
	GameState gameState;
	
	Mesh monkey;
	Mesh bowl;
	Mesh car;

	GameAudio audioDevice;
	unsigned int backgroundAudio;
	unsigned int collisionAudio;

	Texture texture;
	Texture texture1;

	Overlay overlay;

	glm::mat4 modelView;

	Camera camera;

	float counter;
	float bowlWay;

	float colourX;
	float colourY;
	float colourZ;
};