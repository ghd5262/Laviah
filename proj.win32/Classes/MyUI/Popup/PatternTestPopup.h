#pragma once
#include "../Popup.h"
#include <vector>

class CTextField;
class CPatternTestPopup : public CPopup
{
public:
	static CPatternTestPopup* create();

protected:
	virtual bool init() override;

private:
	void End(cocos2d::Node* sender);
	void BulletSelect(cocos2d::Node* sender);
	CPatternTestPopup() 
		: m_SelectButton(nullptr)
		, m_BulletTypeList("")
		, m_CurrentBullet(0){}
		//: m_TextFieldPattern(nullptr)
		//, m_TextFieldWidth(nullptr){};
	virtual ~CPatternTestPopup(){};

private:
	//CTextField* m_TextFieldPattern;
	//CTextField* m_TextFieldWidth;
	cocos2d::ui::Button* m_SelectButton;
	int m_CurrentBullet;
	std::string m_BulletTypeList;
	std::vector<std::string> m_TextureList;
};