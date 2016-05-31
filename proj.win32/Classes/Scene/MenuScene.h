#pragma once

#include "cocos2d.h"

class CMenuScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CMenuScene);
	static cocos2d::Scene* createScene();
	virtual ~CMenuScene();

    virtual void update(float delta) override;
    
    //getter & setter
    static CMenuScene* getGameScene(){ return m_MenuScene; }

protected:
	virtual bool init() override;

private:
	bool initVariable();
    void InitMenuSceneUI();
    void createGameScene();
    
private:
    static CMenuScene* m_MenuScene;
};
