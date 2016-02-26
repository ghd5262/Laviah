#include "SoccerBall.h"

bool CSoccerBall::init()
{
	if (!initVariable())
		return false;
	return true;
}

CSoccerBall::~CSoccerBall()
{
	CC_SAFE_RELEASE(m_pImgSoccerBall);
}

bool CSoccerBall::initVariable()
{
	try
	{
		m_pImgSoccerBall = Sprite::create("SoccerBall.png");
		if (m_pImgSoccerBall != nullptr){
			CC_SAFE_RETAIN(m_pImgSoccerBall);
			m_pImgSoccerBall->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pImgSoccerBall);
		}
	}
	catch(...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

Vec2 CSoccerBall::addNoiseToKick(Vec2 ballPos, Vec2 ballTarget)
{
	double displacement = (M_PI - M_PI * 20.0f) * cocos2d::rand_0_1(); /// [temp] 10.0f 는 슈팅강도

	Vec2 toTarget = ballPos.rotateByAngle(ballTarget, displacement); ///[temp] rotate  값 테스트

	return toTarget + ballPos;
}

void CSoccerBall::kick(Vec2 direction, double force)
{
	direction.getNormalized();
	Vec2 acceleration = (direction * force) / m_dMass;
	m_velocityVec = acceleration;
}

double CSoccerBall::timeToCoverDistance(Vec2 from, Vec2 to, double force) const
{
	double speed = force / m_dMass;

	double DistanceToCover = from.getDistance( to );

	double term = speed * speed + 2.0f * DistanceToCover *  5.0f; /// [temp] 5.0f 는 마찰력

	if (term <= 0.0f) return -1.0f;

	double v = sqrt(term);

	return (v - speed) / 5.0f;
}

Vec2 CSoccerBall::FuturePosition(double time) const
{
	/// 식 x = ut + 1/2at^2 ( x = 거리, a = 마찰, /u = 시작속도)
	Vec2 ut = m_velocityVec * time;

	double half_a_t_squared = 0.5f * 5.0f * time * time;  /// [temp] 5.0f 는 마찰력

	Vec2 ScalarToVector = half_a_t_squared * m_velocityVec.getNormalized();

	return this->getPosition() + ut + ScalarToVector;
}

void CSoccerBall::PlaceAtPosition(Vec2 newPos)
{
	this->setPosition(newPos);
	m_oldPos = this->getPosition();
	m_velocityVec.ZERO;
}