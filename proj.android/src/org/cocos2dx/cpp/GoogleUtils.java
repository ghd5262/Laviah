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
import java.util.ArrayList;
import java.util.Calendar;

public class GoogleUtils extends Activity implements 
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

	private String currentSaveName = "snapshotTemp";

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
	       
	       if(!isNetWork()){
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
	   // loading a snapshot into the game.
	   else if (requestCode == RC_LOAD_SNAPSHOT) {
	       Log.d(TAG,"Loading a snapshot resultCode = " + resultCode);
	       if (resultCode == Activity.RESULT_OK) {
	           if (intent != null && intent.hasExtra(SelectSnapshotActivity.SNAPSHOT_METADATA)) {
	               // Load a snapshot.
	               String conflictId = intent.getStringExtra(SelectSnapshotActivity.CONFLICT_ID);
	               int retryCount = intent.getIntExtra(SelectSnapshotActivity.RETRY_COUNT,
	                       MAX_SNAPSHOT_RESOLVE_RETRIES);
	               SnapshotMetadata snapshotMetadata =
	                       intent.getParcelableExtra(SelectSnapshotActivity.SNAPSHOT_METADATA);
	               if (conflictId == null) {
	                   loadFromSnapshot(snapshotMetadata, "");
	               } else {
	                   Log.d(TAG,"resolving " + snapshotMetadata);
	                   resolveSnapshotConflict(requestCode, conflictId, retryCount,
	                           snapshotMetadata);
	               }
	           }
	       }

	   }
	   // saving the game into a snapshot.
	   else if (requestCode == RC_SAVE_SNAPSHOT) {
	       if (resultCode == Activity.RESULT_OK) {
	           if (intent != null && intent.hasExtra(SelectSnapshotActivity.SNAPSHOT_METADATA)) {
	               // Load a snapshot.
	               String conflictId = intent.getStringExtra(SelectSnapshotActivity.CONFLICT_ID);
	               int retryCount = intent.getIntExtra(SelectSnapshotActivity.RETRY_COUNT,
	                       MAX_SNAPSHOT_RESOLVE_RETRIES);
	               SnapshotMetadata snapshotMetadata =
	                       intent.getParcelableExtra(SelectSnapshotActivity.SNAPSHOT_METADATA);
	               if (conflictId == null) {
	                   saveSnapshot(snapshotMetadata, "", "");
	               } else {
	                   Log.d(TAG,"resolving " + snapshotMetadata);
	                   resolveSnapshotConflict(requestCode, conflictId, retryCount, snapshotMetadata);
	               }
	           }
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
	       final Snapshot snapshot = result.getSnapshot();
	       final Snapshot conflictSnapshot = result.getConflictingSnapshot();

	       ArrayList<Snapshot> snapshotList = new ArrayList<Snapshot>(2);
	       snapshotList.add(snapshot);
	       snapshotList.add(conflictSnapshot);

	       selectSnapshotItem(requestCode, snapshotList, result.getConflictId(), retryCount);
	       // display both to the user and allow them to select on
	   }
	   // Fail, return null.
	   return null;
	}

	/**
	*  Handles resolving the snapshot conflict asynchronously.
	*
	* @param requestCode - the request currently being processed.  This is used to forward on the
	*                    information to another activity, or to send the result intent.
	* @param conflictId - the id of the conflict being resolved.
	* @param retryCount - the current iteration of the retry.  The first retry should be 0.
	* @param snapshotMetadata - the metadata of the snapshot that is selected to resolve the conflict.
	*/
	private void resolveSnapshotConflict(final int requestCode, final String conflictId,
	       final int retryCount,
	       final SnapshotMetadata snapshotMetadata) {

	   Log.i(TAG,"Resolving conflict retry count = " + retryCount + " conflictid = " + conflictId);
	   AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task =
	           new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
	               @Override
	               protected Snapshots.OpenSnapshotResult doInBackground(Void... voids) {

	                   Snapshots.OpenSnapshotResult result;
	                   if (snapshotMetadata.getUniqueName() != null) {
	                       Log.d(TAG,"Opening unique name " + snapshotMetadata.getUniqueName());
	                       result = Games.Snapshots.open(mGoogleApiClient, snapshotMetadata)
	                               .await();
	                   }
	                   else {
	                       Log.d(TAG,"Opening current save name " + currentSaveName);
	                       result = Games.Snapshots.open(mGoogleApiClient, currentSaveName, true)
	                               .await();
	                   }

	                   Log.d(TAG,"opening from metadata - result is " + result.getStatus() +
	                           " snapshot is " + result.getSnapshot());

	                  return Games.Snapshots
	                           .resolveConflict(mGoogleApiClient, conflictId, result.getSnapshot())
	                           .await();
	               }

	               @Override
	               protected void onPostExecute(Snapshots.OpenSnapshotResult openSnapshotResult) {
	                   Snapshot snapshot = processSnapshotOpenResult(requestCode,
	                           openSnapshotResult,
	                           retryCount);
	                   Log.d(TAG,"resolved snapshot conflict - snapshot is " + snapshot);
	                   // if there is a snapshot returned, then pass it along to onActivityResult.
	                   // otherwise, another activity will be used to resolve the conflict so we
	                   // don't need to do anything here.
	                   if (snapshot != null) {
	                       Intent intent = new Intent("");
	                       intent.putExtra(SelectSnapshotActivity.SNAPSHOT_METADATA,
	                               snapshot.getMetadata().freeze());
	                       ActivityResult(requestCode, Activity.RESULT_OK, intent);
	                   }
	               }
	           };

	   task.execute();
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


	private void selectSnapshotItem(int requestCode, ArrayList<Snapshot> items,
	       String conflictId, int retryCount) {

	   ArrayList<SnapshotMetadata> snapshotList = new ArrayList<SnapshotMetadata>(items.size());
	   for (Snapshot m : items) {
	       snapshotList.add(m.getMetadata().freeze());
	   }
	   Intent intent = new Intent(self, SelectSnapshotActivity.class);
	   intent.putParcelableArrayListExtra(SelectSnapshotActivity.SNAPSHOT_METADATA_LIST,
	           snapshotList);

	   intent.putExtra(SelectSnapshotActivity.CONFLICT_ID, conflictId);
	   intent.putExtra(SelectSnapshotActivity.RETRY_COUNT, retryCount);

	   Log.d(TAG, "Starting activity to select snapshot");
	   self.startActivityForResult(intent, requestCode);
	}

	private void selectSnapshotItem(int requestCode, ArrayList<SnapshotMetadata> items) {

	   ArrayList<SnapshotMetadata> metadataArrayList =
	           new ArrayList<SnapshotMetadata>(items.size());
	   for (SnapshotMetadata m : items) {
	       metadataArrayList.add(m.freeze());
	   }
	   Intent intent = new Intent(self, SelectSnapshotActivity.class);
	   intent.putParcelableArrayListExtra(SelectSnapshotActivity.SNAPSHOT_METADATA_LIST,
	           metadataArrayList);

	   self.startActivityForResult(intent, requestCode);
	}

	private Boolean isNetWork(){
		ConnectivityManager manager = (ConnectivityManager) self.getSystemService (Context.CONNECTIVITY_SERVICE);
		boolean isMobileAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isAvailable();
		boolean isMobileConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isConnectedOrConnecting();
		boolean isWifiAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isAvailable();
		boolean isWifiConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnectedOrConnecting();

		if ((isWifiAvailable && isWifiConnect) || (isMobileAvailable && isMobileConnect)){
			Log.d(TAG, "Network is connected");
			return true;
		}else{
			Log.d(TAG, "Network is not connected");
			return false;
		}
	}
}

























