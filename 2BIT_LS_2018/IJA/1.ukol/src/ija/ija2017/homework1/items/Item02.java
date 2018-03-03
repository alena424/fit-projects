package ija.ija2017.homework1.items;

public class Item02 extends AbstractItem {
    public  Item02 ( String name, int number  ){
        super(name, number);
    }

    public void execute(){
        this.state = this.state + 2;
    }
}
