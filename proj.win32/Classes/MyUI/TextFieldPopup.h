#pragma once
#include "../Common/HSHUtility.h"

//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public Layer, public IMEDelegate
{
public:
	KeyboardNotificationLayer();

	virtual std::string subtitle() const = 0;
	virtual void onClickTrackNode(bool bClicked) = 0;

	virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info);

	// Layer
	bool onTouchBegan(Touch  *touch, Event  *event);
	void onTouchEnded(Touch  *touch, Event  *event);

protected:
	Node * _trackNode;
	Vec2  _beginPos;
};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFDefaultTest for test TextFieldTTF default behavior.
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFDefaultTest : public KeyboardNotificationLayer
{
public:
	// KeyboardNotificationLayer
	virtual std::string subtitle() const override;
	virtual void onClickTrackNode(bool bClicked);

	// Layer
	virtual void onEnter() override;
};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////