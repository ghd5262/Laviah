#include "../Common/HSHUtility.h"

class CGoogleCloudManager{
public:
    static *CGoogleCloudManager Instance();
    
    void GoogleCloudDataSave(std::string key, std::string value);
    void GoogleCloudDataLoad(std::string key, std::string value);
    
    
private:
    CGoogleCloudManager();
    ~CGoogleCloudManager();
};