#pragma once
#include <SDL2/SDL.h>
#include "Paddle.h"

class Game {
public:
	// Init game
	Game();
	// Deallocates stuff
	~Game();

	// The game running cycle as a program
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:

	// Helper functions for the game loop, they are self explained	
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	
	// Windows that SDL helps us create
	SDL_Window* mWindow;

	// The Renderer that helps us in drawing 2D graphics
	SDL_Renderer* mRenderer;
	
	// Positions of game objects as vectors in 2D
	Vector2 mPaddlePos;
	int mPaddleDir;

	Vector2 mPaddlePosPlayer2;
	int mPaddleDirPlayer2;

	Vector2 mBallPos;
	Vector2 mBallVel;
	// Delta time 
	Uint32 mTicksCount;

	Paddle player1;
	Paddle player2;
	// A flag that will help us to determine if the game is running
	bool mIsRunning;

};
