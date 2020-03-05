/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.tam.fittimetable.backend.core.extract;

import java.io.IOException;

/**
 *
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class DownloadException extends IOException{

    public DownloadException(String msg) {
        super(msg);
    }

}
