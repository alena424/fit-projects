package com.tam.fittimetable.backend.core.data;

/**
 * Strings used in system
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class Strings {
    public static String LOGIN = "";
    public static String PASSWORD = "";

    public static final String ROOT = "./";

    public static final String PRIVATE_TIMETABLE_LINK = "https://wis.fit.vutbr.cz/FIT/st/studyps-l.php.en";
    public static final String ACADEMIC_YEAR = "https://www.fit.vut.cz/study/calendar/2019/.en";

    public static final String CERTIFICATES = ROOT + "app/keystore/";
    public static final String FIT_CACERT = "fit-cacert.crt";
    public static final String VUT_CACERT = "BrnoUniversityofTechnologyCA.crt";
    public static final String KEYSTORE = CERTIFICATES + "FitKeyStore";

    public static final String DOWNLOAD = ROOT + "downloads/";
    public static final String PRIVATE_TIMETABLE_FILE = DOWNLOAD + "private-timetable.html";
    public static final String SUBJECT_PRIVATE_FILE = DOWNLOAD + "private-subject.html";
    public static final String SUBJECT_CARD_FILE = DOWNLOAD + "subject-card.html";
    public static final String ACADEMIC_YEAR_FILE = DOWNLOAD + "academic-year.html";

    public static final String WEB_PREFIX = "https://wis.fit.vutbr.cz/FIT/st/";
    public static final String WEB_CODING = "ISO-8859-2";

    public static final String DATE_FORMAT = "dd. MMM yyyy";
    public static final String LOGIN_FILE_NAME = "login.txt";
    public static final String DATE_FORMAT_ACADEMIC = "dd MMM yyyy";

    public static final String DATE_FORMAT_DD_MM_YYYY = "dd-MM-yyyy";
    public static final String DATE_FORMAT_YYYY_MM_DD = "yyyy-MM-dd";
    public static final String MIDDLE_OF_YEAR = "20. July ";
    public static final String MIDDLE_OF_YEAR_DD_MM_ = "20-07-";
    public static final String BEGGINING_OF_YEAR = "1. January ";
    public static final String END_OF_YEAR = "31. December ";
    public static final String END_OF_YEAR_DD_MM_ = "31-12-";

    public static final String ALL_SEMESTER_EDUCATION = "lectures"; // field is written if subject is mentioned for whole semester

    public static final String WRITE_EXTERNAL_STORAGE = "I want to write please";
    public static final int WRITE_REQUEST_CODE = 888333444;

    public static final String FILE_NAME = "courses.json";
}
