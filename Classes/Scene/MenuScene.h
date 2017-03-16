#pragma once

#include "cocos2d.h"
#include "network/CCDownloader.h"

class CMenuScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CMenuScene);
	static cocos2d::Scene* createScene();
    virtual void update(float delta) override;
    
    //getter & setter
    static CMenuScene* getMenuScene(){ return m_MenuScene; }

protected:
	virtual bool init() override;

private:
	virtual ~CMenuScene();

	void clearData();
    void InitMenuSceneUI();
	void createGameScene(Node* sender);
    
private:
    static CMenuScene* m_MenuScene;
    std::unique_ptr<cocos2d::network::Downloader> downloader;
};
