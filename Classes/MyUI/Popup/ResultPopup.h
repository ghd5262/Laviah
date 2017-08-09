#pragma once
#include "../Popup.h"
#include <vector>

class CMyButton;
class CUserCoinButton;
class CResultPopup : public CPopup
{
public:
    static CResultPopup* create();
    
protected:
    virtual bool init() override;
    
private:
    void reset();
    void home();
    void end();
    void getCoinFromVideo(Node* sender);
    void getNewCostume(Node* sender);
    void getFreeReward();
    void share();
    void exit();
    
    void createRewardPopup(std::string key, int value);
    cocos2d::Node* createIconLayer(std::string iconName,
                                   std::string text);
    void createScoreLayer(std::string iconName,
                          std::string text,
                          int value);
    void createBonusScoreLayer(std::string iconName,
                               std::string text,
                               int value,
                               int bonus);
    void createTotalScoreLayer(int value);
    void createRankingLayer();
    void createLevelLayer();
    void createChangeLabelAction(cocos2d::Label* label,
                                 std::string text1,
                                 std::string text2);
    void createButtonLayer(std::function<void(Node*)> &callback,
                           std::string iconName,
                           std::string text,
                           Color3B color);
    void userDataUpdate();
    
    CResultPopup()
    : m_BG(nullptr)
    , m_GoalPopupOpen(false)
    , m_Ended(false){};
    virtual ~CResultPopup(){};
    
private:
    std::vector<Node*> m_ScoreLayerList;
    cocos2d::LayerColor* m_BG;
    bool m_GoalPopupOpen;
    bool m_Ended;
};
