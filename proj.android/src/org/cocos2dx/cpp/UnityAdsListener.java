package org.cocos2dx.cpp;

import android.util.Log;

import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds;
import com.unity3d.ads.log.DeviceLog;

/**
 * Created by solomonli on 7/8/16.
 */
public class UnityAdsListener implements IUnityAdsListener {

    public static final String TAG = "UnityAds";
	
    @Override
    public void onUnityAdsReady(final String placementId) {
        DeviceLog.debug("[UnityAds] onUnityAdsReady for placement: " + placementId);
        Log.d(TAG, "[UnityAds] onUnityAdsReady for placement: " + placementId);
    }

    @Override
    public void onUnityAdsStart(String placementId) {
        DeviceLog.debug("[UnityAds] onUnityAdsStart for placement: " + placementId);
        Log.d(TAG, "[UnityAds] onUnityAdsStart for placement: " + placementId);
    }

    @Override
    public void onUnityAdsFinish(String placementId, UnityAds.FinishState result) {
        DeviceLog.debug("[UnityAds] onUnityAdsFinish with FinishState: " + result.name() + " for placement: " + placementId);
        Log.d(TAG, "[UnityAds] onUnityAdsFinish with FinishState: " + result.name() + " for placement: " + placementId);
        UnityAdsJNI.reward(placementId);
    }

    @Override
    public void onUnityAdsError(UnityAds.UnityAdsError error, String message) {
        DeviceLog.debug("[UnityAds] onUnityAdsError with message: " + message);
        Log.d(TAG, "[UnityAds] onUnityAdsError with message: " + message);
    }
}
