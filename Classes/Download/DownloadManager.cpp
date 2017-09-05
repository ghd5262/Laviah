#include "DownloadManager.h"
#include "json/document.h"
#include "unzip/unzip.h"
#include "network/HttpRequest.h"
#include "../Common/StringUtility.h"
#include "../json/json.h"
#include "../MyUI/LevelProgressBar.h"
#include "../MyUI/Popup.h"
#include <regex>

using namespace std;

#define	BUFFER_SIZE		8000 // 5MB
#define	MAX_FILENAME	512

namespace DOWNLOAD_DEFINE {
	const static std::string PACKAGE_FILE_PATH		= "update/package.json";
	const static std::string PACKAGE_INFO_FILE_PATH = std::string("http://www.hshgames.com/game/project_s/") + PACKAGE_FILE_PATH;
}
using namespace DOWNLOAD_DEFINE;

static string basename(const string &path) {
    size_t found = path.find_last_of("/\\");
    
    if (string::npos != found)
        return path.substr(0, found);
    else
        return path;
}

CDownloadManager::CDownloadManager()
: m_ServerVersionFileData("")
, m_DownloadCurrentIndex(0)
, m_DownloadMax(0)
, m_DecompressProgressCurrentIndex(0)
, m_DecompressProgressMax(0)
, m_DownloadSucceedListener(nullptr)
, m_DownloadFailedListener(nullptr)
, m_RequireNextVersion(nullptr)
, m_FileDownloadProgress(nullptr)
, m_FileDecompressProgress(nullptr){}

CDownloadManager::~CDownloadManager(){}

CDownloadManager* CDownloadManager::Instance()
{
    static CDownloadManager instance;
    return &instance;
}

void CDownloadManager::DownloadStart(){
    m_WritablePath = FileUtils::getInstance()->getWritablePath();
    this->downloadPackageInfoFile();
}

std::string CDownloadManager::getAppUrl()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return m_NewPackage._appLinkIos;
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return m_NewPackage._appLinkAndroid;
#endif
}

std::string CDownloadManager::getFacebookPageLink()
{
    return m_NewPackage._facebookPageLink;
}

void CDownloadManager::IsNetworkConnected(std::function<void(bool)> listener)
{
    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl("https://www.google.com");
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback([=](network::HttpClient* client,
                                     network::HttpResponse* response) {
        auto disConnected = (!response || !response->isSucceed());
        listener(!disConnected);
    });
    network::HttpClient::getInstance()->send(request);
    request->release();
}

void CDownloadManager::downloadPackageInfoFile() {
	CCLOG("%s", __FUNCTION__);

	auto versionFilePath = m_WritablePath + "update/";
    if (FileUtils::getInstance()->createDirectory(versionFilePath))
    {
        this->requestDownload(PACKAGE_INFO_FILE_PATH, [=](HttpClient *client, HttpResponse *response){
            this->downloadCompletePackageInfoFile(client, response);
        });
    }
}

// 요청한 리소스 버전 파일 다운로드가 완료되면 호출된다.
void CDownloadManager::downloadCompletePackageInfoFile(HttpClient *client, HttpResponse *response) {
	CCLOG("%s", __FUNCTION__);

	// 리소스 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL || !response->isSucceed() || response->getResponseCode() != 200) {
        this->packageLoadFailed();
        return;
    }

	auto data = response->getResponseData();

	m_ServerVersionFileData = string(data->begin(), data->end());
	this->initPackageInfo(m_NewPackage, m_ServerVersionFileData);
	this->loadLocalPackageInfoFile();

    if (this->newVersionExist()) {
        if(this->isMinBuildVersion())
            this->initDownloadList();
        else{
            this->callVoidListener(m_RequireNextVersion);
            return;
        }
    }
	// 업데이트 패키지 파일 다운로드를 시작한다.
	this->downloadNextFile();
}

