#pragma once
#include "../Popup.h"

enum SIZE_TYPE{
    FULL_SIZE = 0,
    HALF_SIZE = 1,
};
class CSharePopup : public CPopup
{
public:
    static CSharePopup* create();
    CSharePopup* setSizeType(SIZE_TYPE type);
    CSharePopup* setCapturedTexture(cocos2d::Texture2D* texture);
    CSharePopup* setScore(int score);
    CSharePopup* setLogoEnable(bool enable);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
        
private:
    void end();
    void saveToFile();
    
    CSharePopup()
    : m_CapturedTexture(nullptr)
    , m_ScreenNode(nullptr)
    , m_Type(FULL_SIZE)
    , m_Score(0)
    , m_LogoEnable(false){};
    virtual ~CSharePopup(){};
    
private:
    cocos2d::Texture2D* m_CapturedTexture;
    cocos2d::Node* m_ScreenNode;
    SIZE_TYPE m_Type;
    int m_Score;
    bool m_LogoEnable;
};
