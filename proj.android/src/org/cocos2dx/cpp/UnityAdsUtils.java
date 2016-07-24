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

import kr.HongSeongHee.StarStarStar.R;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.util.Log;
import android.view.View;
import android.view.animation.AlphaAnimation;


import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds;
import com.unity3d.ads.UnityAds.FinishState;
import com.unity3d.ads.UnityAds.UnityAdsError;
import com.unity3d.ads.log.DeviceLog;
import com.unity3d.ads.metadata.MediationMetaData;
import com.unity3d.ads.metadata.MetaData;
import com.unity3d.ads.metadata.PlayerMetaData;
import com.unity3d.ads.misc.Utilities;
import com.unity3d.ads.properties.SdkProperties;


public class UnityAdsUtils {

    public static final String TAG_UnityAds = "UnityAds";

 // [Unity Ads - 2016-07-17] START
    final private String defaultGameId = "1096133";
    public static boolean isVideoPlaying = false;
	private static String interstitialPlacementId;
	private static String incentivizedPlacementId;
	private static boolean sInited = false;
	private static boolean isTestMode = true;
	private static AppActivity self = null;
	
    public void init(final Activity activity) {
        if (!sInited) {
        	self = (AppActivity)activity;
            final UnityAdsListener unityAdsListener = new UnityAdsListener();
    		UnityAds.setListener(unityAdsListener);
    		UnityAds.setDebugMode(true);
    		MediationMetaData mediationMetaData = new MediationMetaData(self);
    		mediationMetaData.setName("mediationPartner");
    		mediationMetaData.setVersion("v12345");
    		mediationMetaData.setOrdinal(1);
    		mediationMetaData.commit();

    		MetaData debugMetaData = new MetaData(self);
    		debugMetaData.set("test.debugOverlayEnabled", true);
    		debugMetaData.commit();
          	
    		String gameId = defaultGameId;
    		if (gameId.isEmpty()) {
    			Toast.makeText(self.getApplicationContext(), "Missing game id", Toast.LENGTH_SHORT).show();
    			return;
    		}
    		
    		UnityAds.initialize(self, gameId, unityAdsListener, isTestMode);
  		
    		// store entered gameid in app settings
  			SharedPreferences preferences = self.getSharedPreferences("Settings", self.MODE_PRIVATE);
  			SharedPreferences.Editor preferencesEdit = preferences.edit();
  			preferencesEdit.putString("gameId", gameId);
  			preferencesEdit.commit();
  	
            sInited = true;
        }
    }
 
    
    // [START Interstitial Ads]
    public void ShowNormalUnityAds()
    {
    	PlayerMetaData playerMetaData = new PlayerMetaData(self);
		playerMetaData.setServerId("rikshot");
		playerMetaData.commit();
		isVideoPlaying = true;
		UnityAds.show(self, interstitialPlacementId);
    }
    // [END Interstitial Ads]
    
    
    // [START Incentivized Ads]
    public void ShowRewardUnityAds()
    {
    	PlayerMetaData playerMetaData = new PlayerMetaData(self);
		playerMetaData.setServerId("rikshot");
		playerMetaData.commit();
		isVideoPlaying = true;
		UnityAds.show(self, incentivizedPlacementId);
    }
    // [END Incentivized Ads]
   
    
    // [START Resume]
	public void Resume() {

		if (SdkProperties.isInitialized()) {

			if (UnityAds.isReady(interstitialPlacementId)) {
//				enableButton((Button) self.findViewById(R.id.unityads_example_interstitial_button));
			}
			else {
//				disableButton((Button) self.findViewById(R.id.unityads_example_interstitial_button));
			}

			if (UnityAds.isReady(incentivizedPlacementId)) {
//				enableButton((Button) self.findViewById(R.id.unityads_example_incentivized_button));
			}
			else {
//				disableButton((Button) self.findViewById(R.id.unityads_example_incentivized_button));
			}
		}
	}
    // [END Resume]
	
    private void enableButton (Button btn) {
		btn.setEnabled(true);
		float alpha = 1f;
		AlphaAnimation alphaUp = new AlphaAnimation(alpha, alpha);
		alphaUp.setFillAfter(true);
		btn.startAnimation(alphaUp);
	}

    private void disableButton (Button btn) {
		float alpha = 0.45f;
		btn.setEnabled(false);
		AlphaAnimation alphaUp = new AlphaAnimation(alpha, alpha);
		alphaUp.setFillAfter(true);
		btn.startAnimation(alphaUp);
	}
	
    
    /* LISTENER */
	private class UnityAdsListener implements IUnityAdsListener {

		@Override
		public void onUnityAdsReady(final String zoneId) {
//			TextView statusText = (TextView) self.findViewById(R.id.unityads_example_statustext);
//			statusText.setText("");

			DeviceLog.debug("onUnityAdsReady: " + zoneId);
			Utilities.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					// look for various default placement ids over time
					switch (zoneId) {
						case "video":
						case "defaultZone":
						case "defaultVideoAndPictureZone":
							interstitialPlacementId = zoneId;
							toast("Ready", zoneId);
							Log.d(TAG_UnityAds, "Unity Ads Ready"+ zoneId);
							AppActivity.JAVA_NormalUnityAdsReady();
							break;

						case "rewardedVideo":
						case "rewardedVideoZone":
						case "incentivizedZone":
							incentivizedPlacementId = zoneId;
							toast("Ready", zoneId);
							Log.d(TAG_UnityAds, "Unity Ads Ready"+ zoneId);
							AppActivity.JAVA_RewardUnityAdsReady();
							break;
					}
				}
			});
		}

		@Override
		public void onUnityAdsStart(String zoneId) {
			DeviceLog.debug("onUnityAdsStart: " + zoneId);
			toast("Start", zoneId);
			Log.d(TAG_UnityAds, "Unity Ads Start");
			AppActivity.JAVA_UnityAdsStart();
		}

		@Override
		public void onUnityAdsFinish(String zoneId, UnityAds.FinishState result) {
			isVideoPlaying = false;
			DeviceLog.debug("onUnityAdsFinish: " + zoneId + " - " + result);
			toast("Finish", zoneId + " " + result);
			Log.d(TAG_UnityAds, "Unity Ads Finish");
			AppActivity.JAVA_UnityAdsFinish();
		}
 
		@Override
		public void onUnityAdsError(UnityAds.UnityAdsError error, String message) {
			DeviceLog.debug("onUnityAdsError: " + error + " - " + message);
			toast("Error", error + " " + message);
			statusText.setText(error + " - " + message);
			Log.d(TAG_UnityAds, "Unity Ads Error");
			AppActivity.JAVA_UnityAdsError();
		}

		private void toast(String callback, String msg) {
			Toast.makeText(self.getApplicationContext(), callback + ": " + msg, Toast.LENGTH_SHORT).show();
		}
	}
	// [Unity Ads - 2016-07-17] END
}