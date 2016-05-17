#include "SceneTransition.h"

CSplitCircle* CSplitCircle::create(float duration, unsigned int cols)
{
	CSplitCircle *action = new (std::nothrow) CSplitCircle();

	if (action)
	{
		if (action->initWithDuration(duration, cols))
		{
			action->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(action);
		}
	}

	return action;
}

bool CSplitCircle::initWithDuration(float duration, unsigned int cols)
{
	_cols = cols;
	return TiledGrid3DAction::initWithDuration(duration, Size(cols, 1));
}

CSplitCircle* CSplitCircle::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) CSplitCircle();
	a->initWithDuration(_duration, _cols);
	a->autorelease();
	return a;
}

void CSplitCircle::startWithTarget(Node *target)
{
	TiledGrid3DAction::startWithTarget(target);
	_winSize = Director::getInstance()->getWinSizeInPixels();
}

void CSplitCircle::update(float time)
{
	for (unsigned int i = 0; i < _gridSize.width; ++i)
	{
		Quad3 coords = getOriginalTile(Vec2(i, 0));
		float    direction = 1;

		if ((i % 2) == 0)
		{
			direction = -1;
		}

		coords.bl.y += direction * _winSize.height * time;
		coords.br.y += direction * _winSize.height * time;
		coords.tl.y += direction * _winSize.height * time;
		coords.tr.y += direction * _winSize.height * time;

		setTile(Vec2(i, 0), coords);
	}
}