// 업데이트 서버에서 패키지 파일 다운로드를 요청한다.
void CDownloadManager::downloadNextFile() {
	CCLOG("%s", __FUNCTION__);

	// 모든 패키지 파일 다운로드가 완료되었을 경우
	if (m_DownloadMax <= m_DownloadCurrentIndex) {

		// 다운로드한 버전 파일을 저장한다.
		auto versionFilePath = m_WritablePath + PACKAGE_FILE_PATH;
		this->savePackageInfoFile(versionFilePath, m_ServerVersionFileData);
        this->callVoidListener(m_DownloadSucceedListener);
        m_FileDownloadProgress   = nullptr;
        m_FileDecompressProgress = nullptr;
        
		return;
    }

	// 패키지 파일의 다운로드를 요청한다.
	auto url = m_DownloadList.at(m_DownloadCurrentIndex)._url;
    this->requestDownload(url, [=](HttpClient *client, HttpResponse *response){
        this->downloadCompletePackageFile(client, response);
        this->progressDownloadPackageFile();
    });
}

// 패키지 파일 다운로드가 진행되는 동안 호출된다.
void CDownloadManager::progressDownloadPackageFile() {
    this->callProgressListener(m_FileDownloadProgress, m_DownloadCurrentIndex, m_DownloadMax);
}

// 패키지 파일 다운로드가 완료되면 호출된다.
void CDownloadManager::downloadCompletePackageFile(HttpClient *client, HttpResponse *response) {
	CCLOG("%s", __FUNCTION__);

	// 애셋 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL || !response->isSucceed()) {
        this->packageLoadFailed();
        return;
    }

	if (m_DownloadList.size() > m_DownloadCurrentIndex){
		auto downloadedFile = m_DownloadList.at(m_DownloadCurrentIndex);
        
        // 다운로드한 업데이트 패키지 파일을 저장한다.
        string packageFilePath = m_WritablePath + downloadedFile._localPath;
        string fullpath = packageFilePath + downloadedFile._fileName;
        auto buf = response->getResponseData();
        
        this->savePackageFile(fullpath, buf);
        
        if(downloadedFile._isCompressed){
            // 패키지 파일의 압축을 푼다.
            int currentDownloadIdx = m_DownloadCurrentIndex;
            this->scheduleAfterDelay([=]() {
                this->decompressPackageFile(currentDownloadIdx);
            }, 1.f, "DecompressPackageFile");
        }
        else{
            // 나머지 업데이트 패키지 파일 다운로드를 시작한다.
            this->scheduleAfterDelay([=](){
                this->downloadNextFile();
            }, 1.f, "DownloadNextFile");
        }
        
        ++m_DownloadCurrentIndex;
    }
}

void CDownloadManager::decompressPackageFile(int fileIdx) {
	CCLOG("%s", __FUNCTION__);

	if (m_DownloadList.size() > fileIdx){
		auto downloadedFile = m_DownloadList.at(fileIdx);
        
        std::thread thread = std::thread([=]() {
            string packageFilePath = m_WritablePath + downloadedFile._localPath + downloadedFile._fileName;
            
            // 패키지 파일의 압축을 푸는데 실패하면 로고 화면으로 전환한다.
			if (!decompress(packageFilePath)) {
                this->packageLoadFailed();
                return;
            }
            
            // 압축을 푼 패키지 파일을 삭제한다.
            FileUtils::getInstance()->removeFile(packageFilePath);
        });
        
        thread.detach();
    }
}

void CDownloadManager::progressDecompressPackageFile() {

    this->callProgressListener(m_FileDecompressProgress,
                               m_DecompressProgressCurrentIndex,
                               m_DecompressProgressMax);
    
	// If decompress file finished 
	if (m_DecompressProgressMax <= m_DecompressProgressCurrentIndex) {

		m_DecompressProgressMax = 0;
		m_DecompressProgressCurrentIndex = 0;

		// 나머지 업데이트 패키지 파일 다운로드를 시작한다.
        this->scheduleAfterDelay([=]() {
            this->downloadNextFile();
        }, 1.f, "DownloadNextFile");
	}
}

void CDownloadManager::initDownloadList()
{
	auto isNewFile = [=](string fileName)->bool {

		auto findDataFromList = [=](const DOWNLOAD_LIST& list) {
			return std::find_if(std::begin(list), std::end(list), [=](DOWNLOAD_FILE data){
				return (data._fileName == fileName);
			});
		};

		if (m_CurrentPackage._packageVersion == 0) return true;

		auto currentFileData = findDataFromList(m_CurrentPackage._fileInfoList);
		auto newFileData = findDataFromList(m_NewPackage._fileInfoList);

		if (currentFileData == std::end(m_CurrentPackage._fileInfoList)) return true;
		if (newFileData->_fileVersion > currentFileData->_fileVersion) return true;
	
		return false;
	};

	for (auto fileInfo : m_NewPackage._fileInfoList)
	{
		if (isNewFile(fileInfo._fileName))
			m_DownloadList.push_back(fileInfo);
	}

	m_DownloadMax = m_DownloadList.size();
}

