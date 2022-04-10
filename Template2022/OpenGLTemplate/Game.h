#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CCarShape;
class CPentagonPyramid;
class CPlayer;
class CHeightMapTerrain;
class CEnemyManager;
class CBullet;

enum class GameState {
	Menu = 0,
	GamePlay,
	GameOver
};
enum class CAMERA_ANGLE {
	Top = 0,
	Side,
	Player,
	Free,
	MAX
};

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();
	void SetTreeOffsets();
	void UpdateCamera();
	void SetShaderProps(CShaderProgram* shader);

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	vector <CShaderProgram *> *m_pShaderPrograms;
	//CPlane *m_pPlanarTerrain;
	CHeightMapTerrain* m_pHeightTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pTree;

	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pSpline;
	CCarShape *m_pCar;
	CPlayer *m_pPlayer;
	CBullet *m_pBullet;

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;

	CCamera* m_pCamera;
	static const int FPS = 120;
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;

	CAMERA_ANGLE m_CurrentCameraAngle = CAMERA_ANGLE::Player;
	CEnemyManager* m_pEnemyManager;
	GameState m_currentGameState = GameState::Menu;

	int m_frameCount;
	double m_elapsedTime;
	glm::mat4 m_PlayerOrientation;
	glm::vec3 m_treesOffset[20];
	bool m_useLight = true;
	int m_Score = 0;
	bool m_menuPlaySelected = true;
	bool m_isGameOver = false;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();
	CCamera* GetCamera() { return m_pCamera; }

private:

	void DisplayHUD();
	void GameLoop();
	void ChangeCameraAngle();
	void RenderGameOver();
	void SwitchState(GameState type);
	void RenderMenu();
	void HandleBulletCollision();
};
