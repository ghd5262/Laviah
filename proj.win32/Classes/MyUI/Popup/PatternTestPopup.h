#pragma once
#include "../Popup.h"

class CTextField;
class CPatternTestPopup : public CPopup
{
public:
	static CPatternTestPopup* create();

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);

	CPatternTestPopup() 
		: m_TextFieldPattern(nullptr)
		, m_TextFieldWidth(nullptr){};
	virtual ~CPatternTestPopup(){};

private:
	CTextField* m_TextFieldPattern;
	CTextField* m_TextFieldWidth;
};