#pragma once

#include "cocos2d.h"

class CUIManager;
class CGameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CGameScene);
	static cocos2d::Scene* createScene();
	virtual ~CGameScene();

	virtual void update(float delta) override;

	//getter & setter
	static CGameScene* getGameScene(){ return m_GameScene; }
protected:
	virtual bool init() override;

private:
	bool initVariable();
	void menuCloseCallback(cocos2d::Ref* pSender);
	void rightButtonCallback(cocos2d::Ref* pSender);
	void leftButtonCallback(cocos2d::Ref* pSender);

private:
	static CGameScene* m_GameScene;
	CUIManager* m_GameSceneUIMananger;
};
