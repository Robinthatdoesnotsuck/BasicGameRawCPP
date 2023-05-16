#include "Paddle.h"

Paddle::Paddle(float xPos, float yPos) {
    mPaddlePos.x = xPos;
    mPaddlePos.y = yPos;
    mPaddleDir = 0;
}

Paddle::~Paddle() {

}

void Paddle::setPosition(float xPos, float yPos) {
    mPaddlePos.x = xPos;
    mPaddlePos.y = yPos;
}

void Paddle::setDirection(int direction) {
    mPaddleDir = direction;
}

Vector2 Paddle::getPosition() {
    return mPaddlePos;
}

int Paddle::getDirection() {
    return mPaddleDir;
}
