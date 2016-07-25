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
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.view.View;


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

import java.io.IOException;
import java.math.BigInteger;
import java.util.Random;

public class GoogleUtils implements 
GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    public static final String TAG = "SavedGames";
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
	
    private static boolean sInited = false;
    
    private static AppActivity self = null;
    
    // [Google Cloud - 2016-07-14] START
    // [START init]
    
    public void init(final Activity activity) {
        if (!sInited) {
        	self = (AppActivity)activity;
        	
        	mGoogleApiClient = new GoogleApiClient.Builder(self)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                    .addApi(Drive.API).addScope(Drive.SCOPE_APPFOLDER)
                    .build();
        	
        	beginUserInitiatedSignIn();
            sInited = true;
        }
    }
    // [END init]
    
	 
    // [START show_saved_games_ui]
/*    private static final int RC_SAVED_GAMES = 9009;

    public void ShowSavedGamesUI() {
        int maxNumberOfSavedGamesToShow = 5;
        Intent savedGamesIntent = Games.Snapshots.getSelectSnapshotIntent(mGoogleApiClient,
                "See My Saves", true, true, maxNumberOfSavedGamesToShow);
        self.startActivityForResult(savedGamesIntent, RC_SAVED_GAMES);
    }*/
    // [END show_saved_games_ui]


    // [START on_activity_result]
    // private String mCurrentSaveName = "snapshotTemp";

    /**
     * This callback will be triggered after you call startActivityForResult from the
     * ShowSavedGamesUI method.
     */
    /*public void ActivityResult(int requestCode, int resultCode, Intent intent) {
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
    }*/
    // [END on_activity_result]
    

    // [START load_from_snapshot]
    private byte[] mSaveGameData;

    public void GoogleCloudLoad(final String key) {
        // Display a progress dialog
        // ...
    	showProgressDialog("loading...");
    	
        AsyncTask<Void, Void, Integer> task = new AsyncTask<Void, Void, Integer>() {
            @Override
            protected Integer doInBackground(Void... params) {
                // Open the saved game using its name.
                Snapshots.OpenSnapshotResult result = Games.Snapshots.open(mGoogleApiClient,
                		key, true).await();

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
            	Toast.makeText(self.getBaseContext(), loadData, Toast.LENGTH_LONG).show();
            	dismissProgressDialog();
            	Log.d(TAG, "onPostExecute" + loadData);
            	
            	AppActivity.JAVA_GoogleCloudLoad(loadData);
            }
        };

        task.execute();
    }
    // [END load_from_snapshot]

    
    // [START send]
    public void GoogleCloudSave(final String key, final String value) {
        if(value == null) {
            return;
        }
        
        Log.d(TAG, "send Data : " + value);
        
        if(mGoogleApiClient.isConnected()) {
             AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task = 
                     new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
                 @Override
                 protected Snapshots.OpenSnapshotResult doInBackground(Void... params) {
                     return Games.Snapshots.open(mGoogleApiClient,
                    		 key, true).await();
                 }
                 
                 @Override
                 protected void onPostExecute(Snapshots.OpenSnapshotResult result) {
                     try{
                         Snapshot toWrite = processSnapshotOpenResult(result, 0);
                         if(toWrite!=null) {
                             toWrite.getSnapshotContents().writeBytes(value.getBytes());
                             SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
                                     .setDescription(key)
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
    
    
    /*private PendingResult<Snapshots.CommitSnapshotResult> writeSnapshot(Snapshot snapshot,
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
    }*/
    
    
    // [START process_snapshot_open_result]
    private static final int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;

    /**
     * Conflict resolution for when Snapshots are opened.  Must be run in an AsyncTask or in a
     * background thread,
     */
    Snapshot processSnapshotOpenResult(Snapshots.OpenSnapshotResult result, int retryCount) {
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
                Toast.makeText(self.getBaseContext(), message, Toast.LENGTH_LONG).show();
            }

        }

        // Fail, return null.
        return null;
    }
    // [END process_snapshot_open_result]

    @Override
    public void onConnected(Bundle connectionHint) {
        Log.d(TAG, "onConnected");
        AppActivity.JAVA_GoogleConnectionResult(true);
    }

    @Override
    public void onConnectionSuspended(int i) {
        Log.d(TAG, "onConnectionSuspended: " + i);
        mGoogleApiClient.connect();
        AppActivity.JAVA_GoogleConnectionSuspended();
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
            mIsResolving = BaseGameUtils.resolveConnectionFailure(self, mGoogleApiClient,
                    connectionResult, RC_SIGN_IN, self.getString(R.string.signin_other_error));
        }

        AppActivity.JAVA_GoogleConnectionResult(false);
    }
    
  	
    // [START beginUserInitiatedSignIn]
    /**
     * Start the sign-in process after the user clicks the sign-in button.
     */
    private void beginUserInitiatedSignIn() {
    	if(!isSignedIn()){
        	
            Log.d(TAG, "beginUserInitiatedSignIn");
            // Check to see the developer who's running this sample code read the instructions :-)
            // NOTE: this check is here only because this is a sample! Don't include this
            // check in your actual production app.
            if (!BaseGameUtils.verifySampleSetup(self, R.string.app_id)) {
                Log.w(TAG, "*** Warning: setup problems detected. Sign in may not work!");
            }

            mSignInClicked = true;
            mGoogleApiClient.connect();
        }
    }
    // [END beginUserInitiatedSignIn]
    
    
    /**
     * Determine if the Google API Client is signed in and ready to access Games APIs.
     * @return true if client exits and is signed in, false otherwise.
     */
    public boolean isSignedIn() {
        return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
    }
    
    
    
    /**
     * Show a progress dialog for asynchronous operations.
     * @param msg the message to display.
     */
    private void showProgressDialog(String msg) {
        if (mProgressDialog == null) {
            mProgressDialog = new ProgressDialog(self);
            mProgressDialog.setIndeterminate(true);
        }

        mProgressDialog.setMessage(msg);
        mProgressDialog.show();
    }
    
    /**
     * Hide the progress dialog, if it was showing.
     */
    private void dismissProgressDialog() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
    }
    
    // [Google Cloud - 2016-07-14] END
}