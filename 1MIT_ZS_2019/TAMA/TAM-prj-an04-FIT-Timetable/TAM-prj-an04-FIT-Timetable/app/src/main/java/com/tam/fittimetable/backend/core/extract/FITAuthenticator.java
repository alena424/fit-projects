package com.tam.fittimetable.backend.core.extract;

import java.net.Authenticator;
import java.net.PasswordAuthentication;

/**
 *
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 * @source https://stackoverflow.com/questions/955624/download-a-file-from-the-internet-using-java-how-to-authenticate
 *          by user: <a href="https://stackoverflow.com/users/1342249/kairan">Kairan</a>
 *
 */
public class FITAuthenticator extends Authenticator {
    private static String username = "";
    private static String password = "";


    protected PasswordAuthentication getPasswordAuthentication() {
        return new PasswordAuthentication (FITAuthenticator.username,
                FITAuthenticator.password.toCharArray());
    }

    public static void setPasswordAuthentication(String username, String password) {
        FITAuthenticator.username = username;
        FITAuthenticator.password = password;
    }
}
