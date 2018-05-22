#pragma once
#include "Popup.h"
#include "ui/UIListView.h"

struct FBUSER_PARAM;
typedef std::function<void(int)> RANKUP_LISTENER;
class CFacebookRivalRankLayer : public CPopup
{
public:
    static CFacebookRivalRankLayer* create();
    CFacebookRivalRankLayer* setRankUPListener(const RANKUP_LISTENER& listener);
    void Reset();
    
    
    virtual void update(float delta) override;

protected:
    virtual bool init() override;
    
private:
    void initListView();
    cocos2d::ui::Widget* createRankDP(int &scoreRef, std::string url, int rank);
    void callListener(int rank);
    
    CFacebookRivalRankLayer()
    : m_ListView(nullptr)
    , m_RankUPListener(nullptr)
    , m_PrevScore(0)
    , m_PrevRank(0){}
    virtual ~CFacebookRivalRankLayer(){};
    
private:
    cocos2d::ui::ListView* m_ListView;
    RANKUP_LISTENER m_RankUPListener;
    int m_PrevScore;
    int m_PrevRank;
};
