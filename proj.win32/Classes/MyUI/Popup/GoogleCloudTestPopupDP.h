#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CGoogleCloudTestPopupDP : public cocos2d::Layer
{
public:
	static CGoogleCloudTestPopupDP* create(int workshopItemIdx, const std::function<void(cocos2d::Ref*)> &func);
	void DeSelect();

protected:
	virtual bool init() override;
	bool initVariable();

private:
	void Buy();

	CGoogleCloudTestPopupDP(int workshopItemIdx, const std::function<void(cocos2d::Ref*)> &func)
		: m_WorkshopItemIdx(workshopItemIdx)
		, m_SelectFunc(func){};
	virtual ~CGoogleCloudTestPopupDP(){};

private:
	int m_WorkshopItemIdx;
	std::function<void(cocos2d::Ref*)> m_SelectFunc;
};

#include "../Popup.h"

class CMyButton;
class CTextFieldPopup : public Layer, public IMEDelegate, public CSpecificPopupBase
{
public:
	static CTextFieldPopup* create();

protected:
	virtual bool initVariable() override;

private:
	virtual void onClickTrackNode(bool bClicked);
	virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info);

	// Layer
	bool onTouchBegan(Touch  *touch, Event  *event);
	void onTouchEnded(Touch  *touch, Event  *event);

	Rect getRect(Node * node);

	void End();
	void Select(cocos2d::Ref* dp);
	
	CTextFieldPopup()
		: _trackNode(0)
		, m_btnEnd(nullptr)
		, m_BG(nullptr){}
	virtual ~CTextFieldPopup(){};

private:
	TextFieldTTF* _trackNode;
	Vec2  _beginPos;
	CMyButton* m_btnEnd;
	LayerColor* m_BG;
};