//public class GoogleUtils implements 
//	GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {
//
//   private static final String TAG = "Google Saved Game";
//
//   private static final int RC_SIGN_IN = 9001;
//   private static final int RC_SAVE_SNAPSHOT = 9004;
//   private static final int RC_LOAD_SNAPSHOT = 9005;
//
//   // Client used to interact with Google APIs.
//   private GoogleApiClient mGoogleApiClient;
//
//   // Are we currently resolving a connection failure?
//   private boolean mResolvingConnectionFailure = false;
//
//   // Has the user clicked the sign-in button?
//   private boolean mSignInClicked = false;
//
//   // Set to true to automatically start the sign in flow when the Activity starts.
//   // Set to false to require the user to click the button in order to sign in.
//   private boolean mAutoStartSignInFlow = true;
//
//   private String currentSaveName = "snapshotTemp";
//
//   // Members related to the conflict resolution chooser of Snapshots.
//   final static int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;
//
//   private static AppActivity self = null;
//   
//   boolean sInited = false;
//   
//   public void init(final Activity activity) {
//	   if (!sInited) {
//	   		Log.d(TAG, "GoogleUtils Init Start");
//	   	
//	   		self = (AppActivity)activity;
//
//	   		// Create the Google Api Client with access to Plus and Games
//	   		mGoogleApiClient = new GoogleApiClient.Builder(self)
//	               .addConnectionCallbacks(this)
//	               .addOnConnectionFailedListener(this)
//	               .addApi(Games.API).addScope(Games.SCOPE_GAMES)
//	               .addScope(Drive.SCOPE_APPFOLDER)
//	               .build();
//
//	   		sInited = true;
//	   		Log.d(TAG, "GoogleUtils Init End");
//	   }
//   }
//   // [END init]
//   
//   public void GoogleLogin() {
//   	if(!isSignedIn()){
//       	
//           Log.d(TAG, "beginUserInitiatedSignIn");
//           // Check to see the developer who's running this sample code read the instructions :-)
//           // NOTE: this check is here only because this is a sample! Don't include this
//           // check in your actual production app.
//           if (!BaseGameUtils.verifySampleSetup(self, R.string.app_id)) {
//               Log.w(TAG, "*** Warning: setup problems detected. Sign in may not work!");
//           }
//
//           mSignInClicked = true;
//           mGoogleApiClient.connect();
//           
//           if(!isNetWork()){
//        	   AppActivity.JAVA_GoogleConnectionResult(false);
//           }
//       }
//   }
//   
//   /**
//    * You can capture the Snapshot selection intent in the onActivityResult method. The result
//    * either indicates a new Snapshot was created (EXTRA_SNAPSHOT_NEW) or was selected.
//    */
//   public void ActivityResult(int requestCode, int resultCode, Intent intent) {
//       if (requestCode == RC_SIGN_IN) {
//           Log.d(TAG, "onActivityResult with requestCode == RC_SIGN_IN, responseCode="
//                   + resultCode + ", intent=" + intent);
//           mSignInClicked = false;
//           mResolvingConnectionFailure = false;
//           if (resultCode == Activity.RESULT_OK) {
//               mGoogleApiClient.connect();
//           } else {
//               BaseGameUtils.showActivityResultError(self, requestCode, resultCode, R.string.signin_other_error);
//           }
//       }
//       // loading a snapshot into the game.
//       else if (requestCode == RC_LOAD_SNAPSHOT) {
//           Log.d(TAG,"Loading a snapshot resultCode = " + resultCode);
//           if (resultCode == Activity.RESULT_OK) {
//               if (intent != null && intent.hasExtra(SelectSnapshotActivity.SNAPSHOT_METADATA)) {
//                   // Load a snapshot.
//                   String conflictId = intent.getStringExtra(SelectSnapshotActivity.CONFLICT_ID);
//                   int retryCount = intent.getIntExtra(SelectSnapshotActivity.RETRY_COUNT,
//                           MAX_SNAPSHOT_RESOLVE_RETRIES);
//                   SnapshotMetadata snapshotMetadata =
//                           intent.getParcelableExtra(SelectSnapshotActivity.SNAPSHOT_METADATA);
//                   if (conflictId == null) {
//                       loadFromSnapshot(snapshotMetadata, "");
//                   } else {
//                       Log.d(TAG,"resolving " + snapshotMetadata);
//                       resolveSnapshotConflict(requestCode, conflictId, retryCount,
//                               snapshotMetadata);
//                   }
//               }
//           }
//
//       }
//       // saving the game into a snapshot.
//       else if (requestCode == RC_SAVE_SNAPSHOT) {
//           if (resultCode == Activity.RESULT_OK) {
//               if (intent != null && intent.hasExtra(SelectSnapshotActivity.SNAPSHOT_METADATA)) {
//                   // Load a snapshot.
//                   String conflictId = intent.getStringExtra(SelectSnapshotActivity.CONFLICT_ID);
//                   int retryCount = intent.getIntExtra(SelectSnapshotActivity.RETRY_COUNT,
//                           MAX_SNAPSHOT_RESOLVE_RETRIES);
//                   SnapshotMetadata snapshotMetadata =
//                           intent.getParcelableExtra(SelectSnapshotActivity.SNAPSHOT_METADATA);
//                   if (conflictId == null) {
//                       saveSnapshot(snapshotMetadata, "", "");
//                   } else {
//                       Log.d(TAG,"resolving " + snapshotMetadata);
//                       resolveSnapshotConflict(requestCode, conflictId, retryCount, snapshotMetadata);
//                   }
//               }
//           }
//       }
//   }
//
//   @Override
//   public void onConnected(Bundle connectionHint) {
//
//       // Sign-in worked!
//       log("Sign-in successful! Loading game state from cloud.");
//       Toast.makeText(self.getBaseContext(), "GoogleLogin Succeed", Toast.LENGTH_LONG).show();
//       AppActivity.JAVA_GoogleConnectionResult(true);
//   }
//
//
//   @Override
//   public void onConnectionSuspended(int cause) {
//	   log("onConnectionSuspended() called. Cause: " + cause);
//       // onConnected will automatically be called when the client reconnects.
//	   
//	   mGoogleApiClient.connect();
//	   Toast.makeText(self.getBaseContext(), "GoogleLogin Suspended", Toast.LENGTH_LONG).show();
//	   AppActivity.JAVA_GoogleConnectionSuspended();
//   }
//
//   @Override
//   public void onConnectionFailed(ConnectionResult connectionResult) {
//	   log("onConnectionFailed() called, result: " + connectionResult);
//
//       if (mResolvingConnectionFailure) {
//           Log.d(TAG, "onConnectionFailed() ignoring connection failure; already resolving.");
//           return;
//       }
//
//       if (mSignInClicked || mAutoStartSignInFlow) {
//           mAutoStartSignInFlow = false;
//           mSignInClicked = false;
//           mResolvingConnectionFailure = BaseGameUtils
//                   .resolveConnectionFailure(self, mGoogleApiClient,
//                           connectionResult, RC_SIGN_IN, self.getString(R.string.signin_other_error));
//       }
//   }
//
//   public boolean isSignedIn() {
//	   return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
//   }
//   
//   private byte[] mSaveGameData;
//   /**
//    * Loads a Snapshot from the user's synchronized storage.
//    */
//   public void loadFromSnapshot(final SnapshotMetadata snapshotMetadata, final String key) {
//	   Log.d(TAG, "GoogleCloudLoad : " + key);
//       AsyncTask<Void, Void, Integer> task = new AsyncTask<Void, Void, Integer>() {
//           @Override
//           protected Integer doInBackground(Void... params) {
//                Snapshots.OpenSnapshotResult result;
//               if (snapshotMetadata != null && snapshotMetadata.getUniqueName() != null) {
//                   Log.i(TAG, "Opening snapshot by metadata: " + snapshotMetadata);
//                   result = Games.Snapshots.open(mGoogleApiClient,snapshotMetadata).await();
//               } else {
//                   Log.i(TAG, "Opening snapshot by name: " + key);
//                   result = Games.Snapshots.open(mGoogleApiClient, key, true).await();
//               }
//
//               int status = result.getStatus().getStatusCode();
//
//               Snapshot snapshot = null;
//               if (status == GamesStatusCodes.STATUS_OK) {
//                   snapshot = result.getSnapshot();
//               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
//
//                   // if there is a conflict  - then resolve it.
//                   snapshot = processSnapshotOpenResult(RC_LOAD_SNAPSHOT, result, 0);
//
//                   // if it resolved OK, change the status to Ok
//                   if (snapshot != null) {
//                       status = GamesStatusCodes.STATUS_OK;
//                   }
//                   else {
//                       Log.w(TAG,"Conflict was not resolved automatically");
//                   }
//               } else {
//                   Log.e(TAG, "Error while loading: " + status);
//               }
//               if (snapshot != null) {
//                   try {
//                	   log("data read ~~");
//                	   mSaveGameData = snapshot.getSnapshotContents().readFully();
//                	   log("data read ~~"+ mSaveGameData.toString());
//                   } catch (IOException e) {
//                       Log.e(TAG, "Error while reading snapshot contents: " + e.getMessage());
//                   }
//               }
//               return status;
//           }
//
//           @Override
//           protected void onPostExecute(Integer status) {
//               Log.i(TAG, "Snapshot loaded: " + status);
//               
//
//               // Note that showing a toast is done here for debugging. Your application should
//               // resolve the error appropriately to your app.
//               if (status == GamesStatusCodes.STATUS_SNAPSHOT_NOT_FOUND) {
//                   Log.i(TAG, "Error: Snapshot not found");
//                   Toast.makeText(self.getBaseContext(), "Error: Snapshot not found",
//                           Toast.LENGTH_SHORT).show();
//               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
//                   Log.i(TAG, "Error: Snapshot contents unavailable");
//                   Toast.makeText(self.getBaseContext(), "Error: Snapshot contents unavailable",
//                           Toast.LENGTH_SHORT).show();
//               } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_FOLDER_UNAVAILABLE) {
//                   Log.i(TAG, "Error: Snapshot folder unavailable");
//                   Toast.makeText(self.getBaseContext(), "Error: Snapshot folder unavailable.",
//                           Toast.LENGTH_SHORT).show();
//               }
//               
//               Log.d(TAG, "Data Load Status : " + status);
//               if(status == GamesStatusCodes.STATUS_OK){
//            	   String loadData = new String(mSaveGameData);
//            	   Toast.makeText(self.getBaseContext(), key + " : " + loadData, Toast.LENGTH_LONG).show();
//            	   Log.d(TAG, "onPostExecute" + loadData);
//               	
//            	   AppActivity.JAVA_GoogleCloudLoad(key, loadData);
//               }
//           }
//       };
//
//       task.execute();
//   }
//
//   /**
//    * Conflict resolution for when Snapshots are opened.
//    *
//    * @param requestCode - the request currently being processed.  This is used to forward on the
//    *                    information to another activity, or to send the result intent.
//    * @param result The open snapshot result to resolve on open.
//    * @param retryCount - the current iteration of the retry.  The first retry should be 0.
//    * @return The opened Snapshot on success; otherwise, returns null.
//    */
//   Snapshot processSnapshotOpenResult(int requestCode, Snapshots.OpenSnapshotResult result,
//           int retryCount) {
//
//       retryCount++;
//       int status = result.getStatus().getStatusCode();
//
//       Log.i(TAG, "Save Result status: " + status);
//
//       if (status == GamesStatusCodes.STATUS_OK) {
//           return result.getSnapshot();
//       } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
//           return result.getSnapshot();
//       } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
//           final Snapshot snapshot = result.getSnapshot();
//           final Snapshot conflictSnapshot = result.getConflictingSnapshot();
//
//           ArrayList<Snapshot> snapshotList = new ArrayList<Snapshot>(2);
//           snapshotList.add(snapshot);
//           snapshotList.add(conflictSnapshot);
//
//           selectSnapshotItem(requestCode, snapshotList, result.getConflictId(), retryCount);
//           // display both to the user and allow them to select on
//       }
//       // Fail, return null.
//       return null;
//   }
//
//   /**
//    *  Handles resolving the snapshot conflict asynchronously.
//    *
//    * @param requestCode - the request currently being processed.  This is used to forward on the
//    *                    information to another activity, or to send the result intent.
//    * @param conflictId - the id of the conflict being resolved.
//    * @param retryCount - the current iteration of the retry.  The first retry should be 0.
//    * @param snapshotMetadata - the metadata of the snapshot that is selected to resolve the conflict.
//    */
//   private void resolveSnapshotConflict(final int requestCode, final String conflictId,
//           final int retryCount,
//           final SnapshotMetadata snapshotMetadata) {
//
//       Log.i(TAG,"Resolving conflict retry count = " + retryCount + " conflictid = " + conflictId);
//       AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task =
//               new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
//                   @Override
//                   protected Snapshots.OpenSnapshotResult doInBackground(Void... voids) {
//
//                       Snapshots.OpenSnapshotResult result;
//                       if (snapshotMetadata.getUniqueName() != null) {
//                           Log.d(TAG,"Opening unique name " + snapshotMetadata.getUniqueName());
//                           result = Games.Snapshots.open(mGoogleApiClient, snapshotMetadata)
//                                   .await();
//                       }
//                       else {
//                           Log.d(TAG,"Opening current save name " + currentSaveName);
//                           result = Games.Snapshots.open(mGoogleApiClient, currentSaveName, true)
//                                   .await();
//                       }
//
//                       Log.d(TAG,"opening from metadata - result is " + result.getStatus() +
//                               " snapshot is " + result.getSnapshot());
//
//                      return Games.Snapshots
//                               .resolveConflict(mGoogleApiClient, conflictId, result.getSnapshot())
//                               .await();
//                   }
//
//                   @Override
//                   protected void onPostExecute(Snapshots.OpenSnapshotResult openSnapshotResult) {
//                       Snapshot snapshot = processSnapshotOpenResult(requestCode,
//                               openSnapshotResult,
//                               retryCount);
//                       Log.d(TAG,"resolved snapshot conflict - snapshot is " + snapshot);
//                       // if there is a snapshot returned, then pass it along to onActivityResult.
//                       // otherwise, another activity will be used to resolve the conflict so we
//                       // don't need to do anything here.
//                       if (snapshot != null) {
//                           Intent intent = new Intent("");
//                           intent.putExtra(SelectSnapshotActivity.SNAPSHOT_METADATA,
//                                   snapshot.getMetadata().freeze());
//                           ActivityResult(requestCode, Activity.RESULT_OK, intent);
//                       }
//                   }
//               };
//
//       task.execute();
//   }
//
//
//   /**
//    * Prepares saving Snapshot to the user's synchronized storage, conditionally resolves errors,
//    * and stores the Snapshot.
//    */
//   void saveSnapshot(final SnapshotMetadata snapshotMetadata, final String key, final String value) {
//       
//       if(value == null) {
//           return;
//       }
//	   
//       Log.d(TAG, "send Data : " + key + " : " + value);
//       
//	   AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task =
//               new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
//                   @Override
//                   protected Snapshots.OpenSnapshotResult doInBackground(Void... params) {
//                       if (snapshotMetadata == null) {
//                           Log.i(TAG, "Calling open with " + key);
//                           return Games.Snapshots.open(mGoogleApiClient, key, true)
//                                   .await();
//                       }
//                       else {
//                           Log.i(TAG, "Calling open with " + snapshotMetadata);
//                           return Games.Snapshots.open(mGoogleApiClient, snapshotMetadata)
//                                   .await();
//                       }
//                   }
//
//                   @Override
//                   protected void onPostExecute(Snapshots.OpenSnapshotResult result) {
//                       Snapshot toWrite = processSnapshotOpenResult(RC_SAVE_SNAPSHOT, result, 0);
//                       if (toWrite != null) {
//                    	   toWrite.getSnapshotContents().writeBytes(value.getBytes());
//                           Log.i(TAG, writeSnapshot(toWrite));
//                           AppActivity.CPP_Toast("Save Finished");
//                       }
//                       else {
//                           Log.e(TAG, "Error opening snapshot: " + result.toString());
//                       }
//                   }
//               };
//
//       task.execute();
//   }
//   
//
//   /**
//    * Generates metadata, takes a screenshot, and performs the write operation for saving a
//    * snapshot.
//    */
//   private String writeSnapshot(Snapshot snapshot) {
//       // Save the snapshot.
//       SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
//               .setDescription("Modified data at: " + Calendar.getInstance().getTime())
//               .build();
//       Games.Snapshots.commitAndClose(mGoogleApiClient, snapshot, metadataChange);
//       return snapshot.toString();
//   }
//
//
//   /** Prints a log message (convenience method). */
//   void log(String message) {
//       Log.d(TAG, message);
//   }
//
//
//   private void selectSnapshotItem(int requestCode, ArrayList<Snapshot> items,
//           String conflictId, int retryCount) {
//
//       ArrayList<SnapshotMetadata> snapshotList = new ArrayList<SnapshotMetadata>(items.size());
//       for (Snapshot m : items) {
//           snapshotList.add(m.getMetadata().freeze());
//       }
//       Intent intent = new Intent(self, SelectSnapshotActivity.class);
//       intent.putParcelableArrayListExtra(SelectSnapshotActivity.SNAPSHOT_METADATA_LIST,
//               snapshotList);
//
//       intent.putExtra(SelectSnapshotActivity.CONFLICT_ID, conflictId);
//       intent.putExtra(SelectSnapshotActivity.RETRY_COUNT, retryCount);
//
//       Log.d(TAG, "Starting activity to select snapshot");
//       self.startActivityForResult(intent, requestCode);
//   }
//
//   private void selectSnapshotItem(int requestCode, ArrayList<SnapshotMetadata> items) {
//
//       ArrayList<SnapshotMetadata> metadataArrayList =
//               new ArrayList<SnapshotMetadata>(items.size());
//       for (SnapshotMetadata m : items) {
//           metadataArrayList.add(m.freeze());
//       }
//       Intent intent = new Intent(self, SelectSnapshotActivity.class);
//       intent.putParcelableArrayListExtra(SelectSnapshotActivity.SNAPSHOT_METADATA_LIST,
//               metadataArrayList);
//
//       self.startActivityForResult(intent, requestCode);
//   }
//   
//   private Boolean isNetWork(){
//		ConnectivityManager manager = (ConnectivityManager) self.getSystemService (Context.CONNECTIVITY_SERVICE);
//		boolean isMobileAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isAvailable();
//		boolean isMobileConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isConnectedOrConnecting();
//		boolean isWifiAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isAvailable();
//		boolean isWifiConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnectedOrConnecting();
//
//		if ((isWifiAvailable && isWifiConnect) || (isMobileAvailable && isMobileConnect)){
//			Log.d(TAG, "Network is connected");
//			return true;
//		}else{
//			Log.d(TAG, "Network is not connected");
//			return false;
//		}
//	}
//}


















