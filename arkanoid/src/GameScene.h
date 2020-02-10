#pragma once

#include "cocos2d.h"

#include <string>
#include <fstream>
#include <algorithm>
#include <array>;

USING_NS_CC;

class Game : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Game);

private:
    PhysicsWorld* sceneWorld;
    Node* boundary;
    Node* ball;
    Node* paddle;
    Label* scoreLabel;
    Node* statusBar;
    std::array<Node*, 3> lives;
    int livesCount = 3;
    int score = 0;

    std::ofstream debug_log;
    bool ballIsOnPaddle = true;

    void setPhysicsWorld(PhysicsWorld* world) { sceneWorld = world; }
    bool onContactBegin(PhysicsContact& contact);
    bool onContactEnd(PhysicsContact& contact);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void menuGoBackCallback(cocos2d::Ref* pSender);
    void setBallOnPaddle();
    void gameOver();
};


