#include "GameObject.h"

using namespace cocos2d;

void CGameObject::DrawDebugBinding()
{
	if (!DEBUG_DRAW)
		return;

	for (int angle = 0; angle <= 360; angle++)
	{
		int x = (cos(CC_DEGREES_TO_RADIANS(angle)) * m_fBoundingRadius) + getPosition().x;
		int y = (sin(CC_DEGREES_TO_RADIANS(angle)) * m_fBoundingRadius) + getPosition().y;
		DrawDebugLine(getPosition(), Point(x, y));
	}
}

void CGameObject::DrawDebugRect(Point pos1, Point pos2, std::string text/* = "" */)
{
	if (!DEBUG_DRAW)
		return;
	auto d = DrawNode::create();
	d->clear();
	d->drawRect( pos1, pos2, Color4F(1, 0, 0, 1) );
	addChild(d, 100);

	auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15);
	Point temp = Point((pos2.x - pos1.x) * 0.5f, (pos2.y - pos1.y) * 0.5f);
	label->setPosition(Vec2((pos1.x + temp.x), (pos1.y + temp.y)));
	label->setColor(Color3B(0, 0, 255));
	this->addChild(label, 100);
}

void CGameObject::DrawDebugLine(Point pos1, Point pos2, std::string text/* = "" */)
{
	if (!DEBUG_DRAW)
		return;
	auto d = DrawNode::create();
	d->clear();
	d->drawLine(pos1, pos2, Color4F(1, 0, 0, 1));
	addChild(d, 100);

	auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 15);
	Point temp = Point((pos2.x - pos1.x) * 0.5f, (pos2.y - pos1.y) * 0.5f);
	label->setPosition(Vec2((pos1.x + temp.x), (pos1.y + temp.y)));
	label->setColor(Color3B(0, 0, 255));
	this->addChild(label, 100);
}