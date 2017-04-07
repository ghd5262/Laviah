#pragma once
#include "Bullet.h"

enum eSTAR_TYPE;
class CPlayStar : public CBullet
{
public:
	/*create를 호출하면 CObjectManager에서 메모리를 받는다.
	받은 메모리는 메모리풀에 미리 생성되어있던 메모리이다.*/
    static CPlayStar* create();

	virtual void CollisionWithPlayer() override;
    virtual void Execute(float delta) override;
protected:
	virtual bool init() override;

private:
	CPlayStar();
	virtual ~CPlayStar(){};
    
private:
    int m_OldLevel;
};


