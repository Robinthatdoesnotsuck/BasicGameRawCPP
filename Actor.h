#include "Game.h"
#include "Component.h"
class Actor {
public:
    enum State {
        EActive,
        EPaused,
        EDead
    };

    // Constructor / Destructor
    Actor(class Game* game);
    virtual ~Actor();

    // Update Function called from Game
    void Update(float deltaTime);
    // Update all components attached to the actor
    void UpdateComponents(float deltaTime);
    // Any actor specifict update code this has to be overridable
    virtual void UpdateActor(float deltaTime);

    // getters/setters
    void setPosition(Vector2 Position) ;
    void setScale(float scale);
    void setRotation(float rotation);
    Vector2 getPosition();
    float getScale();
    float getRotation();

    // ADD or REMOVE components
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);
private:
    // Actor state
    State mState;
    // Transform operations
    Vector2 mPosition;
    float mScale;
    float mRotation;
    // Components list
    std::vector<class Component*> mComponents;
    class Game* mGame;

};