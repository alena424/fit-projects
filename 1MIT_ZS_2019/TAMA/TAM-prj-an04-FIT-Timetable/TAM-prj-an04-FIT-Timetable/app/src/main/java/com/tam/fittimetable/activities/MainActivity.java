package com.tam.fittimetable.activities;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.security.ProviderInstaller;
import com.google.gson.JsonArray;
import com.tam.fittimetable.R;
import com.tam.fittimetable.backend.core.data.Strings;
import com.tam.fittimetable.backend.core.data.SubjectManager;
import com.tam.fittimetable.backend.core.extract.DownloadException;
import com.tam.fittimetable.backend.core.extract.Downloader;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.ParseException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;


import static com.tam.fittimetable.util.ExtensionsKt.showToast;


public class MainActivity extends AppCompatActivity {
    private Button loginButton;

    ProgressDialog mProgressDialog;
    MainActivity mainActivity;
    Boolean isLogIn;

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // if there already data, we dont have to sign up again, we can directly go to timetable
        FileInputStream fin = null;
        try {
            fin = openFileInput(Strings.FILE_NAME);

            if (fin.available() != 0){
                // file is not empty, we can show timetable
                final Intent intent = new Intent(this, StaticActivity.class);
                startActivity(intent);
            }
            fin.close();
        } catch (FileNotFoundException e) {
            System.out.println("File was not found");
        } catch (IOException e) {
            e.printStackTrace();
        }

        // variables need to synchronized
        mainActivity = this;
        isLogIn = false;
        //---

        setContentView(R.layout.login_layout);
        loginButton = (Button) findViewById(R.id.login);
        LinearLayout myLayout = (LinearLayout) this.findViewById(R.id.loginLayout);
        myLayout.requestFocus();

        Downloader.setMyContext(this);
        Downloader.recreateDir();
        setLogin();

        // setting progressing bar
        mProgressDialog = new ProgressDialog(this);
        mProgressDialog.setMessage(this.getString(R.string.trying_to_login));

        String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE};

        // this is for lower versions
        if(Build.VERSION.SDK_INT > Build.VERSION_CODES.KITKAT){
            requestPermissions(permissions, Strings.WRITE_REQUEST_CODE);
        } else {
            updateAndroidSecurityProvider(this);
        }

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                signUpToSystem();
            }
        });
    }


    /**
     * Method creates security provider - just for lower version, solves problem with bad protocol
     * @param callingActivity
     */
    private void updateAndroidSecurityProvider(Activity callingActivity) {
        try {
            ProviderInstaller.installIfNeeded(this);
        } catch (GooglePlayServicesRepairableException e) {
            // Thrown when Google Play Services is not installed, up-to-date, or enabled
            // Show dialog to allow users to install, update, or otherwise enable Google Play services.
            GooglePlayServicesUtil.getErrorDialog(e.getConnectionStatusCode(), callingActivity, 0);
        } catch (GooglePlayServicesNotAvailableException e) {
            showToast(this,"Google Play Services not available.");
        }
    }

    private void saveLoginToFile(String name, String password){
        FileOutputStream fos = null;
        try {
            fos = this.openFileOutput(Strings.LOGIN_FILE_NAME, MODE_PRIVATE);
            String txt = name.concat(",").concat(password);
            fos.write(txt.getBytes());
            System.out.println("Write sucessfully to " + txt);
            //showToast(this, "Saved");

        }
        catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (fos != null){
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Method tries to sign up to wis calling asynchronious futuretask
     */
   public void signUpToSystem(){
       EditText nameEdit = findViewById(R.id.loginName);
       EditText passwordEdit = findViewById(R.id.passwordId);

       String name = String.valueOf(nameEdit.getText()).trim();
       String password = String.valueOf(passwordEdit.getText()).trim();
       Downloader.setAuth(name,password);

       if (!isNetworkAvailable()) {
           showToast(this, getString(R.string.message_error_network_connection));
       } else {
           try {
               SubjectManager sm = new SubjectManager();
               ExecutorService executor = Executors.newSingleThreadExecutor();
               FutureTasks futureTask = new FutureTasks();
               futureTask.execute(executor.submit(sm.get()));
               saveLoginToFile(name, password);
           } catch (ParseException | DownloadException e) {
               e.printStackTrace();
               showToast(this, getString(R.string.message_login_failed));
           }
       }
    }

    /**
     * Method sets login and password from login.txt
     */
    private void setLogin(){
        EditText nameEdit = findViewById(R.id.loginName);
        EditText passwordEdit = findViewById(R.id.passwordId);

        String savedLogin = "";
        String savedPass = "";
        FileInputStream is = null;
        try {

            is = this.openFileInput(Strings.LOGIN_FILE_NAME);
            //is = getAssets().open(Strings.LOGIN_FILE_NAME);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);

            String text = new String(buffer);
            String[] token = text.split(",");
           // System.out.println("text " + text);
            if ( token.length == 2 ) {
                savedLogin = token[0];
                savedPass = token[1];
            }
          //  System.out.println("set login to " + is.getClass().getResource());
            is.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }

        nameEdit.setText(savedLogin);
        passwordEdit.setText(savedPass);

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case Strings.WRITE_REQUEST_CODE:
                if(grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    //Granted.
                    System.out.println("Garantovany pristup");
                }
                else{
                    //Denied.
                }
                break;
        }
    }

    public class FutureTasks extends AsyncTask<Future<Boolean>, Void, Void> {

        Boolean loginSuccessful = false;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mProgressDialog.show();
            System.out.println("Start ");
        }

        @Override
        protected Void doInBackground(Future<Boolean>... params) {

            loginSuccessful = false;

            System.out.println("Downloading in progress");
            Future<Boolean> f = params[0];

            FileOutputStream fos = null;
            try {
                f.get();
                JsonArray jsonData = SubjectManager.json;

                fos = openFileOutput(Strings.FILE_NAME, MODE_PRIVATE);
                fos.write(jsonData.toString().getBytes());
                System.out.println(jsonData.toString());
                loginSuccessful = true;
            } catch (IOException | InterruptedException | ExecutionException e) {
                e.printStackTrace();
            } finally {
                if (fos != null){
                    try {
                        fos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            mProgressDialog.dismiss();
            isLogIn = loginSuccessful;
            System.out.println("End of downloading");
            if (loginSuccessful) {
                final Intent intent = new Intent(mainActivity, StaticActivity.class);
                startActivity(intent);
                showToast(mainActivity, mainActivity.getString(R.string.message_login_successfull));
            } else {
                new AlertDialog.Builder(mainActivity).setMessage(R.string.uncorrect_login).setCancelable(true)
                        .setPositiveButton("OK", null)
                        .show();
            }
        }


    }


    private boolean isNetworkAvailable() {
        ConnectivityManager connectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivityManager == null) {
            return false;
        }

        NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();

        return activeNetworkInfo != null && activeNetworkInfo.isConnected();
    }
}
