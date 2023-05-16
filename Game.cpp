#include "Game.h"

// thickness of the paddle rectangle
const int thickness = 15;
const int paddleH = 768/6;
Game::Game() :mWindow(nullptr)
			 ,mRenderer(nullptr)
			 ,mIsRunning(true)
			 ,mTicksCount(0)
			 ,mPaddleDir(0)
			 ,player1(15.0f, 384.0f)
			 ,player2(1009.0f, 384.0f)
			{}

Game::~Game() {

}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    int sdlImgResult = IMG_Init(IMG_INIT_PNG);
	// The SDL_Init returns an integer value this value
	// if not zero means it failed ,we should watch out for that
	if(sdlResult != 0) {

		// With SDL_Log we use a console provided by SDL to printout stuff
		// And with SDL_GetError() we catch the error code and display what the hell happened
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
    // Initializing image
    if (sdlImgResult == 0) {
        SDL_Log("Failted to initialize SDL_Image:", SDL_GetError());
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

	mBallPos.x = 512.0f;
	mBallPos.y = 384.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = -235.0f;

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

    while (!mActors.empty()) {
        delete mActors.back();
    }
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

	// Handling paddle movement
	player1.mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) {
		// In Raw graphics env it is usally the negative y the up direction
		player1.mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		player1.mPaddleDir += 1;
	}
	player2.mPaddleDir = 0;
	if (state[SDL_SCANCODE_I]) {
		player2.mPaddleDir  -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		player2.mPaddleDir  += 1;
	}

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

	// Apply Paddle movement according to the direction
	if (player1.mPaddleDir  != 0) {
		player1.mPaddlePos.y += player1.mPaddleDir * 300.0f * deltaTime;
		// Make some limits so that the paddle doesn't wander of the screen
		if (player1.mPaddlePos.y < (paddleH/2.0f + thickness)) {
			player1.mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if (player1.mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)) {
			player1.mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}

	}

	if (player2.mPaddlePos.y != 0) {
		player2.mPaddlePos.y += player2.mPaddleDir * 300.0f * deltaTime;

		if (player2.mPaddlePos.y < (paddleH/2.0f + thickness)) {
			player2.mPaddlePos.y = paddleH/2.0f + thickness;
		}
		else if (player2.mPaddlePos.y >(768.0f - paddleH/2.0f - thickness)) {
			player2.mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
		}
	}



	// Handling been stuck on walls
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1.0f;
	}
	if (mBallPos.y >= (768.0f - thickness) && mBallVel.y > 0.0f) {
		mBallVel.y *= -1.0f;
	}

	float diff = std::abs(player1.mPaddlePos.y - player1.mPaddlePos.y);
	// Paddle hitting balls
	if (
		// Check if the ball is too high or low
		diff <= paddleH/2.0f &&
		// Check that the ball is on our x range of values for the paddle
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// Check if the ball velocity is less than 0 to know that it didn't bounce of
		mBallVel.x < 0.0
	) {
		mBallVel.x *= -1.0f;
	}

	float diffPlayer2 = std::abs(mBallPos.y - player2.mPaddlePos.y);
	// Paddle 2 hitting balls
	if (
		// Check if the ball is too high or low
		diffPlayer2 <= paddleH/2.0f &&
		// Check that the ball is on our x range of values for the paddle
		mBallPos.x >= 1002.5f && mBallPos.x <= 1007.5f &&
		// Check if the ball velocity is less than 0 to know that it didn't bounce of
		mBallVel.x > 0.0
	) {
		mBallVel.x *= -1.0f;
	}
	// Handling Balls 7w7
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Updating Actors

	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;

	// Move pending actors to the actors on scene
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Adding any dead actors to a temporal vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if(actor->getState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors from mActors
	for (auto actor : deadActors) {
		delete actor;
	}
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
		static_cast<int>(player1.mPaddlePos.x - thickness/2),
		static_cast<int>(player1.mPaddlePos.y - 768/12),
		thickness,
		768/6
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	// To Draw the paddle
	SDL_Rect paddle2player {
		static_cast<int>( player2.mPaddlePos.x - thickness/2),
		static_cast<int>(player2.mPaddlePos.y - 768/12),
		thickness,
		768/6
	};

	SDL_RenderFillRect(mRenderer, &paddle2player);

	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor) {
	if(mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	} else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {

	// Searching on the Actors on scene vector
	auto element = std::find(mActors.begin(), mActors.end(), actor);
	(element != std::end(mActors))
		? mActors.erase(element)
		: mPendingActors.erase(element);
}

SDL_Texture* Game::LoadTexture(const char* fileName) {
    SDL_Surface* surf = IMG_Load(fileName);
    if (!surf) {
        SDL_Log("failed to load texture file %s", fileName);
        return nullptr;
    }

    // Create texture from surface
    SDL_Texture* text = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_FreeSurface(surf);
    if (!text) {
        SDL_Log("Failed to convert surface to texture for %s" , fileName);
    }
    return text;
}

void Game::LoadData() {

}
