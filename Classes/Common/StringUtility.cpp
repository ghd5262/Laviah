#include "StringUtility.h"

#include <sstream>

USING_NS_CC;
using namespace std;

/**
 *  oldChar -> newChar로 변환
 */
string StringUtility::replaceAll(const string &str, const string &oldStr, const string &newStr) {
    
    string result = str;
    for(string::size_type i = 0; (i = result.find(oldStr, i)) != string::npos;)
    {
        result.replace(i, oldStr.length(), newStr);
        i += newStr.length();
    }
    return result;
}

/**
 *  대문자로 변환
 */
string StringUtility::toUpper(const string &str) {
    
    string result = str;
    
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

/**
 *  소문자로 변환
 */
string StringUtility::toLower(const string &str) {
    
    string result = str;
    
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

/**
 *  백의 자리에 ','를 추가 한다. ex) 1,000
 */
string StringUtility::toCommaString(int number) {
    
    string commaString;
    int cnt = 0;
    
    do {
        commaString.insert(0, 1, char('0' + number % 10));
        number /= 10;
        if( ++cnt == 3 && number ) {
            commaString.insert(0, 1, ',');
            cnt = 0;
        }
        
    } while(number);
    
    return commaString;
}

string StringUtility::trimLeft(const string &str) {
    
    string::size_type n = str.find_first_not_of(" \t\v\n");
    return n == string::npos ? str : str.substr(n, str.length());
}

string StringUtility::trimRight(const string &str) {
    
    string::size_type n = str.find_last_not_of(" \t\v\n");
    return n == string::npos ? str : str.substr(0, n + 1);
}

string StringUtility::trim(const string &str) {
    
    return trimLeft(trimRight(str));
}

string StringUtility::insert(vector<string> arr, const string &insertStr) {
    
    string result = "";
    
    for( int i = 0; i < arr.size(); ++i ) {
        result += arr[i];
        
        if( i < arr.size() - 1 ) {
            result += insertStr;
        }
    }
    
    return result;
}

vector<string> StringUtility::split(const string &str, char separators) {
    
    vector<string> result;
    
    istringstream f(str);
    string s;
    
    while( getline(f, s, separators) ) {
        result.push_back(s);
    }
    
    return result;
}

/**
 * 모든 문자가 Integer로 되어 있는지 확인
 */
bool StringUtility::isInteger(const string &str) {
    
    auto pred = [](char c) {
        
        return !isdigit(c);
    };
    
    return ( !str.empty() &&
            (find_if(str.begin(), str.end(), pred) == str.end()) );
}

vector<string> StringUtility::toArray(const string &str) {
    
    const char *text = str.c_str();
    
    int startPos = 0;
    int endPos = 0;
    char ch = *text;
    vector<string> strings;
    
    while (ch) {
        if (0x80 != (0xC0 & ch) &&
            (endPos != 0)) {
            
            string tmp = str.substr(startPos, endPos - startPos);
            strings.push_back(tmp);
            
            startPos = endPos;
        }
        
        ++text;
        ++endPos;
        
        ch = *text;
        
        // 마지막 글자
        if(!ch) {
            string tmp = str.substr(startPos, endPos - startPos);
            strings.push_back(tmp);
            
            break;
        }
    }
    
    return strings;
}

int StringUtility::calcCharCount(const string &str) {
    
    const char *text = str.c_str();
    
    int n = 0;
    char ch = 0;
    
    while ((ch = *text)) {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch)) {
            ++n;
        }
        
        ++text;
    }
    
    return n;
}

string StringUtility::toString(vector<char> *vec) {
    
    string result(vec->begin(),vec->end());
    return result;
}

Texture2D* StringUtility::toTexture(const string &result) {
    
    vector<unsigned char> buffer(result.begin(), result.end());
    
    Image *img = new Image();
    img->initWithImageData(&(buffer.front()), buffer.size());
    
    Texture2D* texture = new Texture2D();
    texture->initWithImage(img);
    img->release();
    
    texture->autorelease();
    
    return texture;
}



