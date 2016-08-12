#include "../Popup.h"

class CMyButton;
class CGoogleCloudDataInfoPopup : public CSpecificPopupBase
{
public:
    static CGoogleCloudDataInfoPopup* create(std::string dataKey);
    
protected:
    virtual bool initVariable() override;
    
private:
    void Save();
    void End();
    void Select(cocos2d::Ref* dp);
    
    CGoogleCloudDataInfoPopup(std::string dataKey)
    : m_btnEnd(nullptr)
    , m_BG(nullptr)
    , m_UserDataKey(dataKey){}
    virtual ~CGoogleCloudDataInfoPopup(){};
    
private:
    CMyButton* m_btnEnd;
    LayerColor* m_BG;
    std::string m_UserDataKey;
};