/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;

public class AppActivity extends Cocos2dxActivity{

	public static final String TAG = "AppActivity";
	private static UnityAdsUtils m_UnityAdsUtils = null;
	private static GoogleUtils m_GoogleUtils = null;
	
	 // [START on_create]
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	
    	Log.d(TAG, "AppActivty Create Start");
    	
    	m_UnityAdsUtils = new UnityAdsUtils();
    	m_GoogleUtils = new GoogleUtils();
    	m_UnityAdsUtils.init(this);
    	m_GoogleUtils.init(this);
    	
    	Log.d(TAG, "AppActivty Create End");
    }
    
    @Override
   	protected void onResume() {
   		super.onResume();
   		m_UnityAdsUtils.Resume();
    }
    
    @Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
    
    // Google Cloud Save      Key / Value
    public static void CPP_GoogleCloudSave(String key, String value)
    {
    	if(m_GoogleUtils.isSignedIn()){
    		m_GoogleUtils.GoogleCloudSave(key, value);
    	}
    }
    
    // Google Cloud Load      Key
    public static void CPP_GoogleCloudLoad(String key)
    {
    	if(m_GoogleUtils.isSignedIn()){
    		m_GoogleUtils.GoogleCloudLoad(key);
    	}
    }
    
    // Reward Unity Ads
    public static void CPP_ShowRewardUnityAds()
    {
    	if(!UnityAdsUtils.isVideoPlaying){
    		m_UnityAdsUtils.ShowRewardUnityAds();
    	}
    }
    
    // Normal Unity Ads
    public static void CPP_ShowNormalUnityAds()
    {
    	if(!UnityAdsUtils.isVideoPlaying){
    		m_UnityAdsUtils.ShowNormalUnityAds();
    	}
    }
    
    // Toast
    public static void CPP_Toast(String content)
    {
    	
    }
    
    // Call Cocos2dx Function - Google Login Result
    public native static void JAVA_GoogleConnectionResult(final boolean isSucceed);
    
    // Call Cocos2dx Function - Google Connection Suspended
    public native static void JAVA_GoogleConnectionSuspended();
        
    // Call Cocos2dx Function - Google Cloud Data Load
    public native static void JAVA_GoogleCloudLoad(final String key, final String value);
    
    // Call Cocos2dx Function - Normal UnityAds Ready
    public native static void JAVA_NormalUnityAdsReady();
    
    // Call Cocos2dx Function - Reward UnityAds Ready
    public native static void JAVA_RewardUnityAdsReady();

    // Call Cocos2dx Function - UnityAds Start
    public native static void JAVA_UnityAdsStart();
    
    // Call Cocos2dx Function - UnityAds Finish
    public native static void JAVA_UnityAdsFinish();
    
    // Call Cocos2dx Function - UnityAds Error
    public native static void JAVA_UnityAdsError();
}