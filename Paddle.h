
struct Vector2 {
    float x;
    float y;
};

class Paddle {
public:
    Paddle(float xPos, float yPos);
    ~Paddle();

    void setPosition(float xPos, float yPos);
    void setDirection(int direction);

    Vector2 getPosition();
    int getDirection();
private:
    Vector2 mPaddlePos;
    int mPaddleDir;
};