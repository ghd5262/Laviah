#pragma once
#include "cocos2d.h"
#include <array>

class CComboScore : public cocos2d::Node
{
	const float MULTIPLE_TIME_LIMIT = 5.f;

public:
	static CComboScore* Instance();
	void AddCombo();
    void ComboScoreReset();
    void LabelUpdate();
    virtual void update(float delta) override;

    CC_SYNTHESIZE(bool, m_IsPause, IsPause);
protected:
	virtual bool init() override;

private:
	void calculateScore();

	CComboScore();
	virtual ~CComboScore();

private:
    static CComboScore* m_Instance;
	cocos2d::Label* m_ComboLabel;
	float m_Time;
    int m_OldLevel;
};
