#include "PatternTestPopup.h"
#include "../MyButton.h"
#include "../TextField.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../Scene/GameScene.h"
#include "../../Common/StringUtility.hpp"
#include <vector>

CPatternTestPopup* CPatternTestPopup::create()
{
	CPatternTestPopup *pRet = new(std::nothrow) CPatternTestPopup();
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

bool CPatternTestPopup::init()
{
	if (!CPopup::init()) return false;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = LayerColor::create(COLOR::WHITEGRAY_ALPHA, 1080.f, 1500.f);
	if (bg != nullptr){
		bg->setIgnoreAnchorPointForPosition(false);
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f));
		this->addChild(bg);
	}

	auto workShopLabel = Label::createWithTTF("Pattern Test", "fonts/malgunbd.ttf", 80);
	if (workShopLabel != nullptr)
	{
		workShopLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		workShopLabel->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.9f));
		workShopLabel->setColor(COLOR::DARKGRAY);
		workShopLabel->setOpacity(0);
		bg->addChild(workShopLabel);
	}

	auto btnExit = CMyButton::create()
		->addEventListener([=](Node* sender){
		this->End(sender);
	})
		->setButtonNormalImage("endIcon.png")
		->setButtonPosition(Vec2(this->getContentSize().width * 0.92f, this->getContentSize().height * 0.05f))
		->setButtonAnchorPoint(Vec2::ANCHOR_MIDDLE)
		->show(this);
	btnExit->setOpacity(0);

	auto patternBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, bg->getContentSize().width * 0.9f, bg->getContentSize().height * 0.65f);
	if (patternBack != nullptr){
		patternBack->setIgnoreAnchorPointForPosition(false);
		patternBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		patternBack->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.475f));
		bg->addChild(patternBack);
	}

	m_TextFieldPattern = CTextField::create("pattern", "fonts/malgunbd.ttf", 40, Vec2(patternBack->getContentSize().width * 0.5f, patternBack->getContentSize().height * 0.5f));
	if (m_TextFieldPattern != nullptr)
	{
		m_TextFieldPattern->setDimensions(Size(patternBack->getContentSize().width * 0.9f, patternBack->getContentSize().height * 0.9f));
		m_TextFieldPattern->setTextColor(Color4B::BLACK);
		patternBack->addChild(m_TextFieldPattern);
	}

	auto widthBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, bg->getContentSize().width * 0.9f, bg->getContentSize().height * 0.075f);
	if (widthBack != nullptr){
		widthBack->setIgnoreAnchorPointForPosition(false);
		widthBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		widthBack->setPosition(Vec2(bg->getContentSize().width * 0.5f, bg->getContentSize().height * 0.075f));
		bg->addChild(widthBack);
	}

	m_TextFieldWidth = CTextField::create("pattern", "fonts/malgunbd.ttf", 40, Vec2(widthBack->getContentSize().width * 0.5f, widthBack->getContentSize().height * 0.5f));
	if (m_TextFieldWidth != nullptr)
	{
		m_TextFieldWidth->setDimensions(Size(widthBack->getContentSize().width * 0.9f, widthBack->getContentSize().height * 0.9f));
		m_TextFieldWidth->setTextColor(Color4B::BLACK);
		widthBack->addChild(m_TextFieldWidth);
	}

	this->setOpenAnimation([=](Node* sender){
		workShopLabel->runAction(FadeIn::create(0.5f));
		btnExit->runAction(FadeIn::create(0.5f));
		bg->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));
	});

	this->setCloseAnimation([=](Node* sender){
		workShopLabel->runAction(FadeTo::create(0.5f, 0));
		btnExit->runAction(FadeTo::create(0.5f, 0));
		bg->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
	});

	return true;
}

void CPatternTestPopup::End(Node* sender){

	if (m_TextFieldWidth->getText().size() && m_TextFieldPattern->getText().size()){
		
		auto strVec = StringUtility::split(m_TextFieldPattern->getText().c_str(), ',');
		auto lineVec = StringUtility::split(strVec.at(0).c_str(), '"');
		std::string patternString = "";

		sBULLET_PATTERN pattern;

		pattern._index = 10000;
		pattern._patternName = "test_Pattern";
		pattern._widthPadding = atol(m_TextFieldWidth->getText().c_str());
		pattern._height = strVec.size();
		pattern._width = lineVec.at(1).size();

		for (auto line : strVec)
		{
			auto lineStr = StringUtility::split(line.c_str(), '"');
			patternString += lineStr.at(1);
		}
		int idx = 0;
		for (int height = 0; height < pattern._height; height++)
		{
			for (int width = 0; width < pattern._width; width++)
			{
				pattern._pattern[(pattern._width * height) + width] = patternString[idx++];
			}
		}

		CBulletPatternDataManager::Instance()->setTestPattern(pattern);
	}

	this->popupClose();
}