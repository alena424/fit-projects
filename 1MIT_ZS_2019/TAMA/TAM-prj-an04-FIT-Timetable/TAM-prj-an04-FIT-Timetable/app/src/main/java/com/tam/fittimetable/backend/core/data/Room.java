package com.tam.fittimetable.backend.core.data;


/**
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public class Room {
    private final String name; // E112, D105, ...

    public Room(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    /**
     * Return link to website where room is described
     * @return link to webpage
     */
    public String getRoomOnWebsite() {
        return "https://www.fit.vut.cz/fit/room/"+name+"/";
    }

    /**
     * Return link to website where room is showed on map
     * @return link to map
     */
    public String getRoomOnMap() {
        return "https://www.fit.vut.cz/fit/map/?show="+name+"&big=1";
    }
}
