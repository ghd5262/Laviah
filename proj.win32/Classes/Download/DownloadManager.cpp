#include "json/document.h"
#include "unzip/unzip.h"
#include "DownloadManager.h"
#include "../json/json.h"
#include "../Common/NoticeDefine.h"
#include "../MyUI/LevelProgressBar.h"

using namespace std;

#define	BUFFER_SIZE		8000 // 5MB
#define	MAX_FILENAME	512

namespace DOWNLOAD_NOTICE {
    const static std::string DECOMPRESS_PROCESS     = "decompressProcess";
    const static std::string DOWNLOAD_PROCESS       = "downloadProcess";
}

namespace DOWNLOAD_DEFINE {
	const static std::string PACKAGE_FILE_PATH		= "update/package.json";
	const static std::string PACKAGE_INFO_FILE_PATH = std::string("http://www.hshgames.com/game/project_s/") + PACKAGE_FILE_PATH;
	const static int TAG_ACTION_DOWNLOADING			= 100;
}

using namespace DOWNLOAD_NOTICE;
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
, m_CurrentGuage(0)
{
	m_WritablePath = FileUtils::getInstance()->getWritablePath();
}

CDownloadManager::~CDownloadManager() {
}

bool CDownloadManager::init() {
	CCLOG("%s", __FUNCTION__);

	if (!Node::init()) return false;
    
    auto createNotice = [=](cocos2d::SEL_CallFuncO selector, std::string name){
        __NotificationCenter::getInstance()->addObserver(this, selector, name, NULL);
    };
    
	createNotice(callfuncO_selector(CDownloadManager::progressDownloadPackageFile),	  DOWNLOAD_NOTICE::DOWNLOAD_PROCESS);
	createNotice(callfuncO_selector(CDownloadManager::progressDecompressPackageFile), DOWNLOAD_NOTICE::DECOMPRESS_PROCESS);

	Size progressBarSize = Size(Director::getInstance()->getVisibleSize().width * 0.7f, 30);

	this->setContentSize(progressBarSize);
	m_DownloadGauge = CLevelProgressBar::create(progressBarSize, 20);
	m_DownloadGauge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_DownloadGauge->setPosition(this->getContentSize() / 2);
	this->addChild(m_DownloadGauge);

	// Request to server to download the file that include new package infomation.
	this->downloadPackageInfoFile();

	return true;
}

void CDownloadManager::downloadPackageInfoFile() {
	CCLOG("%s", __FUNCTION__);

	auto versionFilePath = m_WritablePath + "update/";
    if (FileUtils::getInstance()->createDirectory(versionFilePath))
    {
		this->requestDownload(PACKAGE_INFO_FILE_PATH, (ccHttpRequestCallback)CC_CALLBACK_2(CDownloadManager::downloadCompletePackageInfoFile, this));
    }
}

// 요청한 리소스 버전 파일 다운로드가 완료되면 호출된다.
void CDownloadManager::downloadCompletePackageInfoFile(HttpClient *client, HttpResponse *response) {
	CCLOG("%s", __FUNCTION__);

	// 리소스 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL) { this->packageLoadFailed(); return; }

	auto data = response->getResponseData();

	m_ServerVersionFileData = string(data->begin(), data->end());
	this->initPackageInfo(m_NewPackage, m_ServerVersionFileData);
	this->loadLocalPackageInfoFile();

	if (isThereAnyNewVersion()) this->initDownloadList();
	
	// 업데이트 패키지 파일 다운로드를 시작한다.
	this->downloadNextFile();
}

// 업데이트 서버에서 패키지 파일 다운로드를 요청한다.
void CDownloadManager::downloadNextFile() {
	CCLOG("%s", __FUNCTION__);

	// 모든 패키지 파일 다운로드가 완료되었을 경우
	if (m_DownloadMax <= m_DownloadCurrentIndex) {

		auto downloadComplete = [=](){
			sendNotice(NOTICE::DOWN_COMPLETE);
			removeFromParentAndCleanup(true);
		};

		// 다운로드한 버전 파일을 저장한다.
		auto versionFilePath = m_WritablePath + PACKAGE_FILE_PATH;
		savePackageInfoFile(versionFilePath, m_ServerVersionFileData);

		m_CurrentGuage = m_DownloadGauge->getCurrentLevel();
		auto MaxLevel = m_DownloadGauge->getMaxLevel();
		if (m_CurrentGuage < MaxLevel)
		{
			auto action = Sequence::create(Repeat::create(Sequence::create(CallFunc::create([=](){

				m_DownloadGauge->setCurrentLevel(++m_CurrentGuage);
				m_DownloadGauge->UpdateProgress();

			}), DelayTime::create(1.f / float(MaxLevel)), nullptr), m_DownloadGauge->getMaxLevel()), 
				DelayTime::create(0.5f),
				CallFunc::create([=](){

				downloadComplete();

			}), nullptr);

			this->runAction(action);
		}
		else
		{
			downloadComplete();
		}
		return;
    }

	// 패키지 파일의 다운로드를 요청한다.
	auto url = m_DownloadList.at(m_DownloadCurrentIndex)._url;
	this->requestDownload(url, (ccHttpRequestCallback)CC_CALLBACK_2(CDownloadManager::downloadCompletePackageFile, this));
	
	// 요청한 패키지 파일의 다운로드 진행 상태를 0.5초에 한번씩 Polling하여 모니터링한다.
	auto action = RepeatForever::create(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([=]() {

		CCLOG("Download %d / %d", m_DownloadCurrentIndex, m_DownloadMax);

		this->sendNotice(DOWNLOAD_NOTICE::DOWNLOAD_PROCESS);

		// 다운로드가 완료되었다면
		if (m_DownloadMax <= m_DownloadCurrentIndex) {
			// 모니터링을 그만둔다.
			stopActionByTag(TAG_ACTION_DOWNLOADING);
		}
	})));

	action->setTag(TAG_ACTION_DOWNLOADING);
	runAction(action);
}

// 패키지 파일 다운로드가 진행되는 동안 호출된다.
void CDownloadManager::progressDownloadPackageFile(Ref *object) {
	this->updateProgressBar();
}

// 패키지 파일 다운로드가 완료되면 호출된다.
void CDownloadManager::downloadCompletePackageFile(HttpClient *client, HttpResponse *response) {
	CCLOG("%s", __FUNCTION__);

	// 애셋 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL) { this->packageLoadFailed(); return; }

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
			auto action = Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]() {
				decompressPackageFile(currentDownloadIdx);
			}));
            runAction(action);
        }
        else{
            
            // 나머지 업데이트 패키지 파일 다운로드를 시작한다.
            auto action = Sequence::createWithTwoActions(DelayTime::create(1.0f),
				CallFunc::create([=](){
				this->downloadNextFile();
			}));
            runAction(action);
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
			if (!decompress(packageFilePath)) { this->packageLoadFailed(); return; }
            
            // 압축을 푼 패키지 파일을 삭제한다.
            FileUtils::getInstance()->removeFile(packageFilePath);
        });
        
        thread.detach();
    }
}

void CDownloadManager::progressDecompressPackageFile(Ref *object) {

	CCLOG("Decompressing... %d / %d", m_DecompressProgressCurrentIndex, m_DecompressProgressMax);
  
	// If decompress file finished 
	if (m_DecompressProgressMax <= m_DecompressProgressCurrentIndex) {

		m_DecompressProgressMax = 0;
		m_DecompressProgressCurrentIndex = 0;

		// 나머지 업데이트 패키지 파일 다운로드를 시작한다.
		auto action = Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]() {
			this->downloadNextFile();
		}));
		runAction(action);
	}
}

void CDownloadManager::initDownloadList()
{
	auto isNewFile = [=](string fileName)->bool {

		auto findDataFromList = [=](const DOWNLOAD_LIST& list) {
			return std::find_if(std::begin(list), std::end(list), [=](sDOWNLOADFILE data){
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

bool CDownloadManager::isThereAnyNewVersion()
{
	return (m_NewPackage._packageVersion > m_CurrentPackage._packageVersion);
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

	if		(writableData.size() != 0)	packageInfo = writableData;
	else if (localData.size() != 0)		packageInfo = localData;
	else								{ this->packageLoadFailed(); return; }

	this->initPackageInfo(m_CurrentPackage, packageInfo);
}

void CDownloadManager::onExit() {
	Node::onExit();

	stopAllActions();
	__NotificationCenter::getInstance()->removeAllObservers(this);
}

void CDownloadManager::initPackageInfo(sPACKAGE_INFO& packageInfo, std::string jsonData)
{
	if (jsonData.size() == 0) return;

	Json::Value root;
	Json::Reader reader;

	std::string data = jsonData;
	size_t pos = data.rfind("}");
	data = data.substr(0, pos + 1);

	if (!reader.parse(data, root))
	{
		CCASSERT(false, StringUtils::format("parser failed : \n %s", data.c_str()).c_str());
		return;
	}
	CCLOG("Download list JSON : \n %s\n", data.c_str());

	packageInfo._packageVersion = root["patchversion"].asInt();

	auto fileArray = root["files"];
	packageInfo._downloadFileCount = fileArray.size();

	for (unsigned int fileCount = 0; fileCount < packageInfo._downloadFileCount; fileCount++)
	{
		auto file = fileArray[fileCount];

		sDOWNLOADFILE downloadInfo;

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
	sendNotice(NOTICE::DOWN_ERROR);
	removeFromParentAndCleanup(true);
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

void CDownloadManager::sendNotice(std::string key, Ref* sender/* = nullptr*/)
{
	__NotificationCenter::getInstance()->postNotification(key, sender);
}

void CDownloadManager::updateProgressBar()
{
	auto perMaxLevel = float(m_DownloadGauge->getMaxLevel()) / float(m_DownloadMax);

	auto currentLevel = int(perMaxLevel * m_DownloadCurrentIndex);

	m_DownloadGauge->setCurrentLevel(currentLevel);
	m_DownloadGauge->UpdateProgress();
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
			m_DecompressProgressMax = global_info.number_entry;
			m_DecompressProgressCurrentIndex = i + 1;

			this->sendNotice(DOWNLOAD_NOTICE::DECOMPRESS_PROCESS, nullptr);
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