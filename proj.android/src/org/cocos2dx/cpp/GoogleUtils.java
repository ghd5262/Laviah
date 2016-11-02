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
import android.content.Intent;
import android.net.ConnectivityManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;


import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.snapshot.Snapshot;
import com.google.android.gms.games.snapshot.SnapshotMetadata;
import com.google.android.gms.games.snapshot.SnapshotMetadataChange;
import com.google.android.gms.games.snapshot.Snapshots;

import java.io.IOException;
import java.util.Calendar;

public class GoogleUtils implements 
	GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

   private static final String TAG = "Google Saved Game";

   private static final int RC_SIGN_IN = 9001;
   private static final int RC_SAVE_SNAPSHOT = 9004;
   private static final int RC_LOAD_SNAPSHOT = 9005;

   // Client used to interact with Google APIs.
   private GoogleApiClient mGoogleApiClient;

   // Are we currently resolving a connection failure?
   private boolean mResolvingConnectionFailure = false;

   // Has the user clicked the sign-in button?
   private boolean mSignInClicked = false;

   // Set to true to automatically start the sign in flow when the Activity starts.
   // Set to false to require the user to click the button in order to sign in.
   private boolean mAutoStartSignInFlow = true;

   // Members related to the conflict resolution chooser of Snapshots.
   final static int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;

   private static AppActivity self = null;
   
   boolean sInited = false;
   
   public void init(final Activity activity) {
	   if (!sInited) {
	   		Log.d(TAG, "GoogleUtils Init Start");
	   	
	   		self = (AppActivity)activity;

	   		// Create the Google Api Client with access to Plus and Games
	   		mGoogleApiClient = new GoogleApiClient.Builder(self)
	               .addConnectionCallbacks(this)
	               .addOnConnectionFailedListener(this)
	               .addApi(Games.API).addScope(Games.SCOPE_GAMES)
	               .addScope(Drive.SCOPE_APPFOLDER)
	               .build();

	   		sInited = true;
	   		Log.d(TAG, "GoogleUtils Init End");
	   }
   }
   // [END init]
   
   public void GoogleLogin() {
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
           
           if(!BaseGameUtils.isNetWork(self)){
        	   AppActivity.JAVA_GoogleConnectionResult(false);
           }
       }
   }
   
   /**
    * You can capture the Snapshot selection intent in the onActivityResult method. The result
    * either indicates a new Snapshot was created (EXTRA_SNAPSHOT_NEW) or was selected.
    */
   public void ActivityResult(int requestCode, int resultCode, Intent intent) {
       if (requestCode == RC_SIGN_IN) {
           Log.d(TAG, "onActivityResult with requestCode == RC_SIGN_IN, responseCode="
                   + resultCode + ", intent=" + intent);
           mSignInClicked = false;
           mResolvingConnectionFailure = false;
           if (resultCode == Activity.RESULT_OK) {
               mGoogleApiClient.connect();
           } else {
               BaseGameUtils.showActivityResultError(self, requestCode, resultCode, R.string.signin_other_error);
           }
       }
   }
   
   @Override
   public void onConnected(Bundle connectionHint) {

       // Sign-in worked!
       log("Sign-in successful! Loading game state from cloud.");
       Toast.makeText(self.getBaseContext(), "GoogleLogin Succeed", Toast.LENGTH_LONG).show();
       AppActivity.JAVA_GoogleConnectionResult(true);
   }

   @Override
   public void onConnectionSuspended(int cause) {
	   log("onConnectionSuspended() called. Cause: " + cause);
       // onConnected will automatically be called when the client reconnects.
	   
	   mGoogleApiClient.connect();
	   Toast.makeText(self.getBaseContext(), "GoogleLogin Suspended", Toast.LENGTH_LONG).show();
	   AppActivity.JAVA_GoogleConnectionSuspended();
   }

   @Override
   public void onConnectionFailed(ConnectionResult connectionResult) {
	   log("onConnectionFailed() called, result: " + connectionResult);

       if (mResolvingConnectionFailure) {
           Log.d(TAG, "onConnectionFailed() ignoring connection failure; already resolving.");
           return;
       }

       if (mSignInClicked || mAutoStartSignInFlow) {
           mAutoStartSignInFlow = false;
           mSignInClicked = false;
           mResolvingConnectionFailure = BaseGameUtils
                   .resolveConnectionFailure(self, mGoogleApiClient,
                           connectionResult, RC_SIGN_IN, self.getString(R.string.signin_other_error));
       }
   }

   public boolean isSignedIn() {
	   return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
   }
   
   private byte[] mSaveGameData;
   /**
    * Loads a Snapshot from the user's synchronized storage.
    */
   public void loadFromSnapshot(final SnapshotMetadata snapshotMetadata, final String key) {
	   Log.d(TAG, "GoogleCloudLoad : " + key);
       AsyncTask<Void, Void, Integer> task = new AsyncTask<Void, Void, Integer>() {
           @Override
           protected Integer doInBackground(Void... params) {
                Snapshots.OpenSnapshotResult result;
               if (snapshotMetadata != null && snapshotMetadata.getUniqueName() != null) {
                   Log.i(TAG, "Opening snapshot by metadata: " + snapshotMetadata);
                   result = Games.Snapshots.open(mGoogleApiClient,snapshotMetadata).await();
               } else {
                   Log.i(TAG, "Opening snapshot by name: " + key);
                   result = Games.Snapshots.open(mGoogleApiClient, key, true).await();
               }

               int status = result.getStatus().getStatusCode();

               Snapshot snapshot = null;
               if (status == GamesStatusCodes.STATUS_OK) {
                   snapshot = result.getSnapshot();
               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {

                   // if there is a conflict  - then resolve it.
                   snapshot = processSnapshotOpenResult(RC_LOAD_SNAPSHOT, result, 0);

                   // if it resolved OK, change the status to Ok
                   if (snapshot != null) {
                       status = GamesStatusCodes.STATUS_OK;
                   }
                   else {
                       Log.w(TAG,"Conflict was not resolved automatically");
                   }
               } else {
                   Log.e(TAG, "Error while loading: " + status);
               }
               if (snapshot != null) {
                   try {
                	   log("data read ~~");
                	   mSaveGameData = snapshot.getSnapshotContents().readFully();
                	   log("data read ~~"+ mSaveGameData.toString());
                   } catch (IOException e) {
                       Log.e(TAG, "Error while reading snapshot contents: " + e.getMessage());
                   }
               }
               return status;
           }

           @Override
           protected void onPostExecute(Integer status) {
               Log.i(TAG, "Snapshot loaded: " + status);
               

               // Note that showing a toast is done here for debugging. Your application should
               // resolve the error appropriately to your app.
               if (status == GamesStatusCodes.STATUS_SNAPSHOT_NOT_FOUND) {
                   Log.i(TAG, "Error: Snapshot not found");
                   Toast.makeText(self.getBaseContext(), "Error: Snapshot not found",
                           Toast.LENGTH_SHORT).show();
               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
                   Log.i(TAG, "Error: Snapshot contents unavailable");
                   Toast.makeText(self.getBaseContext(), "Error: Snapshot contents unavailable",
                           Toast.LENGTH_SHORT).show();
               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_FOLDER_UNAVAILABLE) {
                   Log.i(TAG, "Error: Snapshot folder unavailable");
                   Toast.makeText(self.getBaseContext(), "Error: Snapshot folder unavailable.",
                           Toast.LENGTH_SHORT).show();
               }
               
               Log.d(TAG, "Data Load Status : " + status);
               if(status == GamesStatusCodes.STATUS_OK){
            	   String loadData = new String(mSaveGameData);
            	   Toast.makeText(self.getBaseContext(), key + " : " + loadData, Toast.LENGTH_LONG).show();
            	   Log.d(TAG, "onPostExecute" + loadData);
               	
            	   AppActivity.JAVA_GoogleCloudLoad(key, loadData);
               }
           }
       };

       task.execute();
   }

   /**
    * Conflict resolution for when Snapshots are opened.
    *
    * @param requestCode - the request currently being processed.  This is used to forward on the
    *                    information to another activity, or to send the result intent.
    * @param result The open snapshot result to resolve on open.
    * @param retryCount - the current iteration of the retry.  The first retry should be 0.
    * @return The opened Snapshot on success; otherwise, returns null.
    */
   Snapshot processSnapshotOpenResult(int requestCode, Snapshots.OpenSnapshotResult result,
           int retryCount) {

       retryCount++;
       int status = result.getStatus().getStatusCode();

       Log.i(TAG, "Save Result status: " + status);

       if (status == GamesStatusCodes.STATUS_OK) {
           return result.getSnapshot();
       } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
           return result.getSnapshot();
       } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
    	   return result.getSnapshot();
       }
       // Fail, return null.
       return null;
   }

   /**
    * Prepares saving Snapshot to the user's synchronized storage, conditionally resolves errors,
    * and stores the Snapshot.
    */
   void saveSnapshot(final SnapshotMetadata snapshotMetadata, final String key, final String value) {
       
       if(value == null) {
           return;
       }
	   
       Log.d(TAG, "send Data : " + key + " : " + value);
       
	   AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task =
               new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
                   @Override
                   protected Snapshots.OpenSnapshotResult doInBackground(Void... params) {
                       if (snapshotMetadata == null) {
                           Log.i(TAG, "Calling open with " + key);
                           return Games.Snapshots.open(mGoogleApiClient, key, true)
                                   .await();
                       }
                       else {
                           Log.i(TAG, "Calling open with " + snapshotMetadata);
                           return Games.Snapshots.open(mGoogleApiClient, snapshotMetadata)
                                   .await();
                       }
                   }

                   @Override
                   protected void onPostExecute(Snapshots.OpenSnapshotResult result) {
                       Snapshot toWrite = processSnapshotOpenResult(RC_SAVE_SNAPSHOT, result, 0);
                       if (toWrite != null) {
                    	   toWrite.getSnapshotContents().writeBytes(value.getBytes());
                           Log.i(TAG, writeSnapshot(toWrite));
                           AppActivity.CPP_Toast("Save Finished");
                       }
                       else {
                           Log.e(TAG, "Error opening snapshot: " + result.toString());
                       }
                   }
               };

       task.execute();
   }
   

   /**
    * Generates metadata, takes a screenshot, and performs the write operation for saving a
    * snapshot.
    */
   private String writeSnapshot(Snapshot snapshot) {
       // Save the snapshot.
       SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
               .setDescription("Modified data at: " + Calendar.getInstance().getTime())
               .build();
       Games.Snapshots.commitAndClose(mGoogleApiClient, snapshot, metadataChange);
       return snapshot.toString();
   }


   /** Prints a log message (convenience method). */
   void log(String message) {
       Log.d(TAG, message);
   }
}