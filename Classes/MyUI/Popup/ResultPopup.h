#pragma once
#include "../Popup.h"
#include <vector>

class CMyButton;
class CUserCoinButton;
class CResultPopup : public CPopup
{
public:
    static CResultPopup* create();
    CResultPopup* setIsStageEnd(bool isStageEnd);
    CPopup* show(Node* parent = nullptr, int zOrder = 0);
    
protected:
    virtual bool init() override;
    
private:
    void reset(Node* sender);
    void home();
    void end();
    void getCoinFromVideo(Node* sender);
    void getNewCostume(Node* sender);
    void getFreeReward();
    void openAchievementPopup();
    void share();
    void exit();
    void next();
    
    void createRewardPopup(std::string title, std::string key, int value);
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
    
    void createPercentageLayer(std::string iconName, std::string text,
                               int value, int max);
    void createCaptureBtn();
    void userDataUpdate();
    
    CResultPopup()
    : m_BG(nullptr)
    , m_GoalPopupOpen(false)
    , m_PictureBtn(nullptr)
    , m_IsStageEnd(false)
    , m_IsVideoFinished(false){};
    virtual ~CResultPopup();
    
private:
    std::vector<Node*> m_ScoreLayerList;
    cocos2d::LayerColor* m_BG;
    bool m_GoalPopupOpen;
    CMyButton* m_PictureBtn;
    bool m_IsStageEnd;
    bool m_IsVideoFinished;
};
