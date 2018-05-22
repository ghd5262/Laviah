#pragma once
#include "cocos2d.h"
#include "network/HttpClient.h"

#define DEBUGING 0
#define TEST_BUILD 0

using namespace cocos2d;
using namespace cocos2d::network;

struct DOWNLOAD_FILE{
    std::string _fileName;
    std::string _url;
    std::string _localPath;
    int _fileVersion;
    bool _isCompressed;
    
    DOWNLOAD_FILE()
    : _fileName("")
    , _url("")
    , _localPath("")
    , _fileVersion(0)
    , _isCompressed(false){}
};

typedef std::vector<DOWNLOAD_FILE> DOWNLOAD_LIST;

struct PACKAGE_DATA{
    int _packageVersion;
    int _downloadFileCount;
    std::string _minBuildVersionAndroid;
    std::string _minBuildVersionIos;
    std::string _appLinkAndroid;
    std::string _appLinkIos;
    std::string _facebookPageLink;
    std::string _twitterPageLink;
    std::string _instagramPageLink;
    DOWNLOAD_LIST _fileInfoList;
    
    PACKAGE_DATA()
    : _packageVersion(0)
    , _downloadFileCount(0)
    , _minBuildVersionAndroid("1.0.0")
    , _minBuildVersionIos("1.0")
    , _appLinkAndroid("")
    , _appLinkIos("")
    , _facebookPageLink("")
    , _twitterPageLink("")
    , _instagramPageLink(""){};
};

typedef std::function<void(void)> VOID_LISTENER;
typedef std::function<void(int, int)> PROGRESS_LISTENER;
class CDownloadManager {

public:
    static CDownloadManager* Instance();
    void DownloadStart();
    
    CC_SYNTHESIZE(VOID_LISTENER, m_DownloadSucceedListener, DownloadSucceedListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_DownloadFailedListener,  DownloadFailedListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_RequireNextVersion,      RequireNextVersion);
    CC_SYNTHESIZE(PROGRESS_LISTENER, m_FileDownloadProgress,    FileDownloadProgress);
    CC_SYNTHESIZE(PROGRESS_LISTENER, m_FileDecompressProgress,  FileDecompressProgress);
    
    std::string getAppUrl();
    std::string getFacebookPageLink();
    std::string getTwitterPageLink();
    std::string getInstagramPageLink();
    static void IsNetworkConnected(std::function<void(bool)> listener);
    
private:
	CDownloadManager();
	virtual ~CDownloadManager();

private:
	void loadLocalPackageInfoFile();
	void downloadPackageInfoFile();
	void downloadCompletePackageInfoFile(HttpClient *client, HttpResponse *response);

	bool newVersionExist();
    bool isMinBuildVersion();
	void initDownloadList();

	void downloadNextFile();
	void progressDownloadPackageFile();
	void downloadCompletePackageFile(HttpClient *client, HttpResponse *response);

	void decompressPackageFile(int fileIdx);
	void progressDecompressPackageFile();

	void initPackageInfo(PACKAGE_DATA& packageInfo, std::string jsonData);
	void packageLoadFailed();
	void requestDownload(std::string url, ccHttpRequestCallback callback);

	bool savePackageInfoFile(const std::string path, const std::string &data);
	bool savePackageFile(const std::string path, const std::vector<char> *buf);
	bool saveDataToFile(const std::string mode, const std::string path, const std::string &data);
	bool decompress(const std::string &zip);
    void callVoidListener(VOID_LISTENER& listener);
    void callProgressListener(PROGRESS_LISTENER& listener, int cur, int max);
    void scheduleAfterDelay(std::function<void()> listener, float delay, std::string key);
private:
    static CDownloadManager* m_Instance;
	PACKAGE_DATA m_NewPackage;
	PACKAGE_DATA m_CurrentPackage;
	DOWNLOAD_LIST m_DownloadList;
	std::string m_ServerVersionFileData;
	std::string m_WritablePath;
	int m_DownloadMax;
	int m_DownloadCurrentIndex;
	int m_DecompressProgressMax;
	int m_DecompressProgressCurrentIndex;
	int m_CurrentGuage;
};
