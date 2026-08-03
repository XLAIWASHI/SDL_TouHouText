#ifndef PATTERN_H
#define PATTERN_H

class Boss;

class Pattern
{
public:
    Pattern() = default;
    virtual ~Pattern() = default;
    virtual void start(Boss* boss) {};
    virtual void update(Boss* boss, float deltaTime) = 0;

private:
    float timer = 0.0f;
};

#endif