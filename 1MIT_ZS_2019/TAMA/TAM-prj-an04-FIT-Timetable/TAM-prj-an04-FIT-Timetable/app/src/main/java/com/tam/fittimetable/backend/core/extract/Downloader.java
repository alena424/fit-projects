package com.tam.fittimetable.backend.core.extract;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;

import com.tam.fittimetable.backend.FITTimetable;
import com.tam.fittimetable.backend.core.data.Strings;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Authenticator;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;

/**
 *
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class Downloader {

    private static boolean autheticatorSet = false;
    private static Context myContext = null;
    private static SSLContext sslContext = null;
    private static String login = null;
    private static String password = null;

    public static void setAuth(String log, String pass){
        login = log;
        password = pass;
    }

    public static void setMyContext(Context c) {
        myContext = c;
    }
    /**
     * Downloads file with authentication
     *
     * @param storeTo
     * @throws com.tam.fittimetable.backend.core.extract.DownloadException
     * @see
     * https://stackoverflow.com/questions/955624/download-a-file-from-the-internet-using-java-how-to-authenticate
     * code by Kairan at Stack Overflow:
     * https://stackoverflow.com/users/1342249/kairan
     * @param link
     * @return
     */
    public static File download(String link, String storeTo) throws IOException {
        URL url;
        InputStream is = null;
        BufferedReader br;
        String line;


        if (!autheticatorSet) {
            // Install Authenticator
            createFolders();
            setKeystore();
            //if(System.getProperty("login") == null) { // for testing set in Strings class
            FITAuthenticator.setPasswordAuthentication(login, password);
           // System.out.println("aaaaa " +login + " " + password);
            /*if ( ! login.isEmpty() && ! password.isEmpty() ){
                    System.out.println("aaaaa");
                    FITAuthenticator.setPasswordAuthentication(login, password);
                } else {
                    FITAuthenticator.setPasswordAuthentication(Strings.LOGIN, Strings.PASSWORD);
                }

            } else {
                System.out.println(System.getProperties());
                FITAuthenticator.setPasswordAuthentication(System.getProperty("login"), System.getProperty("password"));
            }*/



       }

        try {
            url = new URL(link);
            //is = url.openStream();  // throws an IOException
            HttpsURLConnection urlConnection = (HttpsURLConnection)url.openConnection();
            urlConnection.setSSLSocketFactory(sslContext.getSocketFactory());
            InputStream in = urlConnection.getInputStream();
            br = new BufferedReader(new InputStreamReader(in));
            //File f = File.createTempFile(storeTo,"",Environment.getExternalStoragePublicDirectory(
              //      Environment.DIRECTORY_PICTURES));
            File f = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString() + "/" + storeTo );
            System.out.println("Downloading file: " + f);
            BufferedWriter writer = new BufferedWriter(new FileWriter(f));
            while ((line = br.readLine()) != null) {
                writer.write(line);
            }
            writer.close();
           // autheticatorSet = true; // povedlo se
            Authenticator.setDefault(new FITAuthenticator());
            return f;
            //File file = new File(storeTo);
            //return file;
        } catch (MalformedURLException mue) {
            mue.printStackTrace();
            throw new DownloadException("MalformedURLException exception: " + mue.getMessage() + "\n" + mue.getStackTrace());
        } catch (IOException ioe) {
            ioe.printStackTrace();
            throw new DownloadException("IOException exception: " + ioe.getMessage() + "\n" + ioe.getStackTrace());
        } finally {
            try {
                if (is != null) {
                    is.close();
                }
            } catch (IOException ioe) {
                throw new IOException(ioe.getMessage());
            }
        }
    }

    /**
     * Downloads file with authentication
     *
     * @param storeTo
     * @throws com.tam.fittimetable.backend.core.extract.DownloadException
     *
     * @param link
     * @param checkExistence if it set to yes, it looks for existing file
     * @return
     */
    public static File download(String link, String storeTo, Boolean checkExistence) throws IOException {
        String path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString() + "/" + storeTo;
        File loadeFile = new File(path);

        if (loadeFile.exists() && checkExistence) {
            System.out.println("Cache hit: " + path);
            return loadeFile;
        } else  { // it is like download new
            System.out.println("Cache MISS: " + path);
            return download(link, storeTo);
        }
    }

    public static File downloadNoCertificate(String link, String storeTo) {
        try {
            URL website = new URL(link);
            ReadableByteChannel rbc = Channels.newChannel(website.openStream());
            FileOutputStream fos = new FileOutputStream(storeTo);
            fos.getChannel().transferFrom(rbc, 0, Long.MAX_VALUE);
        } catch (MalformedURLException ex) {
            Logger.getLogger(Downloader.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Downloader.class.getName()).log(Level.SEVERE, null, ex);
        }
        return new File(storeTo);
    }

    public static File downloadSubjectCard(String link) {
        //return new File("./test/subject-card.html");
        //return new File("./test/subject-card-SUI.html");
        return new File("./test/subject-card-UPA.html");
    }

    public static File downloadAcademicYear(String link) {
        return new File("./test/academicYear.html");
    }

    protected static void createFolders() {
        File f = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString() + "/" + Strings.DOWNLOAD);
        if(!f.isDirectory()) {
            f.mkdir();
        }
    }

    /**
     * Sets keystore or create new one exemple from
     * https://coderanch.com/t/133048/engineering/programmatically-create-keystore-import-certificate
     * @throws com.tam.fittimetable.backend.core.extract.DownloadException
     */
    public static void setKeystore() throws DownloadException {
        File keyStore = new File(Strings.KEYSTORE);
        if (keyStore.exists()) {
            System.setProperty("javax.net.ssl.trustStore", Strings.KEYSTORE);
            return;
        }

        KeyStore ks;
        try {
            ks = KeyStore.getInstance(KeyStore.getDefaultType());
            ks.load(null, null);
            // Create a TrustManager that trusts the CAs in our KeyStore
            String tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm();
            TrustManagerFactory tmf = TrustManagerFactory.getInstance(tmfAlgorithm);
            //tmf.init(ks);

            List<String> files = new ArrayList<>();
            files.add(Strings.FIT_CACERT);
            files.add(Strings.VUT_CACERT);
            for (String file : files) {
                AssetManager assetManager = myContext.getAssets();
                InputStream is = assetManager.open(file);
                //FileInputStream fis = new FileInputStream(is);
                BufferedInputStream bis = new BufferedInputStream(is);
                CertificateFactory cf = CertificateFactory.getInstance("X.509");
                Certificate cert = null;
                while (bis.available() > 0) {
                    cert = cf.generateCertificate(bis);
                    ks.setCertificateEntry(file.substring(5), cert);
                }
                //ks.store(/*new FileOutputStream(Strings.KEYSTORE)*/assetManager.open(Strings.KEYSTORE) , "MyPass".toCharArray());
            }
            tmf.init(ks);
            sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, tmf.getTrustManagers(), null);

            System.setProperty("javax.net.ssl.trustStore", Strings.KEYSTORE);
        } catch (KeyStoreException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
            throw new DownloadException("Keystore exception: " + ex.getMessage() + "\n" + ex.getStackTrace());
        } catch (IOException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
            throw new DownloadException("IOException exception: " + ex.getMessage() + "\n" + ex.getStackTrace());
        } catch (NoSuchAlgorithmException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
            throw new DownloadException("NoSuchAlgorithmException exception: " + ex.getMessage() + "\n" + ex.getStackTrace());
        } catch (java.security.cert.CertificateException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
            throw new DownloadException("CertificateException exception: " + ex.getMessage() + "\n" + ex.getStackTrace());
        } catch (KeyManagementException e) {
            e.printStackTrace();
        }
    }

    public static void recreateDir() {
        System.out.println("Recreating download dir at: " + Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString() + "/" + Strings.DOWNLOAD);
        File f = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString() + "/" + Strings.DOWNLOAD);
        if (f.exists()) {
            if (f.isDirectory()) {
                for (File file : f.listFiles()) {
                    file.delete();
                    System.out.println("Deleted: " + file);
                }
                f.delete();
            }
        }
        f.mkdir();
    }
}
