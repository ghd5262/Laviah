#pragma once
#include "../Popup.h"
#include <vector>

namespace PATTERN_TEST {
    static const int CELL_WIDTH = 30;
    static const int CELL_HEIGHT = 60;
}
using namespace PATTERN_TEST;
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
    void CreateBullet(cocos2d::Node* sender);
	CPatternTestPopup() 
		: m_SelectButton(nullptr)
		, m_BulletTypeList("")
		, m_CurrentBullet(0)
		//: m_TextFieldPattern(nullptr)
		, m_TextFieldWidth(nullptr){};
	virtual ~CPatternTestPopup(){};

private:
	//CTextField* m_TextFieldPattern;
	CTextField* m_TextFieldWidth;
	cocos2d::ui::Button* m_SelectButton;
    cocos2d::ui::Button* m_Cell[CELL_WIDTH * CELL_HEIGHT];
    std::string m_Pattern[CELL_HEIGHT];
	std::string m_BulletTypeList;
	std::vector<std::string> m_TextureList;
    int m_CurrentBullet;
};