bool CDownloadManager::newVersionExist()
{
	return (m_NewPackage._packageVersion > m_CurrentPackage._packageVersion);
}

bool CDownloadManager::isMinBuildVersion()
{
    std::string requireMinVersion = "";
    std::string currentVersion    = Application::getInstance()->getVersion();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    requireMinVersion = m_NewPackage._minBuildVersionIos;
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    requireMinVersion = m_NewPackage._minBuildVersionAndroid;
#endif
    
    requireMinVersion = std::regex_replace(requireMinVersion, std::regex("[^0-9]"), "");
    currentVersion    = std::regex_replace(currentVersion,    std::regex("[^0-9]"), "");

    
    int requireMinVer = StringUtility::toNumber<int>(requireMinVersion);
    int currentVer    = StringUtility::toNumber<int>(currentVersion);
    
    return requireMinVer <= currentVer;
}

void CDownloadManager::loadLocalPackageInfoFile()
{
	/** first of all. Load file from local path.
	*  If there is no file, Load from writable path.
	*  If no file in the writable path, the program will be exit.
	*/
	string localPath = m_WritablePath + PACKAGE_FILE_PATH;
	string writableData = FileUtils::getInstance()->getStringFromFile(localPath);
	string localData = FileUtils::getInstance()->getStringFromFile(PACKAGE_FILE_PATH);
	string packageInfo = "";
    
    CCLOG("Local path : %s", localPath.c_str());

	if		(writableData.size() != 0)	packageInfo = writableData;
	else if (localData.size() != 0)		packageInfo = localData;
	else								{ this->packageLoadFailed(); return; }

	this->initPackageInfo(m_CurrentPackage, packageInfo);
}

void CDownloadManager::initPackageInfo(PACKAGE_DATA& packageInfo, std::string jsonData)
{
	if (jsonData.size() == 0) return;

	Json::Value root;
	Json::Reader reader;

	std::string data = jsonData;
	size_t pos = data.rfind("}");
	data = data.substr(0, pos + 1);

	if (!reader.parse(data, root))
	{
        MessageBox("File not found", "Error");
        CCASSERT(false, StringUtils::format("parser failed : \n %s", data.c_str()).c_str());
        return;
	}
	CCLOG("Download list JSON : \n %s\n", data.c_str());

	packageInfo._packageVersion         = root["patchversion"].asInt();
    packageInfo._minBuildVersionAndroid = root["minBuildVersionAndroid"].asString();
    packageInfo._minBuildVersionIos     = root["minBuildVersionIos"].asString();
    packageInfo._appLinkAndroid         = root["appLinkAndroid"].asString();
    packageInfo._appLinkIos             = root["appLinkIos"].asString();
    packageInfo._facebookPageLink       = root["facebookPageLink"].asString();

	auto fileArray = root["files"];
	packageInfo._downloadFileCount  = fileArray.size();

	for (unsigned int fileCount = 0; fileCount < packageInfo._downloadFileCount; fileCount++)
	{
		auto file = fileArray[fileCount];

		DOWNLOAD_FILE downloadInfo;

		downloadInfo._fileName		= file["filename"].asString();
		downloadInfo._url			= file["url"].asString();
		downloadInfo._localPath		= file["localpath"].asString();
		downloadInfo._fileVersion	= file["version"].asInt();
		downloadInfo._isCompressed	= file["compress"].asInt();

		packageInfo._fileInfoList.push_back(downloadInfo);
	}
}

void CDownloadManager::packageLoadFailed()
{
    this->callVoidListener(m_DownloadFailedListener);
    m_FileDownloadProgress = nullptr;
    m_FileDecompressProgress = nullptr;
}

