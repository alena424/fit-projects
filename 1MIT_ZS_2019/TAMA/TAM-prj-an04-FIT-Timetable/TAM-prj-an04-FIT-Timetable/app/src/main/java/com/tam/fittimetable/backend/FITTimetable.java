package com.tam.fittimetable.backend;

import com.tam.fittimetable.backend.core.data.Subject;
import com.tam.fittimetable.backend.core.data.SubjectManager;
import com.tam.fittimetable.backend.core.extract.DownloadException;
import java.text.ParseException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class FITTimetable {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            for(Subject s : SubjectManager.get().getSubjects()) {
                System.out.println(s);
            }
        } catch (ParseException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
        } catch (DownloadException ex) {
            Logger.getLogger(FITTimetable.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

}
