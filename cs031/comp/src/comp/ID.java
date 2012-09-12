/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp;

/**
 *
 * @author sl136
 */
public class ID {

    public IDType type;
    public String name;

    public ID(String n, IDType t){
        name = n;
        type = t;
    }

    public boolean equals(Object o){
        if(!(o instanceof ID))
            return false;

        ID id2 = (ID)o;
        if(name.equals(id2.name))
            return (type == id2.type);
        return false;
    }
}

