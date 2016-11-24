#include "PatternTestPopup.h"
#include "../MyButton.h"
#include "../TextField.h"
#include "../../DataManager/BulletPatternDataManager.h"
#include "../../Scene/GameScene.h"
#include "../../Common/StringUtility.h"
#include "ui/UIListView.h"
#include <vector>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

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

	m_BulletTypeList = " 1456ABCDEFPTUYZz";
	for (auto symbol : m_BulletTypeList)
	{
		std::string name;
		if (symbol == ' ')							name = "b_0.png";
		else if (symbol == '1')						name = "b_1.png";
		else if (symbol == '4')                     name = "b_2.png";
		else if (symbol == '5')                     name = "b_3.png";
		else if (symbol == '6')                     name = "b_4.png";

		else if (symbol == 'A')                     name = "b_5.png";
		else if (symbol == 'B')                     name = "b_6.png";
		else if (symbol == 'C')                     name = "b_7.png";
		else if (symbol == 'D')                     name = "b_8.png";
		else if (symbol == 'E')                     name = "b_9.png";
		else if (symbol == 'F')                     name = "b_10.png";

		else if (symbol == 'P')                     name = "b_11.png";
		else if (symbol == 'T')                     name = "b_12.png";

		else if (symbol == 'U')                     name = "b_13.png";
		else if (symbol == 'Y')                     name = "b_14.png";

		else if (symbol == 'Z')                     name = "b_15.png";
        else if (symbol == 'z')                     name = "b_16.png";
        

		m_TextureList.push_back(name);
	}
    
    for( int h = 0; h < CELL_HEIGHT; h++ )
    {
        m_Pattern[h] = "00000000000000000000000000000000000000000000";
    }

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

	m_SelectButton = Button::create(m_TextureList.at(0), "", "", Widget::TextureResType::PLIST);
	m_SelectButton->addClickEventListener([=](Ref* sender){
		this->BulletSelect(static_cast<Node*>(sender));
	});
	m_SelectButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_SelectButton->setPosition(Vec2(this->getContentSize().width * 0.08f, this->getContentSize().height * 0.05f));
	m_SelectButton->setOpacity(0);
	m_SelectButton->setScale(2.5f);
	m_SelectButton->setRotation(-90);
	this->addChild(m_SelectButton);

    // Create the list view
    auto listViewH = ListView::create();
    listViewH->setDirection(ScrollView::Direction::VERTICAL);
    listViewH->setBounceEnabled(true);
    listViewH->setBackGroundImageScale9Enabled(true);
    listViewH->setContentSize(Size(bg->getContentSize().width, bg->getContentSize().height * 0.7f));
    listViewH->setScrollBarPositionFromCorner(Vec2(7, 7));
    listViewH->setItemsMargin(0);
    listViewH->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    listViewH->setPosition(bg->getContentSize() / 2);
    listViewH->setMagneticType(ListView::MagneticType::CENTER);
    bg->addChild(listViewH);
    
    int idx = 0;
    for(int h = 0; h < CELL_HEIGHT; h++)
    {
        auto listViewW = ListView::create();
        listViewW->setDirection(ScrollView::Direction::HORIZONTAL);
        listViewW->setBounceEnabled(true);
        listViewW->setBackGroundImageScale9Enabled(true);
        listViewW->setContentSize(Size(bg->getContentSize().width, bg->getContentSize().height * 0.05f));
        listViewW->setScrollBarPositionFromCorner(Vec2(7, 7));
        listViewW->setItemsMargin(0);
        listViewW->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        listViewW->setMagneticType(ListView::MagneticType::CENTER);
        
        auto node = Widget::create();
        node->setContentSize(Size(bg->getContentSize().width, bg->getContentSize().height * 0.03f));
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        auto nodeW = node->getContentSize().width;
        auto nodeH = node->getContentSize().height;
        for(int w = 0; w < CELL_WIDTH; w++)
        {
            auto button = Button::create(m_TextureList.at(0), "", "", Widget::TextureResType::PLIST);
            button->addClickEventListener([=](Ref* sender){
                this->CreateBullet(static_cast<Node*>(sender));
            });
            button->setPosition(Vec2(((nodeW / CELL_WIDTH) * w), nodeH * 0.5f));
            button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            node->addChild(button);
            
            m_Cell[idx++] = button;
        }
        
        listViewH->pushBackCustomItem(node);
    }
    
    
    /*auto patternBack = LayerColor::create(COLOR::BRIGHTGRAY_ALPHA, bg->getContentSize().width * 0.9f, bg->getContentSize().height * 0.65f);
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
	}*/

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
		m_SelectButton->runAction(FadeIn::create(0.5f));

		bg->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.62f))));
	});

	this->setCloseAnimation([=](Node* sender){
		workShopLabel->runAction(FadeTo::create(0.5f, 0));
		btnExit->runAction(FadeTo::create(0.5f, 0));
		m_SelectButton->runAction(FadeTo::create(0.5f, 0));

		bg->runAction(EaseSineIn::create(MoveTo::create(0.4f, Vec2(visibleSize.width * 0.5f, visibleSize.height * 1.5f))));
	});

	return true;
}

void CPatternTestPopup::BulletSelect(Node* sender)
{
	m_CurrentBullet++;
	m_CurrentBullet = m_CurrentBullet % m_BulletTypeList.size();
	m_SelectButton->loadTextureNormal(m_TextureList.at(m_CurrentBullet), Widget::TextureResType::PLIST);
}

void CPatternTestPopup::CreateBullet(cocos2d::Node* sender)
{
    auto button = static_cast<Button*>(sender);
    auto iter = std::find(std::begin(m_Cell), std::end(m_Cell), button);
    auto index = std::distance(std::begin(m_Cell), iter);
    
    int height = (int)((int)index / CELL_WIDTH);
    int width =  (int)((int)index % CELL_WIDTH);
    
    (m_Pattern[height])[width] = m_BulletTypeList[m_CurrentBullet];
    
    //버튼 이미지 변경
    button->loadTextureNormal(m_TextureList.at(m_CurrentBullet), Widget::TextureResType::PLIST);
}

void CPatternTestPopup::End(Node* sender)
{

    if (m_TextFieldWidth->getText().size()){// && m_TextFieldPattern->getText().size()){
		
//		auto strVec = StringUtility::split(m_TextFieldPattern->getText().c_str(), ',');
//		auto lineVec = StringUtility::split(strVec.at(0).c_str(), '"');
		std::string patternString = "";

		sBULLET_PATTERN pattern;

		pattern._index = 10000;
		pattern._patternName = "test_Pattern";
		pattern._widthPadding = atol(m_TextFieldWidth->getText().c_str());
		pattern._height = CELL_HEIGHT;
		pattern._width = CELL_WIDTH;
        
        for (int height = 0; height < pattern._height; height++)
		{
			for (int width = 0; width < pattern._width; width++)
			{
				pattern._pattern[(pattern._width * height) + width] = (m_Pattern[height])[width];
			}
		}

		CBulletPatternDataManager::Instance()->setTestPattern(pattern);
	}
    
	this->popupClose();
}