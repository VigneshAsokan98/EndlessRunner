/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler warnings
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "CarShape.h"
#include "Pentagon_pyramid.h"
#include "Player.h"
#include "HeightMapTerrain.h"
#include "EnemyManager.h"
#include "Bullet.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pHeightTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL; 
	m_pSpline = NULL;
	m_pCar = NULL;
	m_pPlayer = NULL;
	m_pEnemyManager = NULL;
	m_pBullet = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pSpline;
	delete m_pCar;
	delete m_pPlayer;
	delete m_pHeightTerrain;
	delete m_pEnemyManager;
	delete m_pBullet;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pHeightTerrain = new CHeightMapTerrain;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pTree = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pSpline = new CCatmullRom;
	m_pCar = new CCarShape;
	m_pPlayer = new CPlayer;
	m_pEnemyManager = new CEnemyManager;
	m_pBullet = new CBullet;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("ObjectShader.vert");
	sShaderFileNames.push_back("ObjectShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the Object shader for Primitives
	CShaderProgram* pObjectShader = new CShaderProgram;
	pObjectShader->CreateProgram();
	pObjectShader->AddShaderToProgram(&shShaders[4]);
	pObjectShader->AddShaderToProgram(&shShaders[5]);
	pObjectShader->LinkProgram();
	m_pShaderPrograms->push_back(pObjectShader);


	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pHeightTerrain->Create((char*)"resources\\textures\\terrainHeightMap200.bmp", (char*)"resources\\textures\\GrassBright.bmp",
																		glm::vec3(0, 0, 0), 1000.0f, 1000.0f, 100.f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("resources\\fonts\\stencil.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pTree->Load("resources\\models\\Clouds\\cloud.obj"); //Downloaded from https://free3d.com/3d-model/tree02-35663.html on 30 March 2022

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	//m_pAudio->Initialise();
	//m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	//m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	//Create Spline
	m_pSpline->CreateCentreline("resources\\textures\\Toon_Road_Texture.png");
	m_pSpline->CreateOffsetCurves();
	m_pSpline->CreateTrack();
	m_pSpline->CreateFenceCurves("resources\\textures\\fence.png");

	//Create Primitive Objects
	m_pCar->CreateCar();

	m_pPlayer->Init(m_pSpline);

	//Create Enemies
	m_pEnemyManager->Init(m_pSpline,m_pPlayer);
	m_pBullet->Init(m_pSpline);
}


// Render method runs repeatedly in a loop
void Game::Render() 
{	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	pMainProgram->SetUniform("fMinHeight", -50.f);	// Diffuse material reflectance
	pMainProgram->SetUniform("fMaxHeight", 70.f);	// Specular material reflectance	

	// Render the new terrain
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, -100.0f, 0.0f));
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	pMainProgram->SetUniform("renderterrain", true);
	pMainProgram->SetUniform("sampler1", 1);
	pMainProgram->SetUniform("sampler2", 2);
	pMainProgram->SetUniform("sampler3", 3);
	m_pHeightTerrain->Render();
	modelViewMatrixStack.Pop();
	pMainProgram->SetUniform("renderterrain", false);

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	//Use of Object Shader
	CShaderProgram* pObjectShader = (*m_pShaderPrograms)[2];
	pObjectShader->UseProgram();
	pObjectShader->SetUniform("sampler0", 0);
	pObjectShader->SetUniform("bUseTexture", true);
	pObjectShader->SetUniform("CubeMapTex", 10);
	pObjectShader->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	


	glm::vec3 playerPos = m_pPlayer->GetPosition();
	glm::mat3 TNB = m_pPlayer->GetTNBFrame();

	// Set light and materials in sphere programme
	pObjectShader->SetUniform("Spotlight[0].position", viewMatrix * glm::vec4(playerPos + TNB[0] * 2.f + TNB[2] * 5.f + TNB[1] * 2.f, 1.f));
	pObjectShader->SetUniform("Spotlight[0].La", glm::vec3(1.f));
	pObjectShader->SetUniform("Spotlight[0].Ld", glm::vec3(1.f));	
	pObjectShader->SetUniform("Spotlight[0].Ls", glm::vec3(1.f));
	pObjectShader->SetUniform("Spotlight[0].direction", glm::normalize(viewNormalMatrix * TNB[0]));
	pObjectShader->SetUniform("Spotlight[0].exponent", 20.0f);
	pObjectShader->SetUniform("Spotlight[0].cutoff", 30.0f); 

	pObjectShader->SetUniform("Spotlight[1].position", viewMatrix * glm::vec4(playerPos + TNB[0] * 2.f + TNB[2] * 5.f + TNB[1] * -2.f, 1.f));
	pObjectShader->SetUniform("Spotlight[1].La", glm::vec3(1.f));
	pObjectShader->SetUniform("Spotlight[1].Ld", glm::vec3(1.f));
	pObjectShader->SetUniform("Spotlight[1].Ls", glm::vec3(1.f));
	pObjectShader->SetUniform("Spotlight[1].direction", glm::normalize(viewNormalMatrix * TNB[0]));
	pObjectShader->SetUniform("Spotlight[1].exponent", 20.0f);
	pObjectShader->SetUniform("Spotlight[1].cutoff", 30.0f);

	pObjectShader->SetUniform("material1.shininess", 15.0f);
	pObjectShader->SetUniform("material1.Ma", glm::vec3(.5f));
	pObjectShader->SetUniform("material1.Md", glm::vec3(.5f));
	pObjectShader->SetUniform("material1.Ms", glm::vec3(.5f));

	pObjectShader->SetUniform("Positionlight[0].Position", viewMatrix* glm::vec4(playerPos + TNB[2] * 2.f + TNB[1] * 5.f + TNB[0] * 2.f, 1.f));
	pObjectShader->SetUniform("Positionlight[0].Intensity", glm::vec3(1.f, .0f, 0.f));

	pObjectShader->SetUniform("useLight", m_useLight);

	// Render the Car
	m_pPlayer->Render(modelViewMatrixStack, pObjectShader, m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()), m_pCamera->GetPosition(), m_pCamera->GetViewMatrix());

	// Render the Spline 
	modelViewMatrixStack.Push();
	pObjectShader->SetUniform("bUseTexture", true); // turn off texturing
	pObjectShader->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pObjectShader->SetUniform("matrices.normalMatrix",
		m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	pObjectShader->SetUniform("ReflectFactor", 0.f);
	/*m_pSpline->RenderCentreline();
	m_pSpline->RenderOffsetCurves();*/
	m_pSpline->RenderTrack();
	m_pSpline->RenderFences();
	// Render your object here
	modelViewMatrixStack.Pop();
	
	m_pEnemyManager->Render(modelViewMatrixStack, pObjectShader, m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()), m_pCamera->GetPosition(), m_pCamera->GetViewMatrix());
	m_pBullet->Render(modelViewMatrixStack, pObjectShader, m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()), m_pCamera->GetPosition(), m_pCamera->GetViewMatrix());
	// Draw the 2D graphics after the 3D graphics
	DisplayHUD();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());	  

}

