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
    void callbackRequireLatestVersion(std::string appUrl);
    void callbackFileDownloadProgress(int current, int max);
    void callbackFileDecompressProgress(int current, int max);
    
protected:
	virtual bool init() override;

private:
    CLoadingScene();
	virtual ~CLoadingScene();

	void createMenuScene();
	void createNetworkConnectPopup();

private:
    int m_DownloadRetryCount;
    cocos2d::ProgressTimer* m_ProgressBar;
};
