#pragma once
#include "../Common/HSHUtility.h"

class CTextField : public Layer, public IMEDelegate
{
public:
    static CTextField* create(const std::string& placeholder, const std::string& fontName, float fontSize, Vec2 pos);
    
	// KeyboardNotificationLayer
	virtual void onClickTrackNode(bool bClicked);
    
    // Layer
    bool onTouchBegan(Touch  *touch, Event  *event) override;
    void onTouchEnded(Touch  *touch, Event  *event) override;
    
    virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info) override;
    
    // getter % setter
    void setTextColor(Color4B color) {m_TrackNode->setTextColor(color);}
    void setPlaceHolderColor(Color4B color) {m_TrackNode->setColorSpaceHolder(color);}
    void setDimensions(Size size) {m_TrackNode->setDimensions(size.width, size.height);}
    std::string getText() {return m_TrackNode->getString();}
    void setText(std::string text) {m_TrackNode->setString(text);}
    
protected:
    virtual bool init() override;
    bool initVariable();
    
private:
    CTextField(const std::string& placeholder,
               const std::string& fontName,
               float fontSize,
               Vec2 pos)
    : m_TrackNode(nullptr)
    , m_PlaceHolderString(placeholder)
    , m_FontName(fontName)
    , m_FontSize(fontSize)
    , m_Pos(pos)
    , m_TouchBeginPos(Vec2::ZERO){}
    virtual ~CTextField(){};
    
private:
    TextFieldTTF* m_TrackNode;
    Vec2          m_Pos;
    Vec2          m_TouchBeginPos;
    std::string   m_PlaceHolderString;
    std::string   m_FontName;
    size_t        m_FontSize;
};