void Game::RenderMenu()
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(0.894f, 0.066f, 0.458f, 1.f));
	if (m_menuPlaySelected)
	{
		m_pFtFont->Render(width / 3, height / 2, 50, ">Play<");
		m_pFtFont->Render(width / 3, height / 2 - 40, 50, "Exit");
	}
	else
	{
		m_pFtFont->Render(width / 3, height / 2, 50, "Play");
		m_pFtFont->Render(width / 3, height / 2 - 40, 50, ">Exit<");
	}

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}
void Game::RenderGameOver()
{
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;
			
	// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(width / 3, height / 2, 50, "Game Over");

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}
// Update method runs repeatedly with the Render method
void Game::Update()
{
		
	if (m_pPlayer->GetPosition().y > 0)
		m_pSkybox->SwitchTexture(1);
	else
		m_pSkybox->SwitchTexture(0);

	m_pPlayer->Update(m_dt);	
	UpdateCamera();
	m_pAudio->Update();
	m_isGameOver = m_pEnemyManager->Update(m_dt, m_pPlayer->GetPosition());
	m_pBullet->Update(m_dt);
	HandleBulletCollision();
	if (m_isGameOver)
		SwitchState(GameState::GameOver);
}

void Game::UpdateCamera()
{
	glm::vec3 playerPos = m_pPlayer->GetPosition();
	glm::mat3 TNB = m_pPlayer->GetTNBFrame();

	switch (m_CurrentCameraAngle)
	{
	case CAMERA_ANGLE::Top:
		m_pCamera->Set(playerPos + TNB[2] * 200.f, playerPos + TNB[1] * 15.f, glm::vec3(0, 1, 0));
		break;
	case CAMERA_ANGLE::Side:
		m_pCamera->Set(playerPos + TNB[1] * 30.f + TNB[2] * 5.f, playerPos, glm::vec3(0, 1, 0));
		break;
	case CAMERA_ANGLE::Player:
		m_pCamera->Set(playerPos - TNB[0] * 25.f + TNB[2] * 10.f, playerPos, glm::vec3(0, 1, 0));
		break;
	case CAMERA_ANGLE::Free:
		m_pCamera->Update(m_dt * 10);
		break;
	case CAMERA_ANGLE::MAX:
		break;
	default:
		break;
	}
}

