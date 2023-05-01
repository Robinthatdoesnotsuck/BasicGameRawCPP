#include "Game.h"

// thickness of the paddle rectangle
const int thickness = 15;

Game::Game() :mWindow(nullptr)
			 ,mRenderer(nullptr)
			 ,mIsRunning(true)
			 ,mTixcksCount(0) {}

Game::~Game() {
	
}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	// The SDL_Init returns an integer value this value 
	// if not zero means it failed ,we should watch out for that
	if(sdlResult != 0) {

		// With SDL_Log we use a console provided by SDL to printout stuff
		// And with SDL_GetError() we catch the error code and display what the hell happened
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// If nothing fishy happened we should be able to make the window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100, 	// Top left x-coordinate of window
		100, 	// Top left y-coordinate of window
		1024, 	// Width of window
		768, 	// Height of window
		0 		// Flags (0 for no flags been set)
	);

	// Butt if something fails we should be able to escape the error
	if (!mWindow) {
		SDL_Log("Unable to create Window: %s",SDL_GetError());
		return false;
	}
	
	// We initialize the renderer cause like we need graphics
	mRenderer = SDL_CreateRenderer(
		mWindow, // The needed window that we will use to render graphics, this is always a pointer
		-1,		 // This is usually -1, cause it is telling SDL to decide the graphics driver, usually you change this if you have multiple windows
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Flags needed for, the first one makes use of any graphics hardware and the other to enable vsync 
	);
	
	// Since the renderer might return a nullptr if it fails at initializing we need to check for that
	if(!mRenderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return false;
	}
	mPaddlePos.x = 15.0f;
	mPaddlePos.y = 384.0f;
	mBallPos.x = 512.0f;
	mBallPos.y = 384.0f;
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {

	// It destroys the window and quits the game
	SDL_DestroyWindow(mWindow);
	// We need to also Destroy our renderer
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::ProcessInput() {
	// This event is what SDL listens to
	// It is a queue that SDL handles for events on the OS
	// These events are separated by type so we will check that on a switch block
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// SDL_QUIT handles well when you quit
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// This event is, well waiting for a keyboard input
	// And it will be dealt with like it was an array
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Here if we press escape, we will close the game
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {

	// This is to force the 60fps for the game
	// Since 60fps means that it goes by 16ms each frame we kinda force it to wait if a frame finishes up before that 16 ms
	// Treshholds
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the differenc e n ticks from the last frame 
	// converted to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Update the ticks count for the next frame
	mTicksCount = SDL_GetTicks();
	// TODO: Update objects as function of delta time
	
}

void Game::GenerateOutput() {
	// this is for the screen
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A
	);
	SDL_RenderClear(mRenderer);

	// Top wall
	// To create the wall we set another one
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// To draw the rectangle wall
	SDL_Rect topWall{
		0,	      // Position at x this is top left
		0,		  // Position at y this is top left
		1024,	  // Width
		thickness //Height
	};

	SDL_RenderFillRect(mRenderer, &topWall);
	
	// Bottom wall
	// To draw the rectangle wall
	SDL_Rect bottomWall{
		0,	      // Position at x this is top left
		768-thickness,		  // Position at y this is top left
		1024,	  // Width
		thickness //Height
	};

	SDL_RenderFillRect(mRenderer, &bottomWall);

	// Right Wall
	SDL_Rect rightWall{
		1024 - thickness,
		0,
		thickness,
		768
	};
	
	SDL_RenderFillRect(mRenderer, &rightWall);

	// To Draw the ball 
	SDL_Rect ball {
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// To Draw the paddle
	SDL_Rect paddle {
		static_cast<int>(mPaddlePos.x - thickness/2),
		static_cast<int>(mPaddlePos.y - 768/12),
		thickness,
		768/6
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);
}
