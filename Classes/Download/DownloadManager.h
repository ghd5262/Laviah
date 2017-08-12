#pragma once
#include "cocos2d.h"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace cocos2d::network;
class CLevelProgressBar;
class CDownloadManager : public cocos2d::Node {
	struct sDOWNLOADFILE{
		std::string _fileName;
		std::string _url;
		std::string _localPath;
		int _fileVersion;
		bool _isCompressed;

		sDOWNLOADFILE()
			: _fileName("")
			, _url("")
			, _localPath("")
			, _fileVersion(0)
			, _isCompressed(false){}
	};

	typedef std::vector<sDOWNLOADFILE> DOWNLOAD_LIST;

	struct sPACKAGE_INFO{
		int _packageVersion;
		int _downloadFileCount;
        std::string _minBuildVersionAndroid;
        std::string _minBuildVersionIos;
        std::string _appLinkAndroid;
        std::string _appLinkIos;
		DOWNLOAD_LIST _fileInfoList;

		sPACKAGE_INFO()
        : _packageVersion(0)
        , _downloadFileCount(0)
        , _minBuildVersionAndroid("1.0.0")
        , _minBuildVersionIos("1.0")
        , _appLinkAndroid("")
        , _appLinkIos(""){};
	};

public:
	CREATE_FUNC(CDownloadManager);
    
    typedef std::function<void(void)> VOID_LISTENER;
    CC_SYNTHESIZE(VOID_LISTENER, m_DownloadSucceedListener, DownloadSucceedListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_DownloadFailedListener,  DownloadFailedListener);
    CC_SYNTHESIZE(VOID_LISTENER, m_RequireNextVersion,      RequireNextVersion);

    std::string getAppUrl();
    static void IsNetworkConnected(std::function<void(bool)> listener);
    
protected:
	virtual bool init() override;
	virtual void onExit() override;

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
	void progressDownloadPackageFile(cocos2d::Ref *object);
	void downloadCompletePackageFile(HttpClient *client, HttpResponse *response);

	void decompressPackageFile(int fileIdx);
	void progressDecompressPackageFile(cocos2d::Ref *object);

	void initPackageInfo(sPACKAGE_INFO& packageInfo, std::string jsonData);
	void packageLoadFailed();
	void requestDownload(std::string url, ccHttpRequestCallback callback);
	void sendNotice(std::string key, cocos2d::Ref* sender = nullptr);
	void updateProgressBar();

	bool savePackageInfoFile(const std::string path, const std::string &data);
	bool savePackageFile(const std::string path, const std::vector<char> *buf);
	bool saveDataToFile(const std::string mode, const std::string path, const std::string &data);
	bool decompress(const std::string &zip);
    void callVoidListener(VOID_LISTENER& listener);

private:
	CLevelProgressBar* m_DownloadGauge;
	sPACKAGE_INFO m_NewPackage;
	sPACKAGE_INFO m_CurrentPackage;
	DOWNLOAD_LIST m_DownloadList;
	std::string m_ServerVersionFileData;
	std::string m_WritablePath;
	int m_DownloadMax;
	int m_DownloadCurrentIndex;
	int m_DecompressProgressMax;
	int m_DecompressProgressCurrentIndex;
	int m_CurrentGuage;
};
