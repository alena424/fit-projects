package ija.ija2017.homework1.items;

import ija.ija2017.repository.Item;

import java.util.Objects;

public abstract class AbstractItem implements Item{
    public String name;
    public int numberOfPorts;
    public int state;
    public  AbstractItem(String name, int number) {
        this.name = name;
        this.numberOfPorts = number;
        this.state = 0;
    }
    @Override
    public String getName() {
        return this.name;
    }

    @Override
    public int numberOfPorts() {
        return this.numberOfPorts;
    }

    @Override
    public void execute() {

    }

    @Override
    public int getState() {
        return this.state;
    }

    @Override
    public  int hashCode() {
        return Objects.hash(this.name, this.numberOfPorts, this.state);

    }

    @Override
    public boolean equals( Object other ){
        //System.out.print(11111);
        return other != null && other instanceof AbstractItem
                && ( this.name == ( (AbstractItem)other ).name )
                &&  ( this.numberOfPorts == ( (AbstractItem)other ).numberOfPorts )
                &&  ( this.state == ( (AbstractItem)other ).state );
    }
}
