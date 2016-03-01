#include "GameObject.h"

bool CGameObject::init()
{
	if (!CGameObject::initVariable())
		return false;
	return true;
}

bool CGameObject::initVariable()
{
	try{
		srand((unsigned)time(NULL));

		m_pTexture = Sprite::create(m_TextureName);
		if (m_pTexture != nullptr){
			m_pTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pTexture);
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
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