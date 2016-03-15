#pragma once

#include "../GameObject/GameObject.h"

enum eLETTER{
	B = 0,
	O = 1,
	N = 2,
	U = 3,
	S = 4,
	T = 5,
	I = 6,
	M = 7,
	E = 8,

	eLETTER_MAX
};

class CLetter : public CGameObject
{
public:
	static CLetter* create(
		std::string normalTextureName,
		std::string bonusTextureName,
		eLETTER letterNum,
		Vec2 position);

	virtual void Execute(float delta = 0.f);

	//setter & getter
	Vec2 getLetterWorldPosition() const { return _parent->convertToWorldSpace(m_OriginPos); }
	bool getIsCollected(){ return m_bIsCollected; }
	void setIsCollected(bool isCollect){
		m_bIsCollected = isCollect;
		if (m_Texture != nullptr){
			if (m_bIsCollected == true)
				m_Texture->setTexture(m_BonusTextureName);
			else
				m_Texture->setTexture(m_NormalTextureName);
		}
	}
protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CLetter(std::string normalTextureName,
		std::string bonusTextureName,
		eLETTER letterNum,
		Vec2 position)
		: m_NormalTextureName(normalTextureName),
		m_BonusTextureName(bonusTextureName),
		m_LetterNum(letterNum),
		m_OriginPos(position),
		m_bIsCollected(false){}
	virtual ~CLetter(){};

private:
	Vec2 m_OriginPos;
	cocos2d::Sprite* m_Texture;
	std::string m_NormalTextureName;
	std::string m_BonusTextureName;
	
	eLETTER m_LetterNum;
	bool m_bIsCollected;
};

class CBonusTimeUI : public CGameObject
{
public:
	static CBonusTimeUI* create();

	void CollectLetter(eLETTER letter);
	void LetsBonusTime();
	void BonusTimeIsFinish();
	eLETTER NonCollectedLetterNum() const;			// 만약 모두 가지고 있다면 max값 반환
	Vec2 NonCollectedLetterWorldPos() const;		// 아직 모으지 않은 문자의 월드좌표 반환
	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CBonusTimeUI() : m_fTime(0.0f){};
	virtual ~CBonusTimeUI(){};

private:
	CLetter* m_LetterList[eLETTER_MAX];
	float m_fTime;
};

