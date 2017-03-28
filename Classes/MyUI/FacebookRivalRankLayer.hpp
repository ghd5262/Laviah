#pragma once
#include "Popup.h"
#include "ui/UIListView.h"

class CFacebookRivalRankLayer : public CPopup
{
public:
    static CFacebookRivalRankLayer* create();
    void InitListView();
    
    virtual void update(float delta) override;

protected:
    virtual bool init() override;
    
private:
    CFacebookRivalRankLayer()
    : m_ListView(nullptr)
    , m_PrevScore(0)
    , m_PrevRank(0){}
    virtual ~CFacebookRivalRankLayer(){};
    
private:
    cocos2d::ui::ListView* m_ListView;
    int m_PrevScore;
    int m_PrevRank;
};