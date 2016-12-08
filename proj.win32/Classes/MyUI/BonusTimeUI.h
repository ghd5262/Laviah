#pragma once

#include "cocos2d.h"
#include "../GameObject/Bullet/BonusLetter.h"

class CLetter : public cocos2d::Node
{
public:
	static CLetter* create(
		std::string normalTextureName,
		std::string bonusTextureName,
		eLETTER letterNum,
		cocos2d::Vec2 position);

	//setter & getter
	cocos2d::Vec2 getLetterWorldPosition() const { return _parent->convertToWorldSpace(m_OriginPos); }
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

private:
	CLetter(std::string normalTextureName,
		std::string bonusTextureName,
		eLETTER letterNum,
		cocos2d::Vec2 position)
		: m_NormalTextureName(normalTextureName),
		m_BonusTextureName(bonusTextureName),
		m_LetterNum(letterNum),
		m_OriginPos(position),
		m_bIsCollected(false){}
	virtual ~CLetter(){};

private:
	cocos2d::Vec2 m_OriginPos;
	cocos2d::Sprite* m_Texture;
	std::string m_NormalTextureName;
	std::string m_BonusTextureName;
	
	eLETTER m_LetterNum;
	bool m_bIsCollected;
};

class CBonusTimeUI : public cocos2d::Node
{
public:
	static CBonusTimeUI* Instance();

    virtual void update(float delta) override;
    
	void CollectLetter(eLETTER letter);
	void LetsBonusTime();
	void BonusTimeIsFinish();
	eLETTER NonCollectedLetterNum() const;
	cocos2d::Vec2 NonCollectedLetterWorldPos() const;	

protected:
	virtual bool init() override;

private:
	CBonusTimeUI() : m_Time(0.0f){};
	virtual ~CBonusTimeUI(){};

private:
    static CBonusTimeUI* m_Instance;
	CLetter* m_LetterList[eLETTER_MAX];
	float m_Time;
};

