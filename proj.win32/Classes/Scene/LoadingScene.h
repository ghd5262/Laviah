#pragma once

#include "cocos2d.h"
//#include "../extensions/cocos-ext.h"
//#include "../extensions/assets-manager/CCEventListenerAssetsManagerEx.h"

class AssetPanel;
class CBackGround;
class CLoadingScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(CLoadingScene);
    static cocos2d::Scene* createScene();
    virtual void update(float delta) override;
    
    //getter & setter
    static CLoadingScene* getLoadingScene(){ return m_LoadingScene; }
    
protected:
    virtual bool init() override;
    
private:
    virtual ~CLoadingScene();
    
    void onLoadEnd();
    void clearData();
    bool initVariable();
    void InitLoadingSceneUI();
    void createMenuScene();
    
private:
    static CLoadingScene* m_LoadingScene;
    AssetPanel* m_Downlaoder;
    //cocos2d::extension::AssetsManagerEx* m_AssetsManager;
    //cocos2d::extension::EventListenerAssetsManagerEx* m_AssetsManagerListener;
};