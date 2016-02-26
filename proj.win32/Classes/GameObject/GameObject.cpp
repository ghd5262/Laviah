#include "GameObject.h"

int CGameObject::m_nNextValidID = 0;

CGameObject::~CGameObject()
{
}

void CGameObject::setM_ID(int ID)
{
	CCASSERT((ID >= m_nNextValidID), "CGameObject::setM_ID(int ID) : invalid ID, ID should be equal or greater than pre id");
	m_nID = ID;
	m_nNextValidID = m_nID + 1;
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
	label->setColor(Color3B(255, 0, 0));
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
	label->setColor(Color3B(255, 0, 0));
	this->addChild(label, 100);
}