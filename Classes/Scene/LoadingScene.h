#pragma once

#include "cocos2d.h"

class CDownloadManager;
class CBackGround;
class CLoadingScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CLoadingScene);
	static cocos2d::Scene* createScene();
	virtual void update(float delta) override;

	//callback
	void callbackNetworkResult();
	void callbackLoginResult(Ref* object);
	void callbackUserDataLoadFinish(Ref* object);
	void callbackDownloadFail(Ref* object);
	void callbackDownloadComplete(Ref* object);

	//getter & setter
	static CLoadingScene* getLoadingScene(){ return m_LoadingScene; }

protected:
	virtual bool init() override;

private:
	virtual ~CLoadingScene();

	void clearData();
	void InitLoadingSceneUI();
	void createMenuScene();
	void createNetworkConnectPopup();

private:
	static CLoadingScene* m_LoadingScene;
	CDownloadManager* m_Downlaoder;
};