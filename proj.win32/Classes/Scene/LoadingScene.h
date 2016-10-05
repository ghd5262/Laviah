#pragma once

#include "cocos2d.h"

class AssetPanel;
class CBackGround;
class CLoadingScene : public cocos2d::Layer
{
public:
    CREATE_FUNC(CLoadingScene);
    static cocos2d::Scene* createScene();
    virtual void update(float delta) override;
    
    //assets downloader callback
    void onAssetUpdateError(Ref* object);
    void onAssetUpdateComplete(Ref* object);
    
    //getter & setter
    static CLoadingScene* getLoadingScene(){ return m_LoadingScene; }
    
protected:
    virtual bool init() override;
    
private:
    virtual ~CLoadingScene();
    
    void clearData();
    bool initVariable();
    void InitLoadingSceneUI();
    void createMenuScene();
    
private:
    static CLoadingScene* m_LoadingScene;
    AssetPanel* m_Downlaoder;
};