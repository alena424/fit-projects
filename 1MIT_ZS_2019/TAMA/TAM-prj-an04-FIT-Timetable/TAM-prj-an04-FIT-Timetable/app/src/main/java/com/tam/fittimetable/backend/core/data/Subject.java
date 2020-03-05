package com.tam.fittimetable.backend.core.data;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.tam.fittimetable.backend.core.extract.DownloadException;
import com.tam.fittimetable.backend.core.extract.Downloader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Objects;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;


/**
 * @author Petr Kohout <xkohou14 at stud.fit.vutbr.cz>
 */
public final class Subject {

    public enum Day {
        MONDAY("Monday", 1, "Mon"),
        TUESDAY("Tuesday", 2, "Tue"),
        WEDNESDAY("Wednesday", 3, "Wed"),
        THURSTDAY("Thursday", 4, "Thu"),
        FRIDAY("Friday", 5, "Fri");

        private final String display;
        private final int value;
        private final String shortCut;

        public String display() {
            return this.display;
        }

        public int value() {
            return this.value;
        }

        public String shortCut() {
            return this.shortCut;
        }

        private Day(String display, int value, String shortCut) {
            this.display = display;
            this.value = value;
            this.shortCut = shortCut;
        }
    }

    private String name;
    private String linkToSubject;
    private Room room;
    private int from; // hour of beggining: 8, 9, 10, ...
    private int to;
    private Day day;
    private String color;
    private Date eventDay = null;

    private boolean[] weeks = {false, false, false, false, false, false, false, false, false, false, false, false, false}; // weeks when event happens

    public Subject(String name) {
        this.name = name;
    }

    public Subject(String name, String link, String room, int from, int to, String day, String color) {
        this.name = name;
        this.linkToSubject = link;
        this.room = new Room(room.toUpperCase());
        this.from = from;
        this.to = to;
        setDay(day);
        this.color = color;
    }

    /**
     * Sets week at index to value
     * @param index
     *
     * @return true if week is in semester range and could be set up false otherwise
     */
    public boolean setWeeksOfMentoring(int index, boolean value){
        if (index >= 0 && index < 12) {
            weeks[index] = value;
            return true;
        } else {
            return false;
        }
    }

    public Date getEventDay() {
        return eventDay;
    }

    public void setEventDay(Date eventDay) {
        this.eventDay = eventDay;
    }