void Game::DisplayHUD()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(.0f, 1.0f, .0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		fontProgram->SetUniform("vColour", glm::vec4(0.894f, 0.066f, 0.458f, 1.f));
		m_pFtFont->Render(width - 120, height - 30, 25, "Laps: %d", m_pPlayer->GetLaps());
		m_pFtFont->Render(width - 120, height - 60, 25, "Score: %d", m_Score);

	}
}

void Game::HandleBulletCollision()
{
	bool isHit = m_pEnemyManager->CheckBulletCollision(m_pBullet->GetPosition());
	if (isHit)
	{
		m_pBullet->Disable();
		m_Score++;
	}
}
void Game::SwitchState(GameState type)
{
	if (m_currentGameState == type)
		return;

	m_currentGameState = type;
}
// The game loop runs repeatedly until game over
void Game::GameLoop()
{	
	// Fixed timer
	m_dt = m_pHighResolutionTimer->Elapsed();
	if (m_dt > 1000.0 / (double) Game::FPS) {
		m_pHighResolutionTimer->Start();
		switch (m_currentGameState)
		{
		case GameState::Menu:
			RenderMenu();
			break;
		case GameState::GamePlay:
			Update();
			Render();
			break;
		case GameState::GameOver:
			RenderGameOver();
			break;
		default:
			break;
		}
		
	}
	
	
	//// Variable timer
	//m_pHighResolutionTimer->Start();

	//if (m_isGameOver)
	//	RenderGameOver();
	//else
	//{
	//	Update();
	//	Render();
	//}

	//	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_SPACE:
			if (m_currentGameState == GameState::Menu)
				SwitchState(GameState::GamePlay);
			else if(m_currentGameState == GameState::GamePlay)
				m_pBullet->Fire(m_pPlayer->GetDistance(), m_pPlayer->GetLane());
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break; 
		case VK_RIGHT:
			m_pPlayer->MovePlayer(Lane::Right);
			break;
		case VK_LEFT:
			m_pPlayer->MovePlayer(Lane::Left);
			break;
		case VK_UP:
			if(m_currentGameState == GameState::Menu)
				m_menuPlaySelected =true;
			break;
		case VK_DOWN:
			if (m_currentGameState == GameState::Menu)
				m_menuPlaySelected = false;
			break;
		case 'W':
			if (m_currentGameState == GameState::Menu)
				m_menuPlaySelected = true;
			break;
		case 'S':
			if (m_currentGameState == GameState::Menu)
				m_menuPlaySelected = false;
			break;
		case 'D':
			m_pPlayer->MovePlayer(Lane::Right);
			break;
		case 'A':
			m_pPlayer->MovePlayer(Lane::Left);
			break;
		case 'C':
			ChangeCameraAngle();
			break;
		case 'L':
			m_useLight = !m_useLight;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

void Game::ChangeCameraAngle()
{
	int idx = (int)m_CurrentCameraAngle;
	idx++;
	m_CurrentCameraAngle = (CAMERA_ANGLE)idx;

	if (m_CurrentCameraAngle == CAMERA_ANGLE::MAX)
		m_CurrentCameraAngle = CAMERA_ANGLE::Top;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return int(game.Execute());
}
