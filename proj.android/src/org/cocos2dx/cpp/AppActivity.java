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

import kr.HongSeongHee.StarStarStar.R;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.view.View;
import android.view.animation.AlphaAnimation;


import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.snapshot.Snapshot;
import com.google.android.gms.games.snapshot.SnapshotMetadata;
import com.google.android.gms.games.snapshot.SnapshotMetadataChange;
import com.google.android.gms.games.snapshot.Snapshots;


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

import java.io.IOException;
import java.math.BigInteger;
import java.util.Random;

public class AppActivity extends Cocos2dxActivity{
	
	private static UnityAdsUtils m_UnityAdsUtils = null;
	private static GoogleUtils m_GoogleUtils = null;
	
	 // [START on_create]
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	this.m_UnityAdsUtils = new UnityAdsUtils();
    	this.m_GoogleUtils = new GoogleUtils();
    	this.m_UnityAdsUtils.init(this);
    	this.m_GoogleUtils.init(this);
    }
    
    @Override
   	protected void onResume() {
   		super.onResume();
   		this.m_UnityAdsUtils.Resume();
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
    public native static void JAVA_GoogleCloudLoad(final String value);
    
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


/* public class AppActivity extends Cocos2dxActivity implements View.OnClickListener,
GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    public static final String TAG = "SavedGames";
    public static final String TAG_UnityAds = "UnityAds";
    // Request code used to invoke sign-in UI.
    private static final int RC_SIGN_IN = 9001;
    
    // True immediately after the user clicks the sign-in button/
    private boolean mSignInClicked = false;

    // Progress Dialog used to display loading messages.
    private ProgressDialog mProgressDialog;

    // True when the application is attempting to resolve a sign-in error that has a possible
    // resolution,
    private boolean mIsResolving = false;
    
    // True if we want to automatically attempt to sign in the user at application start.
    private boolean mAutoStartSignIn = true;

    private GoogleApiClient mGoogleApiClient;

    // [Unity Ads]
    final private String defaultGameId = "1096133";
	private String interstitialPlacementId;
	private String incentivizedPlacementId;
	
    // [Google Cloud - 2016-07-14] START
    // [START on_create]
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        
        // Create the Google API Client with access to Plus, Games, and Drive
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .addApi(Drive.API).addScope(Drive.SCOPE_APPFOLDER)
                .build();
        
        
        
        findViewById(R.id.button_sign_in).setOnClickListener(this);

        findViewById(R.id.button_cloud_save_load).setOnClickListener(this);
        findViewById(R.id.button_cloud_save_update).setOnClickListener(this);
        findViewById(R.id.button_saved_games_select).setOnClickListener(this);
        
        
     // [Unity Ads]
        Log.d(TAG_UnityAds, "OnCreate1");
        final AppActivity self = this;
        public static final UnityAdsListener unityAdsListener = new UnityAdsListener();
        Log.d(TAG_UnityAds, "OnCreate2");
		UnityAds.setListener(unityAdsListener);
		UnityAds.setDebugMode(true);
        Log.d(TAG_UnityAds, "OnCreate3");
		MediationMetaData mediationMetaData = new MediationMetaData(this);
		mediationMetaData.setName("mediationPartner");
		mediationMetaData.setVersion("v12345");
		mediationMetaData.setOrdinal(1);
		mediationMetaData.commit();

		MetaData debugMetaData = new MetaData(this);
		debugMetaData.set("test.debugOverlayEnabled", true);
		debugMetaData.commit();

		final Button interstitialButton = (Button) findViewById(R.id.unityads_example_interstitial_button);
		disableButton(interstitialButton);
		interstitialButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				disableButton(interstitialButton);

				PlayerMetaData playerMetaData = new PlayerMetaData(self);
				playerMetaData.setServerId("rikshot");
				playerMetaData.commit();

				UnityAds.show(self, interstitialPlacementId);
			}
		});

		final Button incentivizedButton = (Button) findViewById(R.id.unityads_example_incentivized_button);
		disableButton(incentivizedButton);
		incentivizedButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				disableButton(incentivizedButton);

				PlayerMetaData playerMetaData = new PlayerMetaData(self);
				playerMetaData.setServerId("rikshot");
				playerMetaData.commit();

				UnityAds.show(self, incentivizedPlacementId);
			}
		});

		final Button initializeButton = (Button) findViewById(R.id.unityads_example_initialize_button);
		final EditText gameIdEdit = (EditText) findViewById(R.id.unityads_example_gameid_edit);
		final CheckBox testModeCheckbox = (CheckBox) findViewById(R.id.unityads_example_testmode_checkbox);
		final TextView statusText = (TextView) findViewById(R.id.unityads_example_statustext);

		SharedPreferences preferences = getSharedPreferences("Settings", MODE_PRIVATE);
		gameIdEdit.setText(preferences.getString("gameId", defaultGameId));

		initializeButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				String gameId = gameIdEdit.getText().toString();
				if (gameId.isEmpty()) {
					Toast.makeText(getApplicationContext(), "Missing game id", Toast.LENGTH_SHORT).show();
					return;
				}

				disableButton(initializeButton);
				gameIdEdit.setEnabled(false);
				testModeCheckbox.setEnabled(false);

				statusText.setText("Initializing...");
				UnityAds.initialize(self, gameId, unityAdsListener, testModeCheckbox.isChecked());

				// store entered gameid in app settings
				SharedPreferences preferences = getSharedPreferences("Settings", MODE_PRIVATE);
				SharedPreferences.Editor preferencesEdit = preferences.edit();
				preferencesEdit.putString("gameId", gameId);
				preferencesEdit.commit();
			}
		});

		LinearLayout layout = (LinearLayout)findViewById(R.id.unityads_example_button_container);

		if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			layout.setOrientation(LinearLayout.HORIZONTAL);

		}
		else {
			layout.setOrientation(LinearLayout.VERTICAL);
		}    
    }
    // [END on_create]

    // [START onResume]
    @Override
	protected void onResume() {
		super.onResume();

		if (SdkProperties.isInitialized()) {
			disableButton((Button) findViewById(R.id.unityads_example_initialize_button));

			if (UnityAds.isReady(interstitialPlacementId)) {
				enableButton((Button) findViewById(R.id.unityads_example_interstitial_button));
			}
			else {
				disableButton((Button) findViewById(R.id.unityads_example_interstitial_button));
			}

			if (UnityAds.isReady(incentivizedPlacementId)) {
				enableButton((Button) findViewById(R.id.unityads_example_incentivized_button));
			}
			else {
				disableButton((Button) findViewById(R.id.unityads_example_incentivized_button));
			}
		}
	}
    // [END onResume]
    
    
    // [START onClick]
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.button_sign_in:
                beginUserInitiatedSignIn();
                break;
            case R.id.button_cloud_save_load:
            	loadFromSnapshot();
                break;
            case R.id.button_cloud_save_update:
            	Log.d(TAG,"send Data Button : " + getData());
            	send(getData().getBytes());
            	break;
            case R.id.button_saved_games_select:
            	showSavedGamesUI();
                break;
        }
    }
    // [END onClick]
    
    
    // [START onConfigurationChanged]
    @Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);

		LinearLayout layout = (LinearLayout)findViewById(R.id.unityads_example_button_container);

		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			layout.setOrientation(LinearLayout.HORIZONTAL);

		}
		else {
			layout.setOrientation(LinearLayout.VERTICAL);
		}
	}
    // [END onConfigurationChanged]
    
    
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

	 
    // [START show_saved_games_ui]
    private static final int RC_SAVED_GAMES = 9009;

    private void showSavedGamesUI() {
        int maxNumberOfSavedGamesToShow = 5;
        Intent savedGamesIntent = Games.Snapshots.getSelectSnapshotIntent(mGoogleApiClient,
                "See My Saves", true, true, maxNumberOfSavedGamesToShow);
        startActivityForResult(savedGamesIntent, RC_SAVED_GAMES);
    }
    // [END show_saved_games_ui]


    // [START on_activity_result]
    private String mCurrentSaveName = "snapshotTemp";
*/
    /**
     * This callback will be triggered after you call startActivityForResult from the
     * showSavedGamesUI method.
     */
/*    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent intent) {
        if (intent != null) {
            if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_METADATA)) {
                // Load a snapshot.
                SnapshotMetadata snapshotMetadata = (SnapshotMetadata)
                        intent.getParcelableExtra(Snapshots.EXTRA_SNAPSHOT_METADATA);
                mCurrentSaveName = snapshotMetadata.getUniqueName();

                // Load the game data from the Snapshot
                // ...
            } else if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_NEW)) {
                // Create a new snapshot named with a unique string
                String unique = new BigInteger(281, new Random()).toString(13);
                mCurrentSaveName = "snapshotTemp-" + unique;

                // Create the new snapshot
                // ...
            }
        }
    }
    // [END on_activity_result]


    // [START write_snapshot]
    private PendingResult<Snapshots.CommitSnapshotResult> writeSnapshot(Snapshot snapshot,
            byte[] data, Bitmap coverImage, String desc) {

        // Set the data payload for the snapshot
        snapshot.getSnapshotContents().writeBytes(data);

        // Create the change operation
        SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
                .setCoverImage(coverImage)
                .setDescription(desc)
                .build();

        // Commit the operation
        return Games.Snapshots.commitAndClose(mGoogleApiClient, snapshot, metadataChange);
    }
    // [END write_snapshot]


    // [START load_from_snapshot]
    private byte[] mSaveGameData;

    void loadFromSnapshot() {
        // Display a progress dialog
        // ...
    	showProgressDialog("loading...");
    	
        AsyncTask<Void, Void, Integer> task = new AsyncTask<Void, Void, Integer>() {
            @Override
            protected Integer doInBackground(Void... params) {
                // Open the saved game using its name.
                Snapshots.OpenSnapshotResult result = Games.Snapshots.open(mGoogleApiClient,
                        mCurrentSaveName, true).await();

                // Check the result of the open operation
                if (result.getStatus().isSuccess()) {
                    Snapshot snapshot = result.getSnapshot();
                    // Read the byte content of the saved game.
                    try {
                        mSaveGameData = snapshot.getSnapshotContents().readFully();
                    } catch (IOException e) {
                        Log.e(TAG, "Error while reading Snapshot.", e);
                    }
                } else{
                    Log.e(TAG, "Error while loading: " + result.getStatus().getStatusCode());
                }

                return result.getStatus().getStatusCode();
            }

            @Override
            protected void onPostExecute(Integer status) {
                // Dismiss progress dialog and reflect the changes in the UI.
                // ...
            	
            	String loadData = new String(mSaveGameData);
            	Toast.makeText(getBaseContext(), loadData, Toast.LENGTH_LONG).show();
            	dismissProgressDialog();
            	updateUI();
            	Log.d(TAG, "onPostExecute" + loadData);
            }
        };

        task.execute();
    }
    // [END load_from_snapshot]

    
    // [START send]
    public void send(final byte[] data) {
        if(data==null) {
            return;
        }
        
        Log.d(TAG, "send Data : " + data.toString());
        
        if(mGoogleApiClient.isConnected()) {
             AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task = 
                     new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
                 @Override
                 protected Snapshots.OpenSnapshotResult doInBackground(Void... params) {
                     return Games.Snapshots.open(mGoogleApiClient,
                             mCurrentSaveName, true).await();
                 }
                 
                 @Override
                 protected void onPostExecute(Snapshots.OpenSnapshotResult result) {
                     try{
                         Snapshot toWrite = processSnapshotOpenResult(result, 0);
                         if(toWrite!=null) {
                             toWrite.getSnapshotContents().writeBytes(data);
                             SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
                                     .setDescription(mCurrentSaveName)
                                      .build();
                             Games.Snapshots.commitAndClose(mGoogleApiClient, toWrite, metadataChange);
                             
                         }
                     } catch(IllegalStateException e) {
                        
                     }
                 }
             };
                 
             task.execute();
        }        
    }
    // [END send]
    
    
    // [START process_snapshot_open_result]
    private static final int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;
*/
    /**
     * Conflict resolution for when Snapshots are opened.  Must be run in an AsyncTask or in a
     * background thread,
     */
/*    Snapshot processSnapshotOpenResult(Snapshots.OpenSnapshotResult result, int retryCount) {
        Snapshot mResolvedSnapshot = null;
        retryCount++;

        int status = result.getStatus().getStatusCode();
        Log.i(TAG, "Save Result status: " + status);

        if (status == GamesStatusCodes.STATUS_OK) {
            return result.getSnapshot();
        } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
            return result.getSnapshot();
        } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
            Snapshot snapshot = result.getSnapshot();
            Snapshot conflictSnapshot = result.getConflictingSnapshot();

            // Resolve between conflicts by selecting the newest of the conflicting snapshots.
            mResolvedSnapshot = snapshot;

            if (snapshot.getMetadata().getLastModifiedTimestamp() <
                    conflictSnapshot.getMetadata().getLastModifiedTimestamp()) {
                mResolvedSnapshot = conflictSnapshot;
            }

            Snapshots.OpenSnapshotResult resolveResult = Games.Snapshots.resolveConflict(
                    mGoogleApiClient, result.getConflictId(), mResolvedSnapshot).await();

            if (retryCount < MAX_SNAPSHOT_RESOLVE_RETRIES) {
                // Recursively attempt again
                return processSnapshotOpenResult(resolveResult, retryCount);
            } else {
                // Failed, log error and show Toast to the user
                String message = "Could not resolve snapshot conflicts";
                Log.e(TAG, message);
                Toast.makeText(getBaseContext(), message, Toast.LENGTH_LONG).show();
            }

        }

        // Fail, return null.
        return null;
    }
    // [END process_snapshot_open_result]

    @Override
    public void onConnected(Bundle connectionHint) {
        Log.d(TAG, "onConnected");
        dismissProgressDialog();
        updateUI();
    }

    @Override
    public void onConnectionSuspended(int i) {
        Log.d(TAG, "onConnectionSuspended: " + i);
        mGoogleApiClient.connect();
        updateUI();
    }

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        Log.d(TAG, "onConnectionFailed");
        if (mIsResolving) {
            // The application is attempting to resolve this connection failure already.
            Log.d(TAG, "onConnectionFailed: already resolving");
            return;
        }

        if (mSignInClicked || mAutoStartSignIn) {
            mSignInClicked = false;
            mAutoStartSignIn = false;

            // Attempt to resolve the connection failure.
            Log.d(TAG, "onConnectionFailed: begin resolution.");
            mIsResolving = BaseGameUtils.resolveConnectionFailure(this, mGoogleApiClient,
                    connectionResult, RC_SIGN_IN, getString(R.string.signin_other_error));
        }

        updateUI();
    }
    
*/ 	
    // [START beginUserInitiatedSignIn]
    /**
     * Start the sign-in process after the user clicks the sign-in button.
     */
/*    private void beginUserInitiatedSignIn() {
        Log.d(TAG, "beginUserInitiatedSignIn");
        // Check to see the developer who's running this sample code read the instructions :-)
        // NOTE: this check is here only because this is a sample! Don't include this
        // check in your actual production app.
        if (!BaseGameUtils.verifySampleSetup(this, R.string.app_id)) {
            Log.w(TAG, "*** Warning: setup problems detected. Sign in may not work!");
        }

        showProgressDialog("Signing in.");
        mSignInClicked = true;
        mGoogleApiClient.connect();
    }
    // [END beginUserInitiatedSignIn]
    
*/    
    // [START updateUI]
    /**
     * Display either the signed-in or signed-out view, depending on the user's state.
     */
/*    private void updateUI() {
        // Show signed in or signed out view
        if (isSignedIn()) {
        	Log.d(TAG, "isSignedIn true");
            findViewById(R.id.layout_signed_in).setVisibility(View.VISIBLE);
            findViewById(R.id.layout_signed_out).setVisibility(View.GONE);
            displayMessage(getString(R.string.message_signed_in), false);
        } else {
        	Log.d(TAG, "isSignedIn false");
            findViewById(R.id.layout_signed_in).setVisibility(View.GONE);
            findViewById(R.id.layout_signed_out).setVisibility(View.VISIBLE);
            displayMessage(getString(R.string.message_sign_in), false);
        }
    }
    // [END updateUI]
    
*/    
    /**
     * Determine if the Google API Client is signed in and ready to access Games APIs.
     * @return true if client exits and is signed in, false otherwise.
     */
/*    private boolean isSignedIn() {
        return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
    }
    
*/    
    /**
     * Display a status message for the last operation at the bottom of the screen.
     * @param msg the message to display.
     * @param error true if an error occurred, false otherwise.
     */
/*    private void displayMessage(String msg, boolean error) {
        // Set text
        TextView messageView = (TextView) findViewById(R.id.text_message);
        messageView.setText(msg);

        // Set text color
        if (error) {
            messageView.setTextColor(Color.RED);
        } else {
            messageView.setTextColor(Color.BLACK);
        }
    }
*/    
    /**
     * Show a progress dialog for asynchronous operations.
     * @param msg the message to display.
     */
/*    private void showProgressDialog(String msg) {
        if (mProgressDialog == null) {
            mProgressDialog = new ProgressDialog(this);
            mProgressDialog.setIndeterminate(true);
        }

        mProgressDialog.setMessage(msg);
        mProgressDialog.show();
    }
*/    
    /**
     * Hide the progress dialog, if it was showing.
     */
/*    private void dismissProgressDialog() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
    }
*/  
    /**
     * Get the data from the EditText.
     * @return the String in the EditText, or "" if empty.
     */
/*    private String getData() {
        EditText dataEditText = (EditText) findViewById(R.id.edit_game_data);
        return dataEditText.getText().toString();
    }
    // [Google Cloud - 2016-07-14] END
*/
    // [Unity Ads - 2016-07-17] START
    /* LISTENER */
/*	private class UnityAdsListener implements IUnityAdsListener {

		@Override
		public void onUnityAdsReady(final String zoneId) {
			TextView statusText = (TextView) findViewById(R.id.unityads_example_statustext);
			statusText.setText("");

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
							enableButton((Button) findViewById(R.id.unityads_example_interstitial_button));
							break;

						case "rewardedVideo":
						case "rewardedVideoZone":
						case "incentivizedZone":
							incentivizedPlacementId = zoneId;
							enableButton((Button) findViewById(R.id.unityads_example_incentivized_button));
							break;
					}
				}
			});

			toast("Ready", zoneId);
		}

		@Override
		public void onUnityAdsStart(String zoneId) {
			DeviceLog.debug("onUnityAdsStart: " + zoneId);
			toast("Start", zoneId);
		}

		@Override
		public void onUnityAdsFinish(String zoneId, UnityAds.FinishState result) {
			DeviceLog.debug("onUnityAdsFinish: " + zoneId + " - " + result);
			toast("Finish", zoneId + " " + result);
		}

		@Override
		public void onUnityAdsError(UnityAds.UnityAdsError error, String message) {
			DeviceLog.debug("onUnityAdsError: " + error + " - " + message);
			toast("Error", error + " " + message);

			TextView statusText = (TextView) findViewById(R.id.unityads_example_statustext);
			statusText.setText(error + " - " + message);
		}

		private void toast(String callback, String msg) {
			Toast.makeText(getApplicationContext(), callback + ": " + msg, Toast.LENGTH_SHORT).show();
		}
	}
	// [Unity Ads - 2016-07-17] END
}*/