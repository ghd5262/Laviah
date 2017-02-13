#pragma once
#include "../../Popup.h"
#include "ui/UIListView.h"

class COptionTitlePopup : public CPopup
{
public:
    static COptionTitlePopup* create();
    
protected:
    virtual bool init() override;
    
private:
    CMyButton* createTitleDP(std::string content);
    cocos2d::ui::ListView* createListView(Size size, size_t distance, Vec2 pos);
    void TitleScrollCallback(cocos2d::Ref* ref, cocos2d::ui::ScrollView::EventType type);

    COptionTitlePopup(){};
    virtual ~COptionTitlePopup(){};
};