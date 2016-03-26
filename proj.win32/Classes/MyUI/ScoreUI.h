#pragma once
#include "../GameObject/GameObject.h"

class CScoreUI : public CGameObject
{
public:
	static CScoreUI* create(std::string fontName, size_t fontSize, std::string valueImgName = "");

	virtual void Execute(float delta = 0.f);
	void UpdateValue(int number);
	void SetLabelAnchor(Vec2 point);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	void insertComma(const char* valueStr, char* resultStr);
	CScoreUI(std::string fontName, size_t fontSize, std::string valueImgName)
		: m_ValueImgName(valueImgName)
		, m_FontName(fontName)
		, m_FontSize(fontSize)
		, m_ValueLabel(nullptr)
		, m_ValueImg(nullptr)
		, m_Value(0){};
	virtual ~CScoreUI(){};

private:
	std::string m_FontName;
	std::string m_ValueImgName;
	Label* m_ValueLabel;	
	Sprite* m_ValueImg;// 숫자 오른쪽에 보여줄 이미지 없다면 보여주지 않는다.
	size_t m_FontSize;
	unsigned int m_Value;
	char m_ValueString[30];
};

