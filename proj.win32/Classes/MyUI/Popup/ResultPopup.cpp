#include "ResultPopup.h"
#include "../MyButton.h"
#include "../ScoreUI.h"
#include "../UIManager.h"
#include "../../Scene/GameScene.h"
#include "../../DataManager/UserDataManager.h"

CResultPopup* CResultPopup::create()
{
    CResultPopup *pRet = new(std::nothrow) CResultPopup();
    if (pRet)
    {
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CResultPopup::initVariable()
{
    try{
        m_BG = LayerColor::create(Color4B(255, 255, 255, 0), 1080.f, 1920.f);
        if(m_BG != nullptr){
            m_BG->ignoreAnchorPointForPosition(false);
            m_BG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->setPosition(Vec2::ZERO);
            m_Popup->addChild(m_BG);
        }
        
        // 총 점수 = 달린 총거리 + 별 + 코인 + (보너스타임횟수 * 10000) + (외계주민 * 10000) + (도전과제 * 10000)
        auto totalScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("TotalScore"));
        
        /* result label*/
        auto resultLabel = Label::createWithTTF("Result", "fonts/malgunbd.ttf", 80);
        if (resultLabel != nullptr)
        {
            resultLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            resultLabel->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.8f));
            resultLabel->setColor(g_labelColor2);
            m_BG->addChild(resultLabel);
            resultLabel->setOpacity(0);
        }
        
        /* moveDistance */
        auto moveDistanceBG = Sprite::create("resultPopup_2.png");
        if (moveDistanceBG != nullptr){
            moveDistanceBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            moveDistanceBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.5f));
            m_BG->addChild(moveDistanceBG);
            moveDistanceBG->setOpacity(0);
            moveDistanceBG->setCascadeOpacityEnabled(true);
            auto moveDistanceIcon = Sprite::create("runIcon.png");
            if (moveDistanceIcon != nullptr)
            {
                moveDistanceIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                moveDistanceIcon->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.1f, moveDistanceBG->getContentSize().height * 0.5f));
                moveDistanceBG->addChild(moveDistanceIcon);
                moveDistanceIcon->setColor(g_labelColor1);
            }
            
            auto moveDistanceLabel = Label::createWithTTF("Run", "fonts/malgunbd.ttf", 50);
            if (moveDistanceLabel != nullptr){
                moveDistanceLabel->setColor(g_labelColor1);
                moveDistanceLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                moveDistanceLabel->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.15f, moveDistanceBG->getContentSize().height * 0.5f));
                moveDistanceBG->addChild(moveDistanceLabel);
            }
            
            auto runScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("RunScoreUI"));
            totalScore->UpdateValue(runScore->getScoreValue());
            auto moveDistanceValueLabel = Label::createWithTTF(runScore->getValueString(), "fonts/malgunbd.ttf", 50);
            if (moveDistanceValueLabel != nullptr){
                moveDistanceValueLabel->setColor(g_labelColor1);
                moveDistanceValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                moveDistanceValueLabel->setPosition(Vec2(moveDistanceBG->getContentSize().width * 0.9f, moveDistanceBG->getContentSize().height * 0.5f));
                moveDistanceBG->addChild(moveDistanceValueLabel);
            }
        }
        
        
        /* starScore */
        auto starScoreBG = Sprite::create("resultPopup_2.png");
        if (starScoreBG != nullptr){
            starScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            starScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.4f));
            m_BG->addChild(starScoreBG);
            starScoreBG->setOpacity(0);
            starScoreBG->setCascadeOpacityEnabled(true);
            auto starScoreIcon = Sprite::create("starIcon.png");
            if (starScoreIcon != nullptr)
            {
                starScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                starScoreIcon->setPosition(Vec2(starScoreBG->getContentSize().width * 0.1f, starScoreBG->getContentSize().height * 0.5f));
                starScoreBG->addChild(starScoreIcon);
                starScoreIcon->setColor(g_labelColor1);
            }
            
            auto starScoreLabel = Label::createWithTTF("Star", "fonts/malgunbd.ttf", 50);
            if (starScoreLabel != nullptr){
                starScoreLabel->setColor(g_labelColor1);
                starScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                starScoreLabel->setPosition(Vec2(starScoreBG->getContentSize().width * 0.15f, starScoreBG->getContentSize().height * 0.5f));
                starScoreBG->addChild(starScoreLabel);
            }
            
            auto starScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
            totalScore->UpdateValue(starScore->getScoreValue());
            auto starScoreValueLabel = Label::createWithTTF(starScore->getValueString(), "fonts/malgunbd.ttf", 50);
            if (starScoreValueLabel != nullptr){
                starScoreValueLabel->setColor(g_labelColor1);
                starScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                starScoreValueLabel->setPosition(Vec2(starScoreBG->getContentSize().width * 0.9f, starScoreBG->getContentSize().height * 0.5f));
                starScoreBG->addChild(starScoreValueLabel);
            }
        }
        
        /* coin Score */
        auto coinScoreBG = Sprite::create("resultPopup_2.png");
        if (coinScoreBG != nullptr){
            coinScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            coinScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.3f));
            m_BG->addChild(coinScoreBG);
            coinScoreBG->setOpacity(0);
            coinScoreBG->setCascadeOpacityEnabled(true);
            auto coinScoreIcon = Sprite::create("coinIcon.png");
            if (coinScoreIcon != nullptr)
            {
                coinScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                coinScoreIcon->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.1f, coinScoreBG->getContentSize().height * 0.5f));
                coinScoreBG->addChild(coinScoreIcon);
                coinScoreIcon->setColor(g_labelColor1);
            }
            
            auto coinScoreLabel = Label::createWithTTF("Coin", "fonts/malgunbd.ttf", 50);
            if (coinScoreLabel != nullptr){
                coinScoreLabel->setColor(g_labelColor1);
                coinScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                coinScoreLabel->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.15f, coinScoreBG->getContentSize().height * 0.5f));
                coinScoreBG->addChild(coinScoreLabel);
            }
            
            auto coinScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("CoinScoreUI"));
            totalScore->UpdateValue(coinScore->getScoreValue());
            auto coinScoreValueLabel = Label::createWithTTF(coinScore->getValueString(), "fonts/malgunbd.ttf", 50);
            if (coinScoreValueLabel != nullptr){
                coinScoreValueLabel->setColor(g_labelColor1);
                coinScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                coinScoreValueLabel->setPosition(Vec2(coinScoreBG->getContentSize().width * 0.9f, coinScoreBG->getContentSize().height * 0.5f));
                coinScoreBG->addChild(coinScoreValueLabel);
            }
        }
        
        /* bonusTime Count */
        auto bonusTimeBG = Sprite::create("resultPopup_2.png");
        if (bonusTimeBG != nullptr){
            bonusTimeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bonusTimeBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.2f));
            m_BG->addChild(bonusTimeBG);
            bonusTimeBG->setOpacity(0);
            bonusTimeBG->setCascadeOpacityEnabled(true);
            auto bonusTimeIcon = Sprite::create("bonustimeIcon.png");
            if (bonusTimeIcon != nullptr)
            {
                bonusTimeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                bonusTimeIcon->setPosition(Vec2(bonusTimeBG->getContentSize().width * 0.1f, bonusTimeBG->getContentSize().height * 0.5f));
                bonusTimeBG->addChild(bonusTimeIcon);
                bonusTimeIcon->setColor(g_labelColor1);
            }
            
            auto bonusTimeLabel = Label::createWithTTF("BonusTime", "fonts/malgunbd.ttf", 50);
            if (bonusTimeLabel != nullptr){
                bonusTimeLabel->setColor(g_labelColor1);
                bonusTimeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                bonusTimeLabel->setPosition(Vec2(bonusTimeBG->getContentSize().width * 0.15f, bonusTimeBG->getContentSize().height * 0.5f));
                bonusTimeBG->addChild(bonusTimeLabel);
            }
            
            auto bonusTimeCount = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("BonusTimeCount"));
            totalScore->UpdateValue(bonusTimeCount->getScoreValue() * 10000);
            auto bonusTimeValueLabel = Label::createWithTTF(StringUtils::format("%d", bonusTimeCount->getScoreValue()), "fonts/malgunbd.ttf", 50);
            if (bonusTimeValueLabel != nullptr){
                bonusTimeValueLabel->setColor(g_labelColor1);
                bonusTimeValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                bonusTimeValueLabel->setPosition(Vec2(bonusTimeBG->getContentSize().width * 0.9f, bonusTimeBG->getContentSize().height * 0.5f));
                bonusTimeBG->addChild(bonusTimeValueLabel);
            }
            auto multipleScoreLabel = Label::createWithTTF(StringUtils::format("%d x ", 10000), "fonts/malgunbd.ttf", 25);
            if (multipleScoreLabel != nullptr){
                multipleScoreLabel->setColor(g_labelColor1);
                multipleScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                multipleScoreLabel->setPosition(
                                                Vec2(bonusTimeValueLabel->getPosition().x - bonusTimeValueLabel->getContentSize().width,
                                                     bonusTimeBG->getContentSize().height * 0.4f));
                bonusTimeBG->addChild(multipleScoreLabel);
            }
        }
        
        /* alien get count */
        auto alienBG = Sprite::create("resultPopup_2.png");
        if (alienBG != nullptr){
            alienBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            alienBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.1f));
            m_BG->addChild(alienBG);
            alienBG->setOpacity(0);
            alienBG->setCascadeOpacityEnabled(true);
            auto alienIcon = Sprite::create("alienIcon.png");
            if (alienIcon != nullptr)
            {
                alienIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                alienIcon->setPosition(Vec2(alienBG->getContentSize().width * 0.1f, alienBG->getContentSize().height * 0.5f));
                alienBG->addChild(alienIcon);
                alienIcon->setColor(g_labelColor1);
            }
            
            auto alienLabel = Label::createWithTTF("AlienGet", "fonts/malgunbd.ttf", 50);
            if (alienLabel != nullptr){
                alienLabel->setColor(g_labelColor1);
                alienLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                alienLabel->setPosition(Vec2(alienBG->getContentSize().width * 0.15f, alienBG->getContentSize().height * 0.5f));
                alienBG->addChild(alienLabel);
            }
            
            auto alienGetCount = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("AlienGetCount"));
            totalScore->UpdateValue(alienGetCount->getScoreValue() * 10000);
            auto alienValueLabel = Label::createWithTTF(StringUtils::format("%d", alienGetCount->getScoreValue()), "fonts/malgunbd.ttf", 50);
            if (alienValueLabel != nullptr){
                alienValueLabel->setColor(g_labelColor1);
                alienValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                alienValueLabel->setPosition(Vec2(alienBG->getContentSize().width * 0.9f, alienBG->getContentSize().height * 0.5f));
                alienBG->addChild(alienValueLabel);
            }
            auto multipleScoreLabel = Label::createWithTTF(StringUtils::format("%d x ", 10000), "fonts/malgunbd.ttf", 25);
            if (multipleScoreLabel != nullptr){
                multipleScoreLabel->setColor(g_labelColor1);
                multipleScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                multipleScoreLabel->setPosition(
                                                Vec2(alienValueLabel->getPosition().x - alienValueLabel->getContentSize().width,
                                                     alienBG->getContentSize().height * 0.4f));
                alienBG->addChild(multipleScoreLabel);
            }
        }
        
        /* challenge clear count */
        auto challengeBG = Sprite::create("resultPopup_2.png");
        if (challengeBG != nullptr){
            challengeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            challengeBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * 0.0f));
            m_BG->addChild(challengeBG);
            challengeBG->setOpacity(0);
            challengeBG->setCascadeOpacityEnabled(true);
            auto challengeIcon = Sprite::create("challengeIcon.png");
            if (challengeIcon != nullptr)
            {
                challengeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                challengeIcon->setPosition(Vec2(challengeBG->getContentSize().width * 0.1f, challengeBG->getContentSize().height * 0.5f));
                challengeBG->addChild(challengeIcon);
                challengeIcon->setColor(g_labelColor1);
            }
            
            auto challengeLabel = Label::createWithTTF("ChallengeClear", "fonts/malgunbd.ttf", 50);
            if (challengeLabel != nullptr){
                challengeLabel->setColor(g_labelColor1);
                challengeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                challengeLabel->setPosition(Vec2(challengeBG->getContentSize().width * 0.15f, challengeBG->getContentSize().height * 0.5f));
                challengeBG->addChild(challengeLabel);
            }
            
            auto challengeClearCount = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("ChallengeClearCount"));
            totalScore->UpdateValue(challengeClearCount->getScoreValue() * 10000);
            auto challengeValueLabel = Label::createWithTTF(StringUtils::format("%d", challengeClearCount->getScoreValue()), "fonts/malgunbd.ttf", 50);
            if (challengeValueLabel != nullptr){
                challengeValueLabel->setColor(g_labelColor1);
                challengeValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                challengeValueLabel->setPosition(Vec2(challengeBG->getContentSize().width * 0.9f, challengeBG->getContentSize().height * 0.5f));
                challengeBG->addChild(challengeValueLabel);
            }
            auto multipleScoreLabel = Label::createWithTTF(StringUtils::format("%d x ", 10000), "fonts/malgunbd.ttf", 25);
            if (multipleScoreLabel != nullptr){
                multipleScoreLabel->setColor(g_labelColor1);
                multipleScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                multipleScoreLabel->setPosition(
                                                Vec2(challengeValueLabel->getPosition().x - challengeValueLabel->getContentSize().width,
                                                     challengeBG->getContentSize().height * 0.4f));
                challengeBG->addChild(multipleScoreLabel);
            }
        }
        
        /* total score */
        auto totalScoreBG = Sprite::create("resultPopup_1.png");
        if (totalScoreBG != nullptr){
            totalScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            totalScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.1f));
            m_BG->addChild(totalScoreBG);
            totalScoreBG->setOpacity(0);
            totalScoreBG->setCascadeOpacityEnabled(true);
            
            std::string bestScoreStr = "Total Score";
            
            // total score가 best score면 저장한다.
            if(totalScore->getScoreValue() > CUserDataManager::Instance()->getUserDataBestTotalScore()){
                bestScoreStr = "Best Score";
                CUserDataManager::Instance()->setUserDataBestTotalScore(totalScore->getScoreValue());
            }
            
            auto totalScoreLabel = Label::createWithTTF(bestScoreStr.c_str(), "fonts/malgunbd.ttf", 60);
            if (totalScoreLabel != nullptr){
                totalScoreLabel->setColor(g_labelColor2);
                totalScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                totalScoreLabel->setPosition(Vec2(totalScoreBG->getContentSize().width * 0.08f, totalScoreBG->getContentSize().height * 0.5f));
                totalScoreBG->addChild(totalScoreLabel);
            }
            
            auto totalScoreValueLabel = Label::createWithTTF(totalScore->getValueString(), "fonts/malgunbd.ttf", 60);
            if (totalScoreValueLabel != nullptr){
                totalScoreValueLabel->setColor(g_labelColor2);
                totalScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                totalScoreValueLabel->setPosition(Vec2(totalScoreBG->getContentSize().width * 0.9f, totalScoreBG->getContentSize().height * 0.5f));
                totalScoreBG->addChild(totalScoreValueLabel);
            }
        }
        
        /* best score */
        auto bestScoreBG = Sprite::create("resultPopup_2.png");
        if (bestScoreBG != nullptr){
            bestScoreBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bestScoreBG->setPosition(Vec2(m_BG->getContentSize().width * 0.5f, m_BG->getContentSize().height * -0.2f));
            m_BG->addChild(bestScoreBG);
            bestScoreBG->setOpacity(0);
            bestScoreBG->setCascadeOpacityEnabled(true);
            auto bestScoreIcon = Sprite::create("challengeIcon.png");
            if (bestScoreIcon != nullptr)
            {
                bestScoreIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                bestScoreIcon->setPosition(Vec2(bestScoreBG->getContentSize().width * 0.1f, bestScoreBG->getContentSize().height * 0.5f));
                bestScoreBG->addChild(bestScoreIcon);
                bestScoreIcon->setColor(g_labelColor1);
            }
            
            auto bestScoreLabel = Label::createWithTTF("Best Score", "fonts/malgunbd.ttf", 50);
            if (bestScoreLabel != nullptr){
                bestScoreLabel->setColor(g_labelColor1);
                bestScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                bestScoreLabel->setPosition(Vec2(bestScoreBG->getContentSize().width * 0.15f, bestScoreBG->getContentSize().height * 0.5f));
                bestScoreBG->addChild(bestScoreLabel);
            }
            
            auto bestScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("BestScore"));
            bestScore->UpdateValue(CUserDataManager::Instance()->getUserDataBestTotalScore());
            auto bestScoreValueLabel = Label::createWithTTF(StringUtils::format("%d", bestScore->getScoreValue()), "fonts/malgunbd.ttf", 50);
            if (bestScoreValueLabel != nullptr){
                bestScoreValueLabel->setColor(g_labelColor1);
                bestScoreValueLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                bestScoreValueLabel->setPosition(Vec2(bestScoreBG->getContentSize().width * 0.9f, bestScoreBG->getContentSize().height * 0.5f));
                bestScoreBG->addChild(bestScoreValueLabel);
            }
        }
        
        m_btnHome = CMyButton::create("homeIcon.png",
                                      END,
                                      std::bind(&CResultPopup::GoHome, this),
                                      EFFECT_ALPHA);
        
        if (m_btnHome != nullptr)
        {
            m_btnHome->setPosition(Vec2(m_BG->getContentSize().width * 0.08f,
                                        m_BG->getContentSize().height * 0.05f));
            m_btnHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->addChild(m_btnHome);
            m_btnHome->setCascadeOpacityEnabled(true);
            m_btnHome->setOpacity(0);
        }
        
        m_btnReset = CMyButton::create("resetIcon.png",
                                       END,
                                       std::bind(&CResultPopup::Reset, this),
                                       EFFECT_ALPHA);
        
        if (m_btnReset != nullptr)
        {
            m_btnReset->setPosition(Vec2(m_BG->getContentSize().width * 0.92f,
                                         m_BG->getContentSize().height * 0.05f));
            m_btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BG->addChild(m_btnReset);
            m_btnReset->setCascadeOpacityEnabled(true);
            m_btnReset->setOpacity(0);
        }
        
        m_Popup->setPopupOpenEffectFunc([=](CPopup* pausePopup){
            auto winSize = Director::getInstance()->getWinSize();
            
            moveDistanceBG->runAction(
                                      Spawn::createWithTwoActions(
                                                                  EaseExponentialOut::create(
                                                                                             MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                       m_BG->getContentSize().height * 0.7f))),
                                                                  FadeIn::create(1.f)));
            starScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseExponentialOut::create(
                                                                                          MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                    m_BG->getContentSize().height * 0.65f))),
                                                               FadeIn::create(1.f)));
            coinScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseExponentialOut::create(
                                                                                          MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                    m_BG->getContentSize().height * 0.6f))),
                                                               FadeIn::create(1.f)));
            bonusTimeBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseExponentialOut::create(
                                                                                          MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                    m_BG->getContentSize().height * 0.55f))),
                                                               FadeIn::create(1.f)));
            alienBG->runAction(
                               Spawn::createWithTwoActions(
                                                           EaseExponentialOut::create(
                                                                                      MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                m_BG->getContentSize().height * 0.5f))),
                                                           FadeIn::create(1.f)));
            challengeBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseExponentialOut::create(
                                                                                          MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                    m_BG->getContentSize().height * 0.45f))),
                                                               FadeIn::create(1.f)));
            totalScoreBG->runAction(
                                    Spawn::createWithTwoActions(
                                                                EaseExponentialOut::create(
                                                                                           MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                     m_BG->getContentSize().height * 0.4f))),
                                                                FadeIn::create(1.f)));
            bestScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseExponentialOut::create(
                                                                                          MoveTo::create(1.3f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                    m_BG->getContentSize().height * 0.35f))),
                                                               FadeIn::create(1.f)));
            resultLabel->runAction(FadeIn::create(0.5f));
            
            m_btnHome->runAction(FadeIn::create(0.5f));
            
            m_btnReset->runAction(FadeIn::create(0.5f));
        });
        
        m_Popup->setPopupCloseEffectFunc([=](CPopup* pausePopup){
            moveDistanceBG->runAction(
                                      Spawn::createWithTwoActions(
                                                                  EaseSineIn::create(
                                                                                     MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                                m_BG->getContentSize().height * 0.5f))),
                                                                  FadeTo::create(0.2f, 0)));
            starScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseSineIn::create(
                                                                                  MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                             m_BG->getContentSize().height * 0.4f))),
                                                               FadeTo::create(0.2f, 0)));
            coinScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseSineIn::create(
                                                                                  MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                             m_BG->getContentSize().height * 0.3f))),
                                                               FadeTo::create(0.2f, 0)));
            bonusTimeBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseSineIn::create(
                                                                                  MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                             m_BG->getContentSize().height * 0.2f))),
                                                               FadeTo::create(0.2f, 0)));
            alienBG->runAction(
                               Spawn::createWithTwoActions(
                                                           EaseSineIn::create(
                                                                              MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                         m_BG->getContentSize().height * 0.1f))),
                                                           FadeTo::create(0.2f, 0)));
            challengeBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseSineIn::create(
                                                                                  MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                             m_BG->getContentSize().height * 0.0f))),
                                                               FadeTo::create(0.2f, 0)));
            totalScoreBG->runAction(
                                    Spawn::createWithTwoActions(
                                                                EaseSineIn::create(
                                                                                   MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                              m_BG->getContentSize().height * -0.1f))),
                                                                FadeTo::create(0.2f, 0)));
            bestScoreBG->runAction(
                                   Spawn::createWithTwoActions(
                                                               EaseSineIn::create(
                                                                                  MoveTo::create(0.35f, Vec2(m_BG->getContentSize().width * 0.5f,
                                                                                                             m_BG->getContentSize().height * -0.2f))),
                                                               FadeTo::create(0.2f, 0)));
            resultLabel->runAction(FadeTo::create(0.5f, 0));
            
            m_btnHome->runAction(FadeTo::create(0.5f, 0));
            
            m_btnReset->runAction(FadeTo::create(0.5f, 0));
            
            m_Popup->scheduleOnce([this](float delta){
                CSpecificPopupBase::PopupRelease();
            }, 0.35f, "PausePopupClose");
        });
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CResultPopup::Reset(){
    CCLOG("format popup Replay");
    CGameScene::getGameScene()->resetGameScene();
}

void CResultPopup::GoHome(){
    CCLOG("format popup GoHome");
    CGameScene::getGameScene()->backToMenuScene();
    CSpecificPopupBase::PopupClose();
}