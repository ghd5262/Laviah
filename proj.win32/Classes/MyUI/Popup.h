#pragma once
#include "../Common/HSHUtility.h"
#include <list>
USING_NS_CC;

enum class ePOPUP_ANIMATION{
	NONE = 0,
	OPEN_LEFT,
	OPEN_RIGHT,
	OPEN_DOWN,
	OPEN_UP,
	OPEN_CENTER,
	CLOSE_LEFT,
	CLOSE_RIGHT,
	CLOSE_DOWN,
	CLOSE_UP,
	CLOSE_CENTER
};

class CMyButton;
class CUIEffect;
class CSpecificPopupBase;
class CPopup : public cocos2d::Node
{
    typedef std::function<void(Node*)> NODE_CALLBACK;
    struct sDEFAULT_CALLBACK {
        NODE_CALLBACK _callback;
		CPopup* _sender;
        
		sDEFAULT_CALLBACK(const NODE_CALLBACK &callback, CPopup* sender)
        : _callback(callback)
        , _sender(sender){}
    };
    typedef std::list<sDEFAULT_CALLBACK>  CALLBACK_STACK;

public:
    static CPopup* create();
   
	CPopup* setPositiveButton(const NODE_CALLBACK &callback, std::string btnName);
	CPopup* setNegativeButton(const NODE_CALLBACK &callback, std::string btnName);
	CPopup* setDefaultCallback(const NODE_CALLBACK &callback, bool cleanUp = true);
	CPopup* setOpenAnimation(const NODE_CALLBACK &callback);
	CPopup* setCloseAnimation(const NODE_CALLBACK &callback);
	CPopup* setDefaultAnimation(ePOPUP_ANIMATION open, ePOPUP_ANIMATION close);
	CPopup* setMessage(std::string message);
	CPopup* setMessageFont(Color3B fontColor, int size);
	CPopup* setButtonFont(Color3B fontColor, int size);
    CPopup* setPopupPosition(cocos2d::Vec2 position);
    CPopup* setPopupAnchorPoint(cocos2d::Vec2 anchorPoint);
	CPopup* setBackgroundColor(Color4B color);
    CPopup* setBackgroundVisible(bool visible);
    CPopup* setDefaultCallbackEnable(bool enable);
	virtual CPopup* show(Node* parent = nullptr, int zOrder = 0);

	void popupOpenAnimation();
	void popupClose();
    void popupTouchEnable(bool enable);
    void setDefaultCallbackToTopAgain();
    void removeDefaultCallbackFromStack();
    std::string getMessage() const { return m_Message; }
    
    static void DefaultCallback();
	
	CC_SYNTHESIZE(bool, m_DefaultCallbackCleanUp, DefaultCallbackCleanUp);
protected:
	virtual bool init() override;

	CPopup();
	virtual ~CPopup();

private:
	void backgroundTouchDisable();
    void popupTouchDisable();

private:
	NODE_CALLBACK m_PositiveButtonCallBack;
	NODE_CALLBACK m_NegativeButtonCallBack;
    NODE_CALLBACK m_DefaultCallBack;
	NODE_CALLBACK m_OpenAnimationCallBack;
	NODE_CALLBACK m_CloseAnimationCallBack;

	CMyButton* m_EmptyBackground;
    CMyButton* m_TouchDisable;

	std::string m_Message;
	std::string m_PositiveButtonName;
	std::string m_NegativeButtonName;
	Color4B m_BackgroundColor;
	Color3B m_MessageFontColor;
    Color3B m_ButtonFontColor;
    Vec2 m_Position;
    Vec2 m_AnchorPoint;
    ePOPUP_ANIMATION m_PopupOpenAnimation;
    ePOPUP_ANIMATION m_PopupCloseAnimation;
	int m_MessageFontSize;
	int m_ButtonFontSize;
    bool m_BackgroundVisible;
    bool m_DefaultCallbackEnable;

    static CALLBACK_STACK m_DefaultCallbackStack;
};