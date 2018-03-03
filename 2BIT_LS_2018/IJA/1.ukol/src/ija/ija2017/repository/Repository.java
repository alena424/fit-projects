
package ija.ija2017.repository;

import java.util.HashSet;
import java.util.Set;

public class Repository {
    private final String name;
    private final Set<Item> content;

    public Repository(String var1) {
        this.name = var1;
        this.content = new HashSet();
    }

    public boolean add(Item var1) {
        if (var1 == null) {
            throw new NullPointerException("Repository: add null item");
        } else if (this.content.contains(var1)) {
            return false;
        } else {
            this.content.add(var1);
            return true;
        }
    }

    public boolean contains(Item var1) {
        if (var1 == null) {
            throw new NullPointerException("Repository: ask for null item");
        } else {
            return this.content.contains(var1);
        }
    }

    public boolean remove(Item var1) {
        if (var1 == null) {
            throw new NullPointerException("Repository: remove null item");
        } else {
            return this.content.remove(var1);
        }
    }
}
