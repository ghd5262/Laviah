#pragma once
#include "../../Popup.h"
#include "ui/UIListView.h"

class COptionMoreGamesPopup : public CPopup
{
public:
    static COptionMoreGamesPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    CMyButton* createGameDP(std::string url);
    cocos2d::ui::ListView* createListView(Size size, size_t distance, Vec2 pos);
    
    COptionMoreGamesPopup(){};
    virtual ~COptionMoreGamesPopup(){};
};