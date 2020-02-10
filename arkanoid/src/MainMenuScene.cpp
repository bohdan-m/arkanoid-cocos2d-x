#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>

USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool MainMenu::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();

    mainMenu = Menu::create();
    auto newGame = MenuItemFont::create("New Game", CC_CALLBACK_1(MainMenu::menuNewGameCallback, this));
    auto about = MenuItemFont::create("About", CC_CALLBACK_1(MainMenu::menuAboutCallback, this));
    auto exit = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenu::menuExitCallback, this));

    newGame->setPosition(size.width / 2, 3 * size.height / 4);
    about->setPosition(size.width / 2, 2 * size.height / 4);
    exit->setPosition(size.width / 2, size.height / 4);

    mainMenu->addChild(newGame);
    mainMenu->addChild(about);
    mainMenu->addChild(exit);

    mainMenu->setPosition(0, 0);
    this->addChild(mainMenu);

    return true;
}


void MainMenu::menuNewGameCallback(Ref* pSender)
{
    Scene* game = Game::createScene();
    Director::getInstance()->pushScene(game);
}

void MainMenu::menuAboutCallback(Ref* pSender)
{
    std::cerr << "about\n";
}

void MainMenu::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
}


