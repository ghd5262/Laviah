#include "../Common/HSHUtility.h"
#include <map>
class CGoogleCloudManager{
public:
    static CGoogleCloudManager* Instance();
    
    void GoogleCloudDataSave(std::string key, std::string valueJson);
    void GoogleCloudDataLoad(std::string key, std::string valueJson);
	void AddDataToAutoSaveList(std::string key, std::string valueJson);

	long long getLastSavedTime() const { return m_LastSavedTime; };
	void setLastSavedTime(long long unixGMT);
	//getter & setter
	CC_SYNTHESIZE(bool, m_isConnected, IsConnected);

private:
    CGoogleCloudManager();
    ~CGoogleCloudManager();
    
private:
    unsigned m_GoogleCloudDataLoadCount;
	long long m_LastSavedTime;
};