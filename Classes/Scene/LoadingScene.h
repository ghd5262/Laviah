#pragma once

#include "cocos2d.h"

class CLoadingScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(CLoadingScene);
	static cocos2d::Scene* createScene();
    void startDownload();
    
	//callback
	void callbackDownloadFail();
	void callbackDownloadComplete();

protected:
	virtual bool init() override;

private:
    CLoadingScene();
	virtual ~CLoadingScene();

	void createMenuScene();
	void createNetworkConnectPopup();

private:
    int m_DownloadRetryCount;
};