void CDownloadManager::requestDownload(std::string url, ccHttpRequestCallback callback)
{
	CCLOG("%s", __FUNCTION__);

	if (url.size() == 0) return;

	auto request = new HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(callback);

	HttpClient::getInstance()->send(request);
	request->release();
}

bool CDownloadManager::savePackageInfoFile(const string path, const string &data)
{
	return this->saveDataToFile("wt", path, data);
}

bool CDownloadManager::savePackageFile(const string path, const vector<char> *buf)
{
	string data = "";
	data.resize(buf->size());
	std::copy(std::begin(*buf), std::end(*buf), std::begin(data));
	return this->saveDataToFile("wb", path, data);
}

bool CDownloadManager::saveDataToFile(const string mode, const string path, const string &data)
{
	FILE *f = fopen(path.c_str(), mode.c_str());
	if (f == NULL) return false;

	fwrite(data.c_str(), 1, data.length(), f);
	fclose(f);
	return true;
}

bool CDownloadManager::decompress(const string &zip) {
	// Find root path for zip file
	size_t pos = zip.find_last_of("/\\");
	if (pos == std::string::npos)
	{
		CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
		return false;
	}
	const std::string rootPath = zip.substr(0, pos + 1);

	// Open the zip file
	unzFile zipfile = unzOpen(FileUtils::getInstance()->getSuitableFOpen(zip).c_str());

	if (!zipfile)
	{
		CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
		return false;
	}

	// Get info about the zip file
	unz_global_info global_info;
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
		unzClose(zipfile);
		return false;
	}

	// Buffer to hold data read from the zip file
	char readBuffer[BUFFER_SIZE];
	// Loop to extract all files.
	unsigned long i;
	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info fileInfo;
		char fileName[MAX_FILENAME];
		if (unzGetCurrentFileInfo(zipfile,
			&fileInfo,
			fileName,
			MAX_FILENAME,
			NULL,
			0,
			NULL,
			0) != UNZ_OK)
		{
			CCLOG("AssetsManagerEx : can not read compressed file info\n");
			unzClose(zipfile);
			return false;
		}

		// 패키지 파일 압축을 풀기 진행 상황을 알린다.
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
			m_DecompressProgressMax          = global_info.number_entry;
			m_DecompressProgressCurrentIndex = i + 1;

            this->progressDecompressPackageFile();
        });

		const std::string fullPath = rootPath + fileName;

		// Check if this entry is a directory or a file.
		const size_t filenameLength = strlen(fileName);
		if (fileName[filenameLength - 1] == '/')
		{
			//There are not directory entry in some case.
			//So we need to create directory when decompressing file entry
			if (!FileUtils::getInstance()->createDirectory(basename(fullPath)))
			{
				// Failed to create directory
				CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
				unzClose(zipfile);
				return false;
			}
		}
		else
		{
			// Entry is a file, so extract it.
			// Open current file.
			if (unzOpenCurrentFile(zipfile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
				unzClose(zipfile);
				return false;
			}

			// Create a file to store current file.
			FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
			if (!out)
			{
				CCLOG("AssetsManagerEx : can not create decompress destination file %s\n", fullPath.c_str());
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return false;
			}

			// Write current file content to destinate file.
			int error = UNZ_OK;
			do
			{
				error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
				if (error < 0)
				{
					CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
					fclose(out);
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				if (error > 0)
				{
					fwrite(readBuffer, error, 1, out);
				}
			} while (error > 0);

			fclose(out);
		}

		unzCloseCurrentFile(zipfile);

		// Goto next entry listed in the zip file.
		if ((i + 1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
				unzClose(zipfile);
				return false;
			}
		}
	}

	unzClose(zipfile);
	return true;
}

void CDownloadManager::callVoidListener(VOID_LISTENER& listener)
{
    if(listener)
    {
        listener();
        listener = nullptr;
    }
}

void CDownloadManager::callProgressListener(PROGRESS_LISTENER& listener,
                                            int cur, int max)
{
    if(listener)
    {
        listener(cur, max);
    }
}

void CDownloadManager::scheduleAfterDelay(std::function<void()> listener,
                                          float delay, std::string key)
{
    cocos2d::Director::getInstance()->getScheduler()->schedule([=](float delta){
        if(listener == nullptr) return;
        
        listener();
    }, cocos2d::Director::getInstance(), 0.f, 0, delay, false, key);
}
