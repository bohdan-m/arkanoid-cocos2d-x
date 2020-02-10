#include "GameScene.h"

Scene* Game::createScene()
{
    auto scene = Scene::createWithPhysics();
    auto layer = Game::create();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);

    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    return scene;
}

bool Game::init()
{
    //debug_log.open("log.txt");

    if (!Scene::init())
    {
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Node* statusBar = Sprite::create("statusbar.png");
    statusBar->setAnchorPoint({0, 0});
    statusBar->setContentSize({ size.width, 20 });
    statusBar->setPosition({0, size.height - 20});
    this->addChild(statusBar);

    for (int i = 0; i < 3; ++i)
    {
        Node* heart = Sprite::create("heart.png");
        heart->setPosition({ size.width - 20 - 15 * i, size.height - 10 });
        this->addChild(heart);
        lives[i] = heart;
    }

    

    scoreLabel = Label::createWithSystemFont("Score: 0", "arial", 11);
    scoreLabel->setAnchorPoint(Point(0, 0));
    scoreLabel->setPosition(20, size.height - 17);
    this->addChild(scoreLabel);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactSeparate = CC_CALLBACK_1(Game::onContactEnd, this);
    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    boundary = Node::create();
    auto boundaryBody = PhysicsBody::createEdgeBox(Size(size.width, size.height - 20), PhysicsMaterial(1, 1, 1), 1);
    boundaryBody->setCollisionBitmask(3);
    boundaryBody->setContactTestBitmask(true);
    boundary->setPosition(size.width / 2 + origin.x, size.height / 2 + origin.y - 10);
    boundary->setPhysicsBody(boundaryBody);
    boundary->setName("boundary");
    this->addChild(boundary);

    paddle = Sprite::create("paddle.png");
    paddle->setPosition(size.width / 2, paddle->getContentSize().height / 2);

    auto paddleBody = PhysicsBody::createBox(paddle->getContentSize(), PhysicsMaterial(0, 1, 0));
    paddleBody->setDynamic(false);
    paddleBody->setCollisionBitmask(3);
    paddleBody->setContactTestBitmask(true);
    paddle->setPhysicsBody(paddleBody);

    ball = Sprite::create("ball.png");
    ball->setPosition(size.width / 2, paddle->getContentSize().height + ball->getContentSize().height / 2);

    auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(0, 1, 0));
    ballBody->setCollisionBitmask(3);
    ballBody->setContactTestBitmask(true);
     
    ball->setPhysicsBody(ballBody);
    ball->setName("ball");

    auto brick = Sprite::create("brick.png"); 
    brick->setPosition(size.width / 2, size.height / 4);
    brick->setName("brick");

    auto brickBody = PhysicsBody::createBox(brick->getContentSize(), PhysicsMaterial(1, 1, 1));
    brickBody->setDynamic(false);
    brick->setPhysicsBody(brickBody);
    brickBody->setCollisionBitmask(3);
    brickBody->setContactTestBitmask(true);

    this->addChild(paddle);
    this->addChild(ball);
    this->addChild(brick);

    return true;
}

void Game::menuGoBackCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->popScene();
}

bool Game::onContactBegin(PhysicsContact& contact)
{
    Sprite* pSpriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    Sprite* pSpriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

    if ((pSpriteA->getName() == "ball" && pSpriteB->getName() == "boundary") ||
        (pSpriteA->getName() == "boundary" && pSpriteB->getName() == "ball"))
    {
        if (ball->getPositionY() < 20)
        {
            ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
            --livesCount;
            lives[livesCount]->setVisible(false);

            if (livesCount > 0)
            {                
                setBallOnPaddle();
                ballIsOnPaddle = true;
            }
            else
            {
                gameOver();
            }
        }
    }

    return true;
}

bool Game::onContactEnd(PhysicsContact& contact)
{
    Sprite* pSpriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    Sprite* pSpriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
    Sprite* pSprite = pSpriteA->getName() == "brick" ? pSpriteA : pSpriteB;
    
    if (pSprite->getName() == "brick")
    {
        pSprite->removeFromParent();
        score += 10;
        scoreLabel->setString("Score: " + std::to_string(score));
    }

    return true;
}

void Game::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    Size size = Director::getInstance()->getVisibleSize();
    float step = 0;

    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        if (ballIsOnPaddle)
        {
            ballIsOnPaddle = false;
            ball->getPhysicsBody()->applyForce(Vec2(10000, 30000));
        }

        break;

    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        step = std::min(50.0f, paddle->getPosition().x - paddle->getContentSize().width / 2);
        paddle->runAction(MoveBy::create(0, Vec2(-step, 0)));

        if (ballIsOnPaddle)
        {
            ball->runAction(MoveBy::create(0, Vec2(-step, 0)));
        }

        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        step = std::min(50.0f, size.width - paddle->getPosition().x - paddle->getContentSize().width / 2);
        paddle->runAction(MoveBy::create(0, Vec2(step, 0)));

        if (ballIsOnPaddle)
        {
            ball->runAction(MoveBy::create(0, Vec2(step, 0)));
        }
        break;

    case EventKeyboard::KeyCode::KEY_ESCAPE:
        //Director::getInstance()->stopAnimation();
        break;
    }
}

void Game::setBallOnPaddle()
{
    Vec2 pos = paddle->getPosition();
    ball->runAction(MoveTo::create(0, Vec2(pos.x, pos.y + 25)));
}

void Game::gameOver()
{
    Director::getInstance()->stopAnimation();
}