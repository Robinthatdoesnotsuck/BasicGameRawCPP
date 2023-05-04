#include "Component.h"

    Component::Component(class Actor* owner, int updateOrder = 100) {

}

Component::~Component() {

}

void Component::Update(float deltaTime) {

}

int Component::GetUpdateOrder() const{
    return mUpdateOrder;
}