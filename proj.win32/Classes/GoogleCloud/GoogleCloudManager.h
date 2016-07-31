#include "../Common/HSHUtility.h"
#include <map>
class CGoogleCloudManager{
public:
    static CGoogleCloudManager* Instance();
    
    void GoogleCloudDataSave(std::string key, std::string value);
    void GoogleCloudDataLoad(std::string key, std::string value);
    
	//getter & setter
	CC_SYNTHESIZE(bool, m_isConnected, IsConnected);

private:
    CGoogleCloudManager();
    ~CGoogleCloudManager();

private:
	typedef std::function<void(std::string)> FUNC;
	std::map<std::string, FUNC> m_DataLoadFuncList;
};