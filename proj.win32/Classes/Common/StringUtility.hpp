#ifndef StringUtility_hpp
#define StringUtility_hpp

#include <stdio.h>

#include "cocos2d.h"

class StringUtility {
    
public:
    static std::string replaceAll(const std::string &str,
                                  const std::string &oldStr, const std::string &newStr);
    static std::string toUpper(const std::string &str);
    static std::string toLower(const std::string &str);
    static std::string toCommaString(int number);
    static std::string trimLeft(const std::string &str);
    static std::string trimRight(const std::string &str);
    static std::string trim(const std::string &str);
    static std::string insert(std::vector<std::string> arr, const std::string &insertStr);
    static std::vector<std::string> split(const std::string &str, char separators);
    static bool isInteger(const std::string &str);
    static std::vector<std::string> toArray(const std::string &str);
    static int calcCharCount(const std::string &str);
    static std::string toString(std::vector<char> *vec);
    static cocos2d::Texture2D* toTexture(const std::string &result);
    
    template <typename T>
    static std::string toString(T number) {
        
        std::stringstream ss;
        ss << number;
        
        return ss.str();
    }
    
    template <typename T>
    static T toNumber(std::string str) {
        
        std::stringstream ss(str);
        T result;
        
        return ss >> result ? result : 0;
    }
};

#endif /* StringUtility_hpp */
