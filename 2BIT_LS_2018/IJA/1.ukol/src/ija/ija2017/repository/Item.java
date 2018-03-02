package ija.ija2017.repository;

import ija.ija2017.homework1.items.AbstractItem;

public interface Item {
    String getName();

    int numberOfPorts();

    void execute();

    int getState();
}
