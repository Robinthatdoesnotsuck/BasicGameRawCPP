#include "Actor.h"


Actor::Actor(class Game* game) {
    
}

Actor::~Actor() {

}

// Updates for everything
void Actor::Update(float deltaTime) {

}

void Actor::UpdateComponents(float deltaTime) {

}

void Actor::UpdateActor(float deltaTime) {

}

void Actor::AddComponent(class Component* component) {

}

void Actor::RemoveComponent(class Component* component) {

}
// Getter and setters

void Actor::setPosition(Vector2 Position) {

}

void Actor::setScale(float scale) {

}

void Actor::setRotation(float rotation) {

}

Vector2 Actor::getPosition() {
    return mPosition;
}

float Actor::getScale() {
    return mScale;
}

float Actor::getRotation() {
    return mRotation;
}