package com.tam.fittimetable.backend.core.conf;

/**
 * INI file has structure like this:
 * [DATA]
 * example = hello
 *
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class IniHandler {
    private final String conf = "conf/"; // path to directory with configuration
    private final String store = "saves/"; // path to saving directory

    /**
     * Returns value in INI file. INI files has extension .INI
     * @param path path is related to root folder of application
     *              ./conf/test.INI and key: DATA and example
     *                  is mapped to:
     *                  <b>conf.test.DATA.example</b>
     * @return data related to key or empty string
     */
    public static String get(String path) {
        return "";
    }
}
