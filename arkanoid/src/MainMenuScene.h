#pragma once

#include "cocos2d.h"
#include "GameScene.h"

class MainMenu : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);

    cocos2d::Menu* mainMenu;    
    void menuNewGameCallback(cocos2d::Ref* pSender);
    void menuAboutCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);
};

