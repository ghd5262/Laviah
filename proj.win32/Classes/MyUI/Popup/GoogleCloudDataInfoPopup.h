#include "../Popup.h"

class CMyButton;
class CGoogleCloudDataInfoPopup : public CPopup
{
public:
    static CGoogleCloudDataInfoPopup* create(std::string dataKey);
    
protected:
    virtual bool init() override;
    
private:
	void Save(Node* sender);
	void End(Node* sender);
    void Select(Node* dp);
    
    CGoogleCloudDataInfoPopup(std::string dataKey)
    : m_UserDataKey(dataKey){}
    virtual ~CGoogleCloudDataInfoPopup(){};
    
private:
    std::string m_UserDataKey;
};