package ija.ija2017.homework1.items;

import java.util.HashSet;

public class Item01 extends AbstractItem{
    public  Item01 ( String name, int numberOfPorts  ){
        super(name, numberOfPorts);

    }

    public void execute(){
        this.state = this.state + 1;
    }





}