    /**
     * Downloads file with subject card and extracts its mentoring weeks if
     * there is some special events listed in table with color #ffffdc which
     * means something really special like an exam, so we will check this events
     * already exists, if not, we will create it.
     * @throws java.text.ParseException
     */
    public void setWeeksOfMentoring() throws ParseException {
        try {
            Document doc = Jsoup.parse(Downloader.download(linkToSubject, Strings.SUBJECT_CARD_FILE + name, true), "ISO-8859-2");
            if (doc.select("table").isEmpty()) { // there is no table with info, so it is mentioned for all weeks
                for (int i = 0; i < 13; i++) {
                    weeks[i] = true;
                }
                return;
            }
            Elements rows = doc.selectFirst("table").selectFirst("tbody").select("tr");

            if(doc.select("table").size() > 1) { //there are other tables
                for (Element e : doc.select("table")) {
                    if(!e.select("th").isEmpty()) { // last often contains lectures timetable
                        rows = e.selectFirst("tbody").select("tr");
                    }
                }
            }

            for (Element e : rows) {
                if (e.select("th").isEmpty()) { // skip if there is no th, which significant day
                    continue;
                }
                if (e.selectFirst("th").text().equalsIgnoreCase(day.shortCut())
                        && // select day
                        Integer.parseInt(e.select("td").get(3).text().split(":")[0]) == from
                        && //select beginning time
                        Integer.parseInt(e.select("td").get(4).text().split(":")[0]) + 1 == to) {
                    // it is same subject as in table, so we extract the lectures weeks
                    String lectures = e.select("td").get(1).text(); // extract lectures
                    if (lectures.equals(Strings.ALL_SEMESTER_EDUCATION)) {
                        for (int i = 0; i < 13; i++) {
                            weeks[i] = true;
                        }
                    } else { // extract some weeks
                        String weeksOfMentioning = lectures.split(" of")[0]; // we can see: 13. of lectures (in tables)
                        String weekByWeek[] = {weeksOfMentioning, ""};
                        if (weeksOfMentioning.contains(",")) { // if it contains "," there is more then 1 week of lecture
                            weekByWeek = weeksOfMentioning.split(","); // so split the string
                            for (int i = 0; i < weekByWeek.length; i++) {
                                // iterate through but ignore the last one because there is nothing for us ;)
                                int numberOfWeek = Integer.parseInt(weekByWeek[i].trim().split("\\.")[0]); // from 13. we want just 13
                                weeks[numberOfWeek - 1] = true; // it is mentioned this week
                            }
                        } else {
                            if (weeksOfMentioning.contains("-")) { //it is simple date in format YYYY-MM-DD
                                SimpleDateFormat formatter = new SimpleDateFormat(Strings.DATE_FORMAT_YYYY_MM_DD);
                                Date date = formatter.parse(weekByWeek[0].trim());
                                int index = SubjectManager.get().getWeekOfSemester(date);
                                for (int i = 0; i < 13; i++) { // set to false
                                    weeks[i] = false;
                                }
                                if(index > -1 && index < 13) {
                                    weeks[index] = true;
                                    System.out.println(this + " is mentioned " + index+1 + " week.");
                                }
                            } else {
                                for (int i = 0; i + 1 < weekByWeek.length; i++) {
                                    // iterate through but ignore the last one because there is nothing for us ;)
                                    int numberOfWeek = Integer.parseInt(weekByWeek[i].trim().split("\\.")[0]); // from 13. we want just 13
                                    weeks[numberOfWeek - 1] = true; // it is mentioned this week
                                }
                            }
                        }
                    }
                } else { // it is not that subject, it can be special, so check it
                    if (e.attr("style").contains("#ffffdc")) { // it is special event
                        Subject specialSubject = new Subject(
                                this.name,
                                this.linkToSubject,
                                this.room.getName(),
                                Integer.parseInt(e.select("td").get(3).text().split(":")[0]),
                                Integer.parseInt(e.select("td").get(4).text().split(":")[0]) + 1,
                                e.selectFirst("th").text(),
                                "#ffffdc");
                        SubjectManager manager = SubjectManager.get();
                        if (!manager.contains(specialSubject)) {
                            manager.addSubject(specialSubject);
                            specialSubject.setWeeksOfMentoring();
                        }
                    }
                }
            }
        } catch (IOException ex) {
            Logger.getLogger(Subject.class.getName()).log(Level.SEVERE, null, ex);
            // it will be mentored each week
        }
    }

    /**
     * returns string with semicoloned weeks
     * Only for toString()
     *
     * @return
     */
    public String semicolonedWeeks() {
        String list = "";
        for (int i = 0; i < 12; i++) {
            list += weeks[i] + ",";
        }
        list += weeks[12];
        return list;
    }

    /**
     * @param week number of week (from 1 to 13)
     */
    public boolean isMentioned(int week) {
        if(week < 1 || week > 13) {
            return false;
        }
        return weeks[week - 1];
    }

