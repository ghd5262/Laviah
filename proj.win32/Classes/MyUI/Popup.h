#pragma once
#include "cocos2d.h"

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
class CPopup : public cocos2d::Layer
{
	friend CSpecificPopupBase;
	enum ePOPUPINFO{
		ONEBTN,
		TWOBTN,
		FORMAT
	};

public:
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
	inline void setPopupCloseEffectFunc(const std::function<void(CPopup*)> &func)
	{
		m_PopupCloseEffect = POPUPEFFECT_custom;
		m_PopupCloseEffectFunc = func;
	}

	inline void setPopupOpenEffectFunc(const std::function<void(CPopup*)> &func)
	{
		m_PopupOpenEffect = POPUPEFFECT_custom;
		m_PopupOpenEffectFunc = func;
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
	void PopupClose(){ popupCloseEffect(); }
	void PopupRelease(){ this->scheduleOnce([this](float delta){removeFromParent(); }, 0.f, "RemovePopupAtNextFrame"); }
	void popupOpenEffect();
	void popupCloseEffect();

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

	CPopup(CSpecificPopupBase* format,
		ePOPUP_EFFECT openEffect = POPUPEFFECT_none,
		const std::function<void(CPopup*)> &openEffectFunc = nullptr);

	~CPopup(){};

	void popupOpenEffect_move(std::function<void(CPopup*)>& popup);
	void popupOpenEffect_fadeIn(std::function<void(CPopup*)>& popup);
	void popupCloseEffect_move(std::function<void(CPopup*)>& popup);
	void popupCloseEffect_fadeOut(std::function<void(CPopup*)>& popup);

private:
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