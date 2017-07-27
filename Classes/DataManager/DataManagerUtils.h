#pragma once

#include <iostream>
#include "cocos2d.h"

namespace DATA_MANAGER_UTILS {
    /* get non-pointer list by func */
    
    template<class __Func, class __Iterator>
    __Iterator getNonPtrListByFunc(__Func func, __Iterator list)
    {
        for (auto iter = list.begin(); iter != list.end();)
        {
            auto &item = (*iter);
            if (!func(item)) iter = list.erase(iter);
            else             iter++;
        }
        return list;
    }
    
    /* get list by func */
    template<class __Func, class __Iterator>
    __Iterator getListByFunc(__Func func, __Iterator list)
    {
        for (auto iter = list.begin(); iter != list.end();)
        {
            auto &item = (*iter);
            if (item != nullptr){
                if (!func(item)) iter = list.erase(iter);
                else             iter++;
            }
        }
        return list;
    }
    
    /* get map by func */
    template<class __Func, class __Iterator>
    __Iterator getMapByFunc(__Func func, __Iterator list)
    {
        for (auto iter = list.begin(); iter != list.end();)
        {
            auto &item = (*iter).second;
            if (item != nullptr){
                if (!func(item)) iter = list.erase(iter);
                else             iter++;
            }
        }
        return list;
    }
    
    /* delete and clean every element of list */
    template<class __Iterator>
    void listDeleteAndClean(__Iterator &list)
    {
        for (auto data : list)
        {
            delete data;
            data = nullptr;
        }
        list.clear();
    }
    
    /* delete and clean every element of map */
    template<class __Iterator>
    void mapDeleteAndClean(__Iterator &list)
    {
        for (auto data : list)
        {
            delete data.second;
            data.second = nullptr;
        }
        list.clear();
    }
    
    template<class __InIterator, class __OutIterator>
    void copyList(__InIterator &inList, __OutIterator &outList){
    	outList.resize(inList.size());
    	std::copy(inList.begin(), inList.end(), outList.begin());
    }
}
