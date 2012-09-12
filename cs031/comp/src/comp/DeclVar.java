/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package comp;

/**
 *
 * @author sl136
 */
public class DeclVar {

    public String name;
    public int size;

    public DeclVar(String name, int size){
        this.name = name;
        this.size = size;
    }

    /**
     * equal if this.name and o.name are the same
     *
     * @param o
     * @return
     */
    public boolean equals(Object o)
    {
        if(!(o instanceof DeclVar))
            return false;

        return (name.equals(((DeclVar)o).name));
    }
}
