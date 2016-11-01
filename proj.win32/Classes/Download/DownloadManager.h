#pragma once

#include "cocos2d.h"
#include "../network/HttpClient.h"


USING_NS_CC;
using namespace network;
using namespace std;

class RefValueMap : public Ref {
public:
	void put(const string &key, Ref *value) {
		map.insert(key, value);
	}

	Ref *get(const string &key) {
		return map.at(key);
	}

	void putInt(const string &key, int value) {
		auto obj = __Integer::create(value);
		put(key, obj);
	}

	int getInt(const string &key) {
		auto obj = (__Integer *)get(key);
		return obj->getValue();
	}

private:
	Map<string, Ref *> map;
};

class CDownloadManager : public Node {
public:
	CDownloadManager();
	virtual ~CDownloadManager();

public:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(CDownloadManager);

private:
	void downloadVersionFile();
	void onDownloadVersionFile(HttpClient *client, HttpResponse *response);

	void downloadNextPackageFile();
	void onProgressDownloadPackageFile(Ref *object);
	void onDownloadPackageFile(HttpClient *client, HttpResponse *response);

	void decompressPackageFile(int fileIdx);
	void onProgressDecompressPackageFile(Ref *object);

	bool saveVersionFile(const string &path, const string &buf);
	bool savePackageFile(const string &path, const vector<char> *buf);

private:
    struct sDOWNLOADFILE{
        std::string _fileName;
        std::string _url;
        std::string _localPath;
        int _version;
        bool _isCompressed;
        
        sDOWNLOADFILE()
        : _fileName("")
        , _url("")
        , _localPath("")
        , _version(0)
        , _isCompressed(false){}
    };
    
	string m_ServerVersionFileData;
	string m_PackageURL;
	int m_ClientVersion;
	int m_DownloadIndex;
	int m_DownloadCount;
	vector<sDOWNLOADFILE> m_UpdatePackages;
};