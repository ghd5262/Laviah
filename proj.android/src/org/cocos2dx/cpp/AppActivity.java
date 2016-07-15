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
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
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

public class AppActivity extends Cocos2dxActivity implements View.OnClickListener,
GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    public static final String TAG = "SavedGames";

    // The AppState slot we are editing.  For simplicity this sample only manipulates a single
    // Cloud Save slot and a corresponding Snapshot entry,  This could be changed to any integer
    // 0-3 without changing functionality (Cloud Save has four slots, numbered 0-3).
    private static final int APP_STATE_KEY = 0;

    // Request code used to invoke sign-in UI.
    private static final int RC_SIGN_IN = 9001;

    // Request code used to invoke Snapshot selection UI.
    private static final int RC_SELECT_SNAPSHOT = 9002;
    
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

    // [START on_create]
    @Override
    public void onCreate(Bundle savedInstanceState) {

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
    }
    // [END on_create]


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

    /**
     * This callback will be triggered after you call startActivityForResult from the
     * showSavedGamesUI method.
     */
    @Override
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
    
    	
    // [START beginUserInitiatedSignIn]
    /**
     * Start the sign-in process after the user clicks the sign-in button.
     */
    private void beginUserInitiatedSignIn() {
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
    
    
    // [START updateUI]
    /**
     * Display either the signed-in or signed-out view, depending on the user's state.
     */
    private void updateUI() {
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
    
    
    /**
     * Determine if the Google API Client is signed in and ready to access Games APIs.
     * @return true if client exits and is signed in, false otherwise.
     */
    private boolean isSignedIn() {
        return (mGoogleApiClient != null && mGoogleApiClient.isConnected());
    }
    
    
    /**
     * Display a status message for the last operation at the bottom of the screen.
     * @param msg the message to display.
     * @param error true if an error occurred, false otherwise.
     */
    private void displayMessage(String msg, boolean error) {
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
    
    /**
     * Show a progress dialog for asynchronous operations.
     * @param msg the message to display.
     */
    private void showProgressDialog(String msg) {
        if (mProgressDialog == null) {
            mProgressDialog = new ProgressDialog(this);
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
    
    /**
     * Get the data from the EditText.
     * @return the String in the EditText, or "" if empty.
     */
    private String getData() {
        EditText dataEditText = (EditText) findViewById(R.id.edit_game_data);
        return dataEditText.getText().toString();
    }
}