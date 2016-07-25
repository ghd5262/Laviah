#include "../Popup.h"

class CMyButton;
class CGoogleCloudDataInfoPopup : public CSpecificPopupBase
{
public:
    static CGoogleCloudDataInfoPopup* create();
    
protected:
    virtual bool initVariable() override;
    
private:
    void Save();
    void End();
    void Select(cocos2d::Ref* dp);
    
    CGoogleCloudDataInfoPopup()
    : m_btnEnd(nullptr)
    , m_BG(nullptr){}
    virtual ~CGoogleCloudDataInfoPopup(){};
    
private:
    CMyButton* m_btnEnd;
    LayerColor* m_BG;
};