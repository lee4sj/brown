/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package life;

/**
 *
 * @author sl136
 */
public class MainClass {
/**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        long start = System.currentTimeMillis();

        int numGen = 5;
        int row = 10;
        int col = 10;
        int[][] initial = new int[row][col];

        //initialize

        /*initial[0][0] = 1;
        initial[0][1] = 1;
        initial[1][1] = 1;
        initial[0][9] = 1;
        initial[0][2] = 1;*/


        for(int k1 = 0; k1 < row; k1++)
            for(int l1 = 0; l1 < col; l1++)
                if(k1 == l1)
                    initial[k1][l1] = 1;

        //start;
        Life life = new Life(row, col);
        try{
            life.setTo(initial);
            arPrint(initial, row, col);
            for(int i = 0; i < numGen; i++){
                int[][] nextGen = life.nextGen();
                life.setTo(nextGen);
                arPrint(nextGen, row, col);
                
            }

        }catch(Exception e){
            System.out.println(e.getMessage());
        }

        long end = System.currentTimeMillis();
        System.out.println ("**** TIME  = " + (end - start) + " milliseconds");

    }


    public static void arPrint(int[][] array, int row, int col){
        for(int a = 0; a < row; a++){
            for(int b = 0; b < col; b++)
                System.out.print(array[a][b] + " ");
            System.out.println();
        }
        System.out.println("============================");
    }
}
