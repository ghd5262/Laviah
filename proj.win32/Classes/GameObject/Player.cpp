#include "Player.h"

CPlayer::CPlayer(std::string textureName, float boundingRadius, float runSpeed, float scale)
	: CMover(textureName, boundingRadius)
	, m_fRunSpeed(runSpeed)
	, m_fScale(scale)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Execute(float delta)
{
	//플레이어 런! 런!
}