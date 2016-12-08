#pragma once

#include "cocos2d.h"

class CScoreUI : public cocos2d::Node
{
public:
	static CScoreUI* create(std::string fontName, size_t fontSize, std::string valueImgName = "");

	virtual void update(float delta) override;
	void setLabelAnchor(cocos2d::Vec2 point);
	void addValue(int value);
	void setValue(int value);

	static std::string insertComma(unsigned value);

	//getter & setter
	std::string getValueString() const { return m_ValueString; }
	CC_SYNTHESIZE(unsigned int, m_Value, ScoreValue);

protected:
	virtual bool init() override;

private:
	CScoreUI(std::string fontName, size_t fontSize, std::string valueImgName)
		: m_ValueImgName(valueImgName)
		, m_FontName(fontName)
		, m_FontSize(fontSize)
		, m_ValueLabel(nullptr)
		, m_ValueImg(nullptr)
		, m_ValueString("0")
		, m_Value(0){};
	virtual ~CScoreUI(){};

private:
	std::string m_FontName;
	std::string m_ValueImgName;
	cocos2d::Label* m_ValueLabel;
	cocos2d::Sprite* m_ValueImg;// 숫자 오른쪽에 보여줄 이미지 없다면 보여주지 않는다.
	size_t m_FontSize;
	std::string m_ValueString;
};

