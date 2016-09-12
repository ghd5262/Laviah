#include "json/document.h"
#include "unzip/unzip.h"
#include "AssetPanel.h"

#define	BUFFER_SIZE		8192
#define	MAX_FILENAME	512

static string basename(const string &path) {
    size_t found = path.find_last_of("/\\");
    
    if (string::npos != found)
        return path.substr(0, found);
    else
        return path;
}

static bool decompress(const string &zip) {
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
            RefValueMap values;
            values.putInt("total", global_info.number_entry);
            values.putInt("now", i + 1);
            
            __NotificationCenter::getInstance()->postNotification("AssetPanel::onProgressDecompressPackageFile", (Ref *)&values);
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

AssetPanel::AssetPanel()
: m_PackageURL("")
, m_ServerVersionFileData("")
, m_ClientVersion(0)
, m_DownloadIndex(0)
, m_DownloadCount(0)
{
    m_PackageURL = "";
		m_UpdatePackages.clear();
}

AssetPanel::~AssetPanel() {
}

bool AssetPanel::init() {
	if (!Node::init())
        return false;
    
    auto statusLabel = Label::createWithTTF("Status...", "fonts/Marker Felt.ttf", 20);
    statusLabel->setName("statusLabel");
    statusLabel->setColor(Color3B::RED);
    statusLabel->setPosition(0, 15);
    addChild(statusLabel);
    
    auto prograssLabel = Label::createWithTTF("Progress...", "fonts/Marker Felt.ttf", 20);
    prograssLabel->setName("prograssLabel");
    prograssLabel->setColor(Color3B::RED);
    prograssLabel->setPosition(0, -15);
    addChild(prograssLabel);
    
	__NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(AssetPanel::onProgressDownloadPackageFile), "AssetPanel::onProgressDownloadPackageFile", NULL);
	__NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(AssetPanel::onProgressDecompressPackageFile), "AssetPanel::onProgressDecompressPackageFile", NULL);

	return true;
}

