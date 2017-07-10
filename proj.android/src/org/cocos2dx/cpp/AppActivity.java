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

import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import com.unity3d.ads.UnityAds;

public class AppActivity extends Cocos2dxActivity{

	public static final String TAG = "AppActivity";
	private static AppActivity m_AppActivity = null;
//	private static UnityAdsUtils m_UnityAdsUtils = null;
//	private static GoogleUtils m_GoogleUtils = null;
	private static Handler m_Handler = null;
	private static String m_ToastMessage = "";
	private static HashMap<String, String> m_UserDataList = new HashMap<String, String>();
	
	 // [START on_create]
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	
    	Log.d(TAG, "AppActivty Create Start");
 
    	m_AppActivity = this;
        final UnityAdsListener unityAdsListener = new UnityAdsListener();
        UnityAdsJNI.activity = m_AppActivity;
        UnityAdsJNI.unityAdsListener = unityAdsListener;
        
//    	m_UnityAdsUtils = new UnityAdsUtils();
//    	m_GoogleUtils = new GoogleUtils();
//    	m_UnityAdsUtils.init(m_AppActivity);
//    	m_GoogleUtils.init(m_AppActivity);
    	
    	
    	
    	
    	m_Handler = new Handler(){
    		@Override
    		public void handleMessage(Message msg)
    		{
    			Toast.makeText(m_AppActivity, m_ToastMessage, Toast.LENGTH_SHORT).show();
    			super.handleMessage(msg);
    		}
    	};
        
    	Log.d(TAG, "AppActivty Create End");
    }
    
    @Override
    protected void onStart() {
    	Log.d(TAG, "onStart function called");
        super.onStart();
    }
    
    @Override
   	protected void onResume() {
    	Log.d(TAG, "onResume function called");
   		super.onResume();
//   		m_UnityAdsUtils.Resume();
    }
    
    @Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
    	super.onActivityResult(requestCode, resultCode, intent);
//    	m_GoogleUtils.ActivityResult(requestCode, resultCode, intent);
    }

    @Override
    protected void onStop() {
    	Log.d(TAG, "onStop function called");
        super.onStop();
    }   
    
    @Override
    protected void onPause() {
    	Log.d(TAG, "onPause function called");
    	super.onPause();
//    	CPP_AutoSave();     
    }   
    
//    public static void CPP_GoogleLogin()
//    {
//    	Log.d(TAG, "CPP_GoogleLogin function called");
//    	
//    	if(m_GoogleUtils == null)
//    		return;
//    	
//    	m_GoogleUtils.GoogleLogin();
//    }
     
    // Google Cloud Save      Key / Value
//    public static void CPP_GoogleCloudSave(String key, String value)
//    {
//    	Log.d(TAG, "CPP_GoogleCloudSave function called");
//    	
//    	if(m_GoogleUtils == null)
//    		return;
//    	
//    	if(m_GoogleUtils.isSignedIn()){
//    		m_GoogleUtils.saveSnapshot(null, key, value);
//    	}
//    }
    
    // Google Cloud Load      Key
//    public static void CPP_GoogleCloudLoad(String key)
//    {
//    	Log.d(TAG, "CPP_GoogleCloudLoad function called");
//    	
//    	if(m_GoogleUtils == null)
//    		return;
//    	
//    	if(m_GoogleUtils.isSignedIn()){
//    		m_GoogleUtils.loadFromSnapshot(null, key);
//    	}
//    }
    
    // Set user data to auto save list
    public static void CPP_AddDataToAutoSaveList(String key, String value)
    {
    	Log.d(TAG, "CPP_AddDataToAutoSaveList function called");

    	m_UserDataList.put(key, value);
    }
    
    // Set user data to auto save list
    public static void CPP_AutoSave()
    {
    	Log.d(TAG, "CPP_AutoSave function called");
    	Log.d(TAG, "User data list size : " + m_UserDataList.size());

    	if(m_UserDataList.size() > 0)
    	{
    		if(BaseGameUtils.isNetWork(m_AppActivity)) 
        	{
    			for(String key : m_UserDataList.keySet()){
    	    		String data = m_UserDataList.get(key);
    	    		Log.d(TAG, "Auto save key : " + key);
    	    		Log.d(TAG, "data : " + data);
//    	    		CPP_GoogleCloudSave(key, data);
    	    	}
    			m_UserDataList.clear();
        	}
    		else{
        		Log.d(TAG, "You can not store user data in Google Cloud because your network is not connected.");
    		}
    	}   
    }
    
    // Reward Unity Ads
//    public static void CPP_ShowRewardUnityAds()
//    {
//    	Log.d(TAG, "CPP_ShowRewardUnityAds function called");
//    	
//    	if(m_UnityAdsUtils == null)
//    		return;
//    	
//    	if(!UnityAdsUtils.isVideoPlaying){
//    		m_UnityAdsUtils.ShowRewardUnityAds();
//    	}
//    }
    
    // Normal Unity Ads
//    public static void CPP_ShowNormalUnityAds()
//    {
//    	Log.d(TAG, "CPP_ShowNormalUnityAds function called");
//    	
//    	if(m_UnityAdsUtils == null)
//    		return;
//    	
//    	if(!UnityAdsUtils.isVideoPlaying){
//    		m_UnityAdsUtils.ShowNormalUnityAds();
//    	}
//    }
    
    // Toast
    public static void CPP_Toast(String content)
    {
    	m_ToastMessage = content;
    	m_Handler.sendMessage( new Message() );
    }
    
    // Is Network Connect
    public static void CPP_IsNetworkConnect()
    {
    	JAVA_NetworkConnect(BaseGameUtils.isNetWork(m_AppActivity));
    }
    
    // Call Cocos2dx Function - Google Login Result
    public native static void JAVA_GoogleConnectionResult(final boolean isSucceed);
    
    // Call Cocos2dx Function - Google Connection Suspended
    public native static void JAVA_GoogleConnectionSuspended();
        
    // Call Cocos2dx Function - Google Cloud Data Load
    public native static void JAVA_GoogleCloudLoad(final String key, final String value);
    
    // Call Cocos2dx Function - Google Cloud Data Save Succeed
    public native static void JAVA_GoogleCloudSaveSucceed(long unixTime);
    
    // Call Cocos2dx Function - Normal UnityAds Ready
//    public native static void JAVA_NormalUnityAdsReady();
    
    // Call Cocos2dx Function - Reward UnityAds Ready
//    public native static void JAVA_RewardUnityAdsReady();

    // Call Cocos2dx Function - UnityAds Start
//    public native static void JAVA_UnityAdsStart();
    
    // Call Cocos2dx Function - UnityAds Finish
//    public native static void JAVA_UnityAdsFinish();
    
    // Call Cocos2dx Function - UnityAds Error
//    public native static void JAVA_UnityAdsError();
    
    // Call Cocos2dx Function - Network Connect
    public native static void JAVA_NetworkConnect(final boolean isConnect);
}