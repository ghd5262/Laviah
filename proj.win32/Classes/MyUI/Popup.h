#pragma once
#include "../Common/HSHUtility.h"

USING_NS_CC;


class CMyButton;
class CUIEffect;
class CSpecificPopupBase;
enum ePOPUP_EFFECT{
	POPUPEFFECT_none,
	POPUPEFFECT_default_move,
	POPUPEFFECT_default_fade,
	POPUPEFFECT_custom
};
class CPopup : public cocos2d::Node
{
	typedef std::function<void(Node*)> FUNCTION_CALLBACK;
    typedef cocos2d::ui::Button::ccWidgetTouchCallback BUTTON_CALLBACK;
    
	friend CSpecificPopupBase;
	enum ePOPUPINFO{
		ONEBTN,
		TWOBTN,
		FORMAT
	};

public:
    static CPopup* create();
    
	static CPopup* create(CSpecificPopupBase* test);

	static CPopup* createWithOneButton(std::string popupNotice,
		CMyButton* button,
		size_t fontSize = 20,
		Color3B fontColor = Color3B::BLACK,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	static CPopup* createWithTwoButton(std::string popupNotice,
		CMyButton* button1,
		CMyButton* button2,
		size_t fontSize = 20,
		Color3B fontColor = Color3B::BLACK,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	static CPopup* createWithSpecificFormat(CSpecificPopupBase* format,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	//getter & setter
	CPopup* setPositiveButton(const BUTTON_CALLBACK &callback, std::string btnName);
	CPopup* setNegativeButton(const BUTTON_CALLBACK &callback, std::string btnName);
	CPopup* setWidget(const FUNCTION_CALLBACK &func);
	CPopup* setOpenEffect(const FUNCTION_CALLBACK &func);
	CPopup* setCloseEffect(const FUNCTION_CALLBACK &func);
	CPopup* setMessage(std::string message);
	CPopup* setMessageFont(Color3B fontColor, int size);
	CPopup* setButtonFont(Color3B fontColor, int size);
    CPopup* WidgetInitTest();
    CPopup* setPopupPosition(cocos2d::Vec2 position);
    CPopup* setPopupAnchorPoint(cocos2d::Vec2 anchorPoint);
	CPopup* show(Node* parent, int zOrder = 0);

	inline void setPopupCloseEffectFunc(const std::function<void(CPopup*)> &func)
	{
//		m_PopupCloseEffect = POPUPEFFECT_custom;
//		m_PopupCloseEffectFunc = func;
	}

	inline void setPopupOpenEffectFunc(const std::function<void(CPopup*)> &func)
	{
//		m_PopupOpenEffect = POPUPEFFECT_custom;
//		m_PopupOpenEffectFunc = func;
        
        m_OpenEffectFunc = [=](Node* sender){
            if(func != nullptr)
                func(dynamic_cast<CPopup*>(sender));
        };
	}

	virtual void setPosition(const Vec2& position) override;
protected:
	virtual bool init() override;
	bool initVariable();

	//getter & setter
	CC_SYNTHESIZE(Label*, m_NoticeLabel, PopupNoticeLabel);
	CC_SYNTHESIZE(ePOPUP_EFFECT, m_PopupOpenEffect, PopupOpenEffect);
	CC_SYNTHESIZE(ePOPUP_EFFECT, m_PopupCloseEffect, PopupCloseEffect);

private:
	void playEffect(const FUNCTION_CALLBACK &func);

	void PopupClose(){ popupCloseEffect(); }
	void PopupRelease(){ this->scheduleOnce([this](float delta){removeFromParent(); }, 0.f, "RemovePopupAtNextFrame"); }
	void popupOpenEffect();
	void popupCloseEffect();
	
    CPopup();
    
	CPopup(CSpecificPopupBase* test);

	CPopup(std::string popupNotice,
		CMyButton* button,
		size_t fontSize,
		Color3B fontColor,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	CPopup(std::string popupNotice,
		CMyButton* button1,
		CMyButton* button2,
		size_t fontSize,
		Color3B fontColor,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

//	CPopup(CSpecificPopupBase* format,
//		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
//		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	~CPopup(){};

	void popupOpenEffect_move(std::function<void(CPopup*)>& popup);
	void popupOpenEffect_fadeIn(std::function<void(CPopup*)>& popup);
	void popupCloseEffect_move(std::function<void(CPopup*)>& popup);
	void popupCloseEffect_fadeOut(std::function<void(CPopup*)>& popup);

private:
    BUTTON_CALLBACK m_PositiveButtonFunc;
	BUTTON_CALLBACK m_NegativeButtonFunc;
	FUNCTION_CALLBACK m_WidgetInitFunc;
	FUNCTION_CALLBACK m_OpenEffectFunc;
	FUNCTION_CALLBACK m_CloseEffectFunc;

	std::string m_Message;
	std::string m_PositiveButtonName;
	std::string m_NegativeButtonName;
	Color3B m_MessageFontColor;
	Color3B m_ButtonFontColor;
	int m_MessageFontSize;
	int m_ButtonFontSize;
    Vec2 m_Position;
    Vec2 m_AnchorPoint;

	std::shared_ptr<CSpecificPopupBase> m_PopupAble;
	std::function<void(CPopup*)> m_PopupOpenEffectFunc;
	std::function<void(CPopup*)> m_PopupCloseEffectFunc;

	CMyButton* m_Btn1;
	CMyButton* m_Btn2;

	std::string m_PopupNotice;
	size_t m_FontSize;
	Color3B m_FontColor;

	ePOPUPINFO m_PopupInfo;

	Vec2 m_OriginPos;
};


class CSpecificPopupBase{
public:
	virtual bool init(CPopup* popup);
	CSpecificPopupBase(){};
	virtual ~CSpecificPopupBase(){};

    inline void PopupRelease(){ m_Popup->PopupRelease(); }
    inline void PopupClose(){ m_Popup->PopupClose(); }
    
protected:
	virtual bool initVariable() = 0;
    
protected:
	CPopup* m_Popup;
};