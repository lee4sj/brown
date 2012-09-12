/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package life;

import java.io.IOException;

/**
 *
 * @author sl136
 */
public class Life {

    private int[][] cellArray;
    int row, col;

    public Life(int row, int col){
        this.row = row;
        this.col = col;
        cellArray = new int[row][col];
    }

    public int[][] setTo(int[][] initialEnvironment) throws IOException{

        int index1 = initialEnvironment.length;
        int index2 = initialEnvironment[0].length;

        if(index1 != row || index2 != col)
            throw new IOException("wrong input!");

        for(int a = 0; a < index1; a++)
            for(int b = 0; b < index2; b++){
                cellArray[a][b] = initialEnvironment[a][b];
            }

        return initialEnvironment;
    }

    public void addCell(int row, int col) throws IOException{
        if(row < 0 || row > this.row-1 || col < 0 || col > this.col-1)
            throw new IOException("wrong input!");

        cellArray[row][col] = 1;
    }

    public int countNeighbors(int a, int b){
        int neighbors = 0;

        int row1;
        int col1;
        
        for(int index1 = -1; index1 <= 1; index1++){
            row1 = a + index1;
            for(int index2 = -1; index2 <= 1; index2++){
                col1 = b + index2;
                if(row1 >= 0 && col1 >=0 && row1 < row && col1 < col && !(row1 == a && col1 == b))
                    if(cellArray[row1][col1] == 1)
                        neighbors ++;
            }
        }
        return neighbors;


/*        if(a == 0){
            if(b == 0)
                neighbors = cellArray[a+1][b] + cellArray[a][b+1] + cellArray[a+1][b+1];
            else if(b == col-1)
                neighbors = cellArray[a+1][b] + cellArray[a][b-1] + cellArray[a+1][b-1];
            else
                neighbors = cellArray[a][b-1] + cellArray[a+1][b-1] + cellArray[a+1][b] + cellArray[a][b+1]
                                  + cellArray[a+1][b+1];
        }

        else if(a == row-1){
            if(b == 0)
                neighbors = cellArray[a-1][b] + cellArray[a-1][b+1] + cellArray[a][b+1];
            else if(b == col-1)
                neighbors = cellArray[a-1][b] + cellArray[a-1][b-1] + cellArray[a][b-1];
            else
                neighbors = cellArray[a][b-1] + cellArray[a-1][b-1] + cellArray[a-1][b] + cellArray[a][b+1]
                                  + cellArray[a-1][b+1];
        }

        else{
            if(b == 0)
                neighbors = cellArray[a-1][b] + cellArray[a-1][b+1] + cellArray[a][b+1] + cellArray[a+1][b+1]
                                  + cellArray[a+1][b];
            else if(b == col-1)
                neighbors = cellArray[a-1][b] + cellArray[a-1][b-1] + cellArray[a][b-1] + cellArray[a+1][b-1]
                                  + cellArray[a+1][b];
            else
                neighbors = cellArray[a-1][b-1] + cellArray[a-1][b] + cellArray[a-1][b+1] + cellArray[a][b+1]
                                + cellArray[a+1][b+1] + cellArray[a+1][b] + cellArray[a+1][b-1] + cellArray[a][b-1];
        }

        return neighbors;*/
    }


    public int[][] nextGen(){
        
        int[][] neighbors = new int[row][col];

        for(int a1 = 0; a1<row; a1++)
            for(int b1 = 0; b1< col; b1++)
                neighbors[a1][b1] = countNeighbors(a1, b1);

        for(int a = 0; a < row; a++)
            for(int b = 0; b < col; b++){
                if(cellArray[a][b] == 0){
                    if(neighbors[a][b] == 3)
                        cellArray[a][b] = 1;
                }

                else if (cellArray[a][b] == 1){
                    if(neighbors[a][b] < 2 || neighbors[a][b] > 3)
                        cellArray[a][b] = 0;
                }
            }

        return cellArray;
    }
}












