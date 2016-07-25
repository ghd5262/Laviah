#include "TextField.h"

CTextField* CTextField::create(const std::string& placeholder, const std::string& fontName, float fontSize, Vec2 pos)
{
    CTextField *pRet = new(std::nothrow) CTextField(placeholder, fontName, fontSize, pos);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CTextField::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CTextField::initVariable()
{
    try{
        //Register Touch Event
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(CTextField::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(CTextField::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        // add TextFieldTTF
        m_TrackNode = TextFieldTTF::textFieldWithPlaceHolder(m_PlaceHolderString, m_FontName,
                                                             m_FontSize);
        
        if(m_TrackNode != nullptr){
            m_TrackNode->setPosition(m_Pos);
            addChild(m_TrackNode);
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

Rect getRect(Node * node)
{
	Rect rc;
	rc.origin = node->getPosition();
	rc.size = node->getContentSize();
	rc.origin.x -= rc.size.width / 2;
	rc.origin.y -= rc.size.height / 2;
	return rc;
}


void CTextField::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
	CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
		info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

	if (!m_TrackNode)
	{
		return;
	}

	auto rectTracked = getRect(m_TrackNode);
	CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
		rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

	// if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
	if (!rectTracked.intersectsRect(info.end))
	{
		return;
	}

	// assume keyboard at the bottom of screen, calculate the vertical adjustment.
	float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
	CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

	// move all the children node of KeyboardNotificationLayer
	auto& children = getChildren();
	Node * node = 0;
	ssize_t count = children.size();
	Vec2 pos;
	for (int i = 0; i < count; ++i)
	{
		node = children.at(i);
		pos = node->getPosition();
		pos.y += adjustVert;
		node->setPosition(pos);
	}
}

// Layer function

bool CTextField::onTouchBegan(Touch  *touch, Event  *event)
{
	CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
	m_TouchBeginPos = touch->getLocation();
	return true;
}

void CTextField::onTouchEnded(Touch  *touch, Event  *event)
{
	if (!m_TrackNode)
	{
		return;
	}

	auto endPos = touch->getLocation();

	float delta = 5.0f;
	if (::abs(endPos.x - m_TouchBeginPos.x) > delta
		|| ::abs(endPos.y - m_TouchBeginPos.y) > delta)
	{
		// not click
		m_TouchBeginPos.x = m_TouchBeginPos.y = -1;
		return;
	}

	// decide the trackNode is clicked.
	Rect rect;
	auto point = convertTouchToNodeSpaceAR(touch);
	CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);

	rect = getRect(m_TrackNode);
	CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
		rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

	this->onClickTrackNode(rect.containsPoint(point));
	CCLOG("----------------------------------");
}

void CTextField::onClickTrackNode(bool bClicked)
{
	auto pTextField = m_TrackNode;
	if (bClicked)
	{
		// TextFieldTTFTest be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
		pTextField->attachWithIME();
	}
	else
	{
		// TextFieldTTFTest not be clicked
		CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
		pTextField->detachWithIME();
	}
}