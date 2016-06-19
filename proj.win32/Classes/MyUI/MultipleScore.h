#pragma once
#include "../GameObject/GameObject.h"

class CScoreUI;
class CPlayer;

class CMultipleScore : public CGameObject
{
	const float MULTIPLE_TIME_LIMIT = 5.f;

public:
	static CMultipleScore* create();
	void AddScore(unsigned score);
	virtual void Execute(float delta = 0.f);


protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	void calculateScore();
	void multipleScoreReset();

	CMultipleScore();
	virtual ~CMultipleScore(){};

private:
	CScoreUI* m_StarScore;
	Label* m_MultipleNumberLabel;
	CPlayer* m_Player;
	unsigned m_MultipleNumber;		//���� ���ھ� ���( MULTIPLE_TIME_LIMIT ���� ������ �� 1�� ����� )
	unsigned m_SavedScore;			//������� ����� ����( MULTIPLE_TIME_LIMIT ���� ������ �� 0���� ����� )
	bool m_isAbleToMultiple;	//������ ���� ȹ�� �ð����� ���� MULTIPLE_TIME_LIMIT���� true�� �����ȴ�.
	float m_fTime;
};