void AssetPanel::onEnter() {
	Node::onEnter();

	// 번들 디렉토리에서 클라이언트 리소스 버전 파일을 읽어들인다. 만약 번들 디렉토리에
	// 파일이 없으면 캐쉬 디렉토리에서 읽어들인다.
    
    auto versionFilePath = FileUtils::getInstance()->getWritablePath() + "update/package.json";
    
    string versionData = "";
    string cache = FileUtils::getInstance()->getStringFromFile(versionFilePath);
    string buf = FileUtils::getInstance()->getStringFromFile("update/package.json");
    if(cache.length() != 0)
    {
        versionData = cache;
    }
    else if(buf.length() != 0)
    {
        versionData = buf;
    }
    else{
        // 클라이언트 리소스 버전 파일을 읽어들이는데 실패했다면 로고 화면으로 전환한다.
        __NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
        removeFromParentAndCleanup(true);
        return;
    }

	// 클라이언트 리소스 버전 파일을 읽어들여 현재 적용된 업데이트의 버전을 알아낸다.
	rapidjson::Document json;
	json.Parse<0>(versionData.c_str());

	if (json.HasParseError()) {
		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

    m_PackageURL = json["package"].GetString();
    m_ClientVersion = json["version"].GetInt();

	// 업데이트 서버에서 리소스 버전 파일 다운로드를 요청한다.
	downloadVersionFile();
}

void AssetPanel::onExit() {
	Node::onExit();

	stopAllActions();
	__NotificationCenter::getInstance()->removeAllObservers(this);
}

// 업데이트 서버에서 리소스 버전 파일 다운로드를 요청한다.
void AssetPanel::downloadVersionFile() {
	CCLOG("AssetPanel::downloadVersionFile()");

    auto versionFilePath = FileUtils::getInstance()->getWritablePath() + "update/";
    if (FileUtils::getInstance()->createDirectory(versionFilePath))
    {
        auto request = new HttpRequest();
        request->setUrl(m_PackageURL);
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback((ccHttpRequestCallback)std::bind(&AssetPanel::onDownloadVersionFile, this, std::placeholders::_1, std::placeholders::_2));
        
        HttpClient::getInstance()->send(request);
        request->release();
    }
}

// 요청한 리소스 버전 파일 다운로드가 완료되면 호출된다.
void AssetPanel::onDownloadVersionFile(HttpClient *client, HttpResponse *response) {
	log("AssetPanel::onDownloadVersionFile()");

	// 리소스 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL) {
		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

	auto data = response->getResponseData();

	m_ServerVersionFileData = string(data->begin(), data->end());

	if (m_ServerVersionFileData.length() == 0)
		return;

	rapidjson::Document json;
	json.Parse<0>(m_ServerVersionFileData.c_str());

	if (json.HasParseError()) {
		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

	int serverVersion = json["version"].GetInt();

	// 클라이언트 리소스가 최신 버전이면 업데이트를 완료한다.
	if (m_ClientVersion >= serverVersion) {
		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateComplete", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

	// 다운로드해야할 업데이트 패키지 파일을 리스트에 저장한다.
	const auto &files = json["files"];

	for (rapidjson::Value::ConstMemberIterator itr = files.MemberBegin(); itr != files.MemberEnd(); ++itr) {
		string key = itr->name.GetString();

		if (!itr->value.HasMember("url"))
			continue;

		string url = itr->value["url"].GetString();

		int packageVersion = atoi(key.c_str());

		// 이미 적용된 업데이트 버전이라면 생략한다.
		if (packageVersion <= m_ClientVersion)
			continue;

		m_UpdatePackages.push_back(url);
	}

	m_DownloadCount = m_UpdatePackages.size();
    
	// 업데이트 패키지 파일 다운로드를 시작한다.
	downloadNextPackageFile();
}

// 업데이트 서버에서 패키지 파일 다운로드를 요청한다.
void AssetPanel::downloadNextPackageFile() {
	log("AssetPanel::downloadNextPackageFile()");

	// 모든 패키지 파일 다운로드가 완료되었을 경우
	if (m_UpdatePackages.size() == 0) {
		// 다운로드한 버전 파일을 저장한다.
		auto versionFilePath = FileUtils::getInstance()->getWritablePath() + "update/package.json";
		saveVersionFile(versionFilePath, m_ServerVersionFileData);

		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateComplete", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

	++m_DownloadIndex;

	// 패키지 파일의 다운로드를 요청한다.
	auto request = new HttpRequest();
	request->setUrl((*m_UpdatePackages.begin()).c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback((ccHttpRequestCallback)std::bind(&AssetPanel::onDownloadPackageFile, this, std::placeholders::_1, std::placeholders::_2));

	HttpClient::getInstance()->send(request);
	request->release();

	// 다운로드 요청된 패키지 파일을 리스트에서 삭제하고
	m_UpdatePackages.erase(m_UpdatePackages.begin());

	// 요청한 패키지 파일의 다운로드 진행 상태를 0.5초에 한번씩 Polling하여 모니터링한다.
	auto action = RepeatForever::create(
		Sequence::createWithTwoActions(
			DelayTime::create(0.5f),
			CallFunc::create(
			[&]() {
				int total = m_DownloadCount;
                int now = m_DownloadIndex;

				RefValueMap values;
				values.putInt("total", total);
				values.putInt("now", now);

				__NotificationCenter::getInstance()->postNotification("AssetPanel::onProgressDownloadPackageFile", (Ref *)&values);

				// 다운로드가 완료되었다면
				if (total <= now) {
					// 모니터링을 그만둔다.
					stopActionByTag(100);
				}
			})));
	action->setTag(100);
	runAction(action);
}

// 패키지 파일 다운로드가 진행되는 동안 호출된다.
void AssetPanel::onProgressDownloadPackageFile(Ref *object) {
	RefValueMap *values = (RefValueMap *)object;
	int total = values->getInt("total");
	int now = values->getInt("now");

	auto statusLabel = (Label *)getChildByName("statusLabel");
	auto prograssLabel = (Label *)getChildByName("prograssLabel");

	statusLabel->setString(__String::createWithFormat("Downloading... (%d/%d)", m_DownloadIndex, m_DownloadCount)->getCString());
	prograssLabel->setString(__String::createWithFormat("%d/%d", now, total)->getCString());
}

// 패키지 파일 다운로드가 완료되면 호출된다.
void AssetPanel::onDownloadPackageFile(HttpClient *client, HttpResponse *response) {
	log("AssetPanel::onDownloadPackageFile()");

	// 애셋 버전 파일 다운로드에 실패했다면 로고 화면으로 전환한다.
	if (response == NULL) {
		__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
		removeFromParentAndCleanup(true);
		return;
	}

	// 다운로드한 업데이트 패키지 파일을 저장한다.
	string packageFilePath = FileUtils::getInstance()->getWritablePath() + "update/package.zip";
	auto buf = response->getResponseData();

	savePackageFile(packageFilePath, buf);

	// 패키지 파일의 압축을 푼다.
	auto action = Sequence::createWithTwoActions(
		DelayTime::create(1.0f),
		CallFunc::create(
		[&]() {
			decompressPackageFile();
		}));
	runAction(action);
}

void AssetPanel::decompressPackageFile() {
	log("AssetPanel::decompressPackageFile()");

	std::thread thread = std::thread([&]() {
		string packageFilePath = FileUtils::getInstance()->getWritablePath() + "update/package.zip";

		// 패키지 파일의 압축을 푸는데 실패하면 로고 화면으로 전환한다.
		if (!decompress(packageFilePath)) {
			__NotificationCenter::getInstance()->postNotification("HelloWorld::onAssetUpdateError", NULL);
			removeFromParentAndCleanup(true);
			return;
		}

		// 압축을 푼 패키지 파일을 삭제한다.
		FileUtils::getInstance()->removeFile(packageFilePath);
	});

	thread.detach();
}

void AssetPanel::onProgressDecompressPackageFile(Ref *object) {
	RefValueMap *values = (RefValueMap *)object;
	int total = values->getInt("total");
	int now = values->getInt("now");

	auto statusLabel = (Label *)getChildByName("statusLabel");
	auto prograssLabel = (Label *)getChildByName("prograssLabel");

	statusLabel->setString("Decompressing...");
	prograssLabel->setString(__String::createWithFormat("%d/%d", now, total)->getCString());

	// 압축을 다 풀었다면
	if (total <= now) {
		// 나머지 업데이트 패키지 파일 다운로드를 시작한다.
		auto action = Sequence::createWithTwoActions(
			DelayTime::create(1.0f),
			CallFunc::create(
			[&]() {
				downloadNextPackageFile();
			}));
		runAction(action);
	}
}

bool AssetPanel::saveVersionFile(const string &path, const string &buf) {
	// 다운로드한 버전 파일을 저장한다.
	FILE *f = fopen(path.c_str(), "wt");

	if (f == NULL)
		return false;

	fwrite(buf.c_str(), 1, buf.length(), f);

	fclose(f);

	return true;
}

bool AssetPanel::savePackageFile(const string &path, const vector<char> *buf) {
	// 다운로드한 업데이트 패키지 파일을 저장한다.
	FILE *f = fopen(path.c_str(), "wb");

	if (f == NULL)
		return false;

	fwrite(buf->data(), 1, buf->size(), f);

	fclose(f);

	return true;
}