//
//public class GoogleUtils implements 
//GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {
//
//    public static final String TAG = "SavedGames";
//    // Request code used to invoke sign-in UI.
//    private static final int RC_SIGN_IN = 9001;
//    
//    // True immediately after the user clicks the sign-in button/
//    private boolean mSignInClicked = false;
//
//    // Progress Dialog used to display loading messages.
//    private ProgressDialog mProgressDialog;
//
//    // True when the application is attempting to resolve a sign-in error that has a possible
//    // resolution,
//    private boolean mIsResolving = false;
//    
//    // True if we want to automatically attempt to sign in the user at application start.
//    private boolean mAutoStartSignIn = true;
//
//    private GoogleApiClient mGoogleApiClient;
//	
//    private static boolean sInited = false;
//    
//    private static AppActivity self = null;
//    
//    // [Google Cloud - 2016-07-14] START
//    // [START init]
//    
//    public void init(final Activity activity) {
//        if (!sInited) {
//        	Log.d(TAG, "GoogleUtils Init Start");
//        	
//        	self = (AppActivity)activity;
//        	
//        	mGoogleApiClient = new GoogleApiClient.Builder(self)
//                    .addConnectionCallbacks(this)
//                    .addOnConnectionFailedListener(this)
//                    .addApi(Games.API).addScope(Games.SCOPE_GAMES)
//                    .addApi(Drive.API).addScope(Drive.SCOPE_APPFOLDER)
//                    .build();
//        	
//        	//GoogleLogin();
//            sInited = true;
//            
//            Log.d(TAG, "GoogleUtils Init End");
//        }
//    }
//    // [END init]
//    
//	 
//    // [START show_saved_games_ui]
///*    private static final int RC_SAVED_GAMES = 9009;
//
//    public void ShowSavedGamesUI() {
//        int maxNumberOfSavedGamesToShow = 5;
//        Intent savedGamesIntent = Games.Snapshots.getSelectSnapshotIntent(mGoogleApiClient,
//                "See My Saves", true, true, maxNumberOfSavedGamesToShow);
//        self.startActivityForResult(savedGamesIntent, RC_SAVED_GAMES);
//    }*/
//    // [END show_saved_games_ui]
//
//
//    // [START on_activity_result]
//    // private String mCurrentSaveName = "snapshotTemp";
//
//    /**
//     * This callback will be triggered after you call startActivityForResult from the
//     * ShowSavedGamesUI method.
//     */
//    /*public void ActivityResult(int requestCode, int resultCode, Intent intent) {
//        if (intent != null) {
//            if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_METADATA)) {
//                // Load a snapshot.
//                SnapshotMetadata snapshotMetadata = (SnapshotMetadata)
//                        intent.getParcelableExtra(Snapshots.EXTRA_SNAPSHOT_METADATA);
//                mCurrentSaveName = snapshotMetadata.getUniqueName();
//
//                // Load the game data from the Snapshot
//                // ...
//            } else if (intent.hasExtra(Snapshots.EXTRA_SNAPSHOT_NEW)) {
//                // Create a new snapshot named with a unique string
//                String unique = new BigInteger(281, new Random()).toString(13);
//                mCurrentSaveName = "snapshotTemp-" + unique;
//
//                // Create the new snapshot
//                // ...
//            }
//        }
//    }*/
//    // [END on_activity_result]
//    
//
//    // [START load_from_snapshot]
//    private byte[] mSaveGameData;
//
//    public void GoogleCloudLoad(final String key) {
//        // Display a progress dialog
//        // ...
//    	Log.d(TAG, "GoogleCloudLoad : " + key);
////    	showProgressDialog("loading...");
//    	
//        AsyncTask<Void, Void, Integer> task = new AsyncTask<Void, Void, Integer>() {
//            @Override
//            protected Integer doInBackground(Void... params) {
//                // Open the saved game using its name.
//                Snapshots.OpenSnapshotResult result = Games.Snapshots.open(mGoogleApiClient,
//                		key, true).await();
//
//                // Check the result of the open operation
//                if (result.getStatus().isSuccess()) {
//                    Snapshot snapshot = result.getSnapshot();
//                    // Read the byte content of the saved game.
//                    try {
//                        mSaveGameData = snapshot.getSnapshotContents().readFully();
//                    } catch (IOException e) {
//                        Log.e(TAG, "Error while reading Snapshot.", e);
//                    }
//                } else{
//                    Log.e(TAG, "Error while loading: " + result.getStatus().getStatusCode());
//                }
//
//                return result.getStatus().getStatusCode();
//            }
//
//            @Override
//            protected void onPostExecute(Integer status) {
//                // Dismiss progress dialog and reflect the changes in the UI.
//                // ...
//            	Log.d(TAG, "Data Load Status : " + status);
//            	if(status == GamesStatusCodes.STATUS_OK){
//                	String loadData = new String(mSaveGameData);
//                	Toast.makeText(self.getBaseContext(), key + " : " + loadData, Toast.LENGTH_LONG).show();
////                	dismissProgressDialog();
//                	Log.d(TAG, "onPostExecute" + loadData);
//                	
//                	AppActivity.JAVA_GoogleCloudLoad(key, loadData);
//                }
//            }
//        };
//
//        task.execute();
//    }
//    // [END load_from_snapshot]
//
//    
//    // [START send]
//    public void GoogleCloudSave(final String key, final String value) {    	
//        if(value == null) {
//            return;
//        }
//            
//        Log.d(TAG, "send Data : " + key + " : " + value);
//        if(mGoogleApiClient.isConnected()) {
//             AsyncTask<Void, Void, Snapshots.OpenSnapshotResult> task = 
//                     new AsyncTask<Void, Void, Snapshots.OpenSnapshotResult>() {
//                 @Override
//                 protected Snapshots.OpenSnapshotResult doInBackground(Void... params) {
//                     return Games.Snapshots.open(mGoogleApiClient,
//                    		 key, true).await();
//                 }
//                 
//                 @Override
//                 protected void onPostExecute(Snapshots.OpenSnapshotResult result) {
//                     try{
//                         Snapshot toWrite = processSnapshotOpenResult(result, 0);
//                         if(toWrite!=null) {
//                             toWrite.getSnapshotContents().writeBytes(value.getBytes());
//                             SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
//                                     .setDescription(key)
//                                      .build();
//                             Games.Snapshots.commitAndClose(mGoogleApiClient, toWrite, metadataChange);
//                             AppActivity.CPP_Toast("Save Finished");
//                         }
//                    } catch(IllegalStateException e) {
//                        
//                     }
//                 }
//             };
//                 
//             task.execute();
//        }        
//    }
//    // [END send]
//    
//    
////    private PendingResult<Snapshots.CommitSnapshotResult> writeSnapshot(Snapshot snapshot,
////            byte[] data, Bitmap coverImage, String desc) {
////
////        // Set the data payload for the snapshot
////        snapshot.getSnapshotContents().writeBytes(data);
////
////        // Create the change operation
////        SnapshotMetadataChange metadataChange = new SnapshotMetadataChange.Builder()
////                .setCoverImage(coverImage)
////                .setDescription(desc)
////                .build();
////
////        // Commit the operation
////        AppActivity.CPP_Toast("Save Finished");
////        return Games.Snapshots.commitAndClose(mGoogleApiClient, snapshot, metadataChange);
////    }
//    
//    
//    // [START process_snapshot_open_result]
//    private static final int MAX_SNAPSHOT_RESOLVE_RETRIES = 3;
//
//    /**
//     * Conflict resolution for when Snapshots are opened.  Must be run in an AsyncTask or in a
//     * background thread,
//     */
//    Snapshot processSnapshotOpenResult(Snapshots.OpenSnapshotResult result, int retryCount) {
//        Snapshot mResolvedSnapshot = null;
//        retryCount++;
//
//        int status = result.getStatus().getStatusCode();
//        Log.i(TAG, "Save Result status: " + status);
//
//        if (status == GamesStatusCodes.STATUS_OK) {
//            return result.getSnapshot();
//        } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONTENTS_UNAVAILABLE) {
//            return result.getSnapshot();
//        } else if (status == GamesStatusCodes.STATUS_SNAPSHOT_CONFLICT) {
//            Snapshot snapshot = result.getSnapshot();
//            Snapshot conflictSnapshot = result.getConflictingSnapshot();
//
//            // Resolve between conflicts by selecting the newest of the conflicting snapshots.
//            mResolvedSnapshot = snapshot;
//
//            if (snapshot.getMetadata().getLastModifiedTimestamp() <
//                    conflictSnapshot.getMetadata().getLastModifiedTimestamp()) {
//                mResolvedSnapshot = conflictSnapshot;
//            }
//
//            Snapshots.OpenSnapshotResult resolveResult = Games.Snapshots.resolveConflict(
//                    mGoogleApiClient, result.getConflictId(), mResolvedSnapshot).await();
//
//            if (retryCount < MAX_SNAPSHOT_RESOLVE_RETRIES) {
//                // Recursively attempt again
//                return processSnapshotOpenResult(resolveResult, retryCount);
//            } else {
//                // Failed, log error and show Toast to the user
//                String message = "Could not resolve snapshot conflicts";
//                Log.e(TAG, message);
//                Toast.makeText(self.getBaseContext(), message, Toast.LENGTH_LONG).show();
//            }
//
//        }
//
//        // Fail, return null.
//        return null;
//    }
//    // [END process_snapshot_open_result]
//
//    @Override
//    public void onConnected(Bundle connectionHint) {
//        Log.d(TAG, "onConnected");
//        Toast.makeText(self.getBaseContext(), "GoogleLogin Succeed", Toast.LENGTH_LONG).show();
//        AppActivity.JAVA_GoogleConnectionResult(true);
//    }
//
//    @Override
//    public void onConnectionSuspended(int i) {
//        Log.d(TAG, "onConnectionSuspended: " + i);
//        mGoogleApiClient.connect();
//        Toast.makeText(self.getBaseContext(), "GoogleLogin Suspended", Toast.LENGTH_LONG).show();
//        AppActivity.JAVA_GoogleConnectionSuspended();
//    }
//
//    @Override
//    public void onConnectionFailed(ConnectionResult connectionResult) {
//        Log.d(TAG, "onConnectionFailed");
//        if (mIsResolving) {
//            // The application is attempting to resolve this connection failure already.
//            Log.d(TAG, "onConnectionFailed: already resolving");
//            return;
//        }
//
//        if (mSignInClicked || mAutoStartSignIn) {
//            mSignInClicked = false;
//            mAutoStartSignIn = false;
//
//            // Attempt to resolve the connection failure.
//            Log.d(TAG, "onConnectionFailed: begin resolution. : " + connectionResult.getErrorCode());
//            mIsResolving = BaseGameUtils.resolveConnectionFailure(self, mGoogleApiClient,
//                    connectionResult, RC_SIGN_IN, self.getString(R.string.signin_other_error));
//        }
//        Toast.makeText(self.getBaseContext(), "GoogleLogin Failed", Toast.LENGTH_LONG).show();
//        AppActivity.JAVA_GoogleConnectionResult(false);
//    }
//    
//  	
//    // [START beginUserInitiatedSignIn]
//    /**
//     * Start the sign-in process after the user clicks the sign-in button.
//     */
//    public void GoogleLogin() {
//    	if(!isSignedIn()){
//        	
//            Log.d(TAG, "beginUserInitiatedSignIn");
//            // Check to see the developer who's running this sample code read the instructions :-)
//            // NOTE: this check is here only because this is a sample! Don't include this
//            // check in your actual production app.
//            if (!BaseGameUtils.verifySampleSetup(self, R.string.app_id)) {
//                Log.w(TAG, "*** Warning: setup problems detected. Sign in may not work!");
//            }
//
//            mSignInClicked = true;
//            mGoogleApiClient.connect();
//            
//            if(!isNetWork()){
//            	AppActivity.JAVA_GoogleConnectionResult(false);
//            }
//        }
//    }
//    // [END beginUserInitiatedSignIn]
//    
//    
//    /**
//     * Determine if the Google API Client is signed in and ready to access Games APIs.
//     * @return true if client exits and is signed in, false otherwise.
//     */
//    public boolean isSignedIn() {
//        return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
//    }
//    
//    private Boolean isNetWork(){
//		ConnectivityManager manager = (ConnectivityManager) self.getSystemService (Context.CONNECTIVITY_SERVICE);
//		boolean isMobileAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isAvailable();
//		boolean isMobileConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).isConnectedOrConnecting();
//		boolean isWifiAvailable = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isAvailable();
//		boolean isWifiConnect = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnectedOrConnecting();
//
//		if ((isWifiAvailable && isWifiConnect) || (isMobileAvailable && isMobileConnect)){
//			Log.d(TAG, "Network is connected");
//			return true;
//		}else{
//			Log.d(TAG, "Network is not connected");
//			return false;
//		}
//	}
//    
//    /**
//     * Show a progress dialog for asynchronous operations.
//     * @param msg the message to display.
//     */
//    private void showProgressDialog(String msg) {
//        if (mProgressDialog == null) {
//            mProgressDialog = new ProgressDialog(self);
//            mProgressDialog.setIndeterminate(true);
//        }
//
//        mProgressDialog.setMessage(msg);
//        mProgressDialog.show();
//    }
//    
//    /**
//     * Hide the progress dialog, if it was showing.
//     */
//    private void dismissProgressDialog() {
//        if (mProgressDialog != null && mProgressDialog.isShowing()) {
//            mProgressDialog.dismiss();
//        }
//    }
//    
//    // [Google Cloud - 2016-07-14] END
//}