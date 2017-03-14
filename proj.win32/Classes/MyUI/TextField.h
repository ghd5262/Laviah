#pragma once
#include "../Common/HSHUtility.h"

class CTextField : public cocos2d::Layer, public cocos2d::IMEDelegate
{
public:
    static CTextField* create(const std::string& placeholder, const std::string& fontName, float fontSize, cocos2d::Vec2 pos);
    
	// KeyboardNotificationLayer
	virtual void onClickTrackNode(bool bClicked);
    
    // Layer
    bool onTouchBegan(cocos2d::Touch  *touch, cocos2d::Event  *event) override;
    void onTouchEnded(cocos2d::Touch  *touch, cocos2d::Event  *event) override;
    
    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
    
    // getter % setter
    void setTextColor(cocos2d::Color4B color) {m_TrackNode->setTextColor(color);}
    void setPlaceHolderColor(cocos2d::Color4B color) {m_TrackNode->setColorSpaceHolder(color);}
    void setDimensions(cocos2d::Size size) {m_TrackNode->setDimensions(size.width, size.height);}
    std::string getText() {return m_TrackNode->getString();}
    void setText(std::string text) {m_TrackNode->setString(text);}
    
protected:
    virtual bool init() override;
    
private:
    CTextField(const std::string& placeholder,
               const std::string& fontName,
               float fontSize,
               cocos2d::Vec2 pos)
    : m_TrackNode(nullptr)
    , m_PlaceHolderString(placeholder)
    , m_FontName(fontName)
    , m_FontSize(fontSize)
    , m_Pos(pos)
    , m_TouchBeginPos(cocos2d::Vec2::ZERO){}
    virtual ~CTextField(){};
    
private:
    cocos2d::TextFieldTTF* m_TrackNode;
    cocos2d::Vec2          m_Pos;
    cocos2d::Vec2          m_TouchBeginPos;
    std::string   m_PlaceHolderString;
    std::string   m_FontName;
    size_t        m_FontSize;
};