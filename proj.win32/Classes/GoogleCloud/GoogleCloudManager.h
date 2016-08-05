#include "../Common/HSHUtility.h"
#include <map>
class CGoogleCloudManager{
public:
    static CGoogleCloudManager* Instance();
    
    void GoogleCloudDataSave(std::string key, std::string valueJson);
    void GoogleCloudDataLoad(std::string key, std::string valueJson);
    
	//getter & setter
	CC_SYNTHESIZE(bool, m_isConnected, IsConnected);

private:
    CGoogleCloudManager();
    ~CGoogleCloudManager();
};