    /**
     *
     * @return true if subject is mentioned in semester (event 1 week)
     */
    public boolean isMentioned() {
        for (boolean w : weeks) {
            if (w) {
                return true;
            }
        }
        return false;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Room getRoom() {
        return room;
    }

    public void setRoom(Room room) {
        this.room = room;
    }

    public int getFrom() {
        return from;
    }

    public void setFrom(int from) {
        this.from = from;
    }

    public int getTo() {
        return to;
    }

    public void setTo(int to) {
        this.to = to;
    }

    public Day getDay() {
        return day;
    }

    public void setDay(Day day) {
        this.day = day;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public String getLinkToSubject() {
        return linkToSubject;
    }

    public void setLinkToSubject(String linkToSubject) {
        this.linkToSubject = linkToSubject;
    }

    @Override
    public String toString() {
        return name + "|" + linkToSubject + "|" + room.getName() + "|" + from + "|" + to + "|" + day.shortCut() + "|" + color + "|" + semicolonedWeeks()+ "|" + eventDay;
    }
    
    private String parseToJson(String arg, String value) {
        return "\"" + arg + "\": \"" + value + "\"";
    }
    
    private String parseToJson(String arg, int value) {
        return "\"" + arg + "\": " + value ;
    }
    
    private String getMentionedWeeksInJson() {
        return "\"weeks\": [" + semicolonedWeeks() + "]";
    }
    
    private String parseAttributes(JsonObject courseJson, int weekIndex) {
        courseJson.addProperty("id", hashCode() + weekIndex);
        courseJson.addProperty("name", name);
        courseJson.addProperty("linkToSubject", linkToSubject);
        courseJson.addProperty("room", room.getName());
        courseJson.addProperty("from", from);
        courseJson.addProperty("startTime", from + ":00");
        courseJson.addProperty("to", to);
        courseJson.addProperty("endTime", to + ":00");
        courseJson.addProperty("day", day.shortCut());
        courseJson.addProperty("color", color);

        return ""
                + parseToJson("id", hashCode() + weekIndex) + ",\n"
                + parseToJson("name", name) + ",\n"
                + parseToJson("linkToSubject", linkToSubject) + ",\n"
                + parseToJson("room", room.getName())+ ",\n"
                + parseToJson("from", from)+ ",\n"
                + parseToJson("startTime", from + ":00")+ ",\n"
                + parseToJson("to", to)+ ",\n"
                + parseToJson("endTime", to + ":00")+ ",\n"
                + parseToJson("day", day.shortCut())+ ",\n"
                + parseToJson("color", color)+ ",\n"
                + getMentionedWeeksInJson()+ "";
    }
    
    private String packJsonObject(String obj) {
        return "{\n"
                + obj
                + "}\n";
    }
    
    public JsonArray toJson() {
        //JsonObject courseJson = new JsonObject();
        JsonArray jsonArray = new JsonArray();

        String tmp; // it is always same
        String finalJson = "", actualJson = "";
        Date begginingOfSemester = null;
        Calendar c = Calendar.getInstance();
        
        try {
            begginingOfSemester = SubjectManager.get().actualSemester().get(0);
        } catch (ParseException | DownloadException ex) {
            Logger.getLogger(Subject.class.getName()).log(Level.SEVERE, null, ex);
            return null;
        }
        
        int lastIndex = 0; // last index of true
        for (int i = 0; i < 13; i++) {
            if(weeks[i]) {
                lastIndex = i;
            }
        }
        
        for (int i = 0; i <= lastIndex; i++) {
            JsonObject courseJson = new JsonObject();
            tmp = parseAttributes(courseJson, i);
            if(weeks[i]) { // subject is mentioned this week
                c.setTime(begginingOfSemester);
                c.add(Calendar.DAY_OF_MONTH, i*7 + day.value()-1);


                actualJson += tmp + ",\n";
                courseJson.addProperty("dayOfMonth", c.get(Calendar.DAY_OF_MONTH));
                courseJson.addProperty("month", c.get(Calendar.MONTH));
                courseJson.addProperty("year", c.get(Calendar.YEAR));
                jsonArray.add(courseJson);

                /*actualJson += parseToJson("dayOfMonth", c.get(Calendar.DAY_OF_MONTH)) + ",\n";
                actualJson += parseToJson("month", c.get(Calendar.MONTH)) + ",\n";
                actualJson += parseToJson("year", c.get(Calendar.YEAR)) + "\n";
                finalJson += packJsonObject(actualJson);
                if(i != lastIndex) { //if it is last record does not make ,
                    finalJson += ",\n";
                }       */
            } else {
                if(eventDay != null) {
                    c.setTime(eventDay);
                    courseJson.addProperty("dayOfMonth", c.get(Calendar.DAY_OF_MONTH));
                    courseJson.addProperty("month", c.get(Calendar.MONTH));
                    courseJson.addProperty("year", c.get(Calendar.YEAR));
                    jsonArray.add(courseJson);
                    return jsonArray;
                }
            }
            //actualJson = "";
        }
        //return courseJson;
        return jsonArray;
        
        //return finalJson;
    }

    public void setDay(String day) {
        if (day.length() > 3) {
            day = day.toLowerCase().substring(0, 3);
        } else {
            day = day.toLowerCase();
        }
        switch (day) {
            case "mon":
            case "po":
                setDay(Day.MONDAY);
                break;
            case "tue":
            case "út":
                setDay(Day.TUESDAY);
                break;
            case "wed":
            case "st":
                setDay(Day.WEDNESDAY);
                break;
            case "thu":
            case "čt":
                setDay(Day.THURSTDAY);
                break;
            case "fri":
            case "pá":
                setDay(Day.FRIDAY);
                break;
            default:
                throw new IllegalArgumentException(day + " is not recognized as day. We support: Mon, Tue, Wed, Thu or Fri");
        }
    }

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 41 * hash + Objects.hashCode(this.name);
        hash = 41 * hash + this.from;
        hash = 41 * hash + this.to;
        hash = 41 * hash + Objects.hashCode(this.day);
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Subject other = (Subject) obj;
        if (this.from != other.from) {
            return false;
        }
        if (this.to != other.to) {
            return false;
        }
        if (!Objects.equals(this.name, other.name)) {
            return false;
        }
        if (this.day != other.day) {
            return false;
        }
        return true;
    }
}
