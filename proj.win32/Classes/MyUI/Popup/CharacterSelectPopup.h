#pragma once
#include "../Popup.h"

class CMyButton;
class CCharacterSelectPopup : public CSpecificPopupBase
{
public:
    static CCharacterSelectPopup* create();
    
protected:
    virtual bool initVariable() override;
    
private:
    void End();
	void Select(cocos2d::Ref* dp);
    
    CCharacterSelectPopup()
    : m_btnEnd(nullptr)
    , m_BG(nullptr)
    , m_ScrollBack(nullptr){};
    virtual ~CCharacterSelectPopup(){};
    
private:
    CMyButton* m_btnEnd;
    Sprite* m_BG;
    Sprite* m_ScrollBack;
};