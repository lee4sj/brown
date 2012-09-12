/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package Assembler;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;


/**
 *
 * @author sl136
 */
public class Assembler {

    public static void main(String[] args){

        System.out.print("File Name: ");
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

        String name;


        try{
            name = in.readLine();
            File file = new File(name);

            System.out.println(file.getName());

            BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file)));

            String line = null;
            String hex = "v2.0 raw";

            while((line = reader.readLine()) != null){
                hex = hex.concat(toHex(line) + " ");
            }

            BufferedWriter out = new BufferedWriter(new FileWriter(file.getName()+".img"));

            System.out.println(file.getAbsolutePath());

            out.write(hex);
            out.close();


        }catch(Exception e){
            System.out.println(e.getMessage());
        }



    }

    public static String toHex(String code){

        String hexCode;

        int index1 = code.indexOf(" ");
        String opcode = code.substring(0, index1);
        code = code.substring(index1);

        String dest, sa, sb;


        if(opcode.equals("ADD")){
            opcode = Integer.toHexString(0);

            int index2 = code.indexOf(",");

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            index2 = code.indexOf(",");
            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sb = Integer.toHexString(twoComp(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim())));

            hexCode = opcode.concat(dest).concat(sa).concat(sb);
        }
        else if(opcode.equals("CMP")){
            opcode = Integer.toHexString(1);

            int index2 = code.indexOf(",");

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            index2 = code.indexOf(",");
            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sb = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat(dest).concat(sa).concat(sb);
        }
        else if(opcode.equals("SUB")){
            opcode = Integer.toHexString(2);

            int index2 = code.indexOf(",");

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            index2 = code.indexOf(",");
            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sb = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat(dest).concat(sa).concat(sb);
        }
        else if(opcode.equals("MOV")){
            opcode = Integer.toHexString(3);

            int index2 = code.indexOf(",");

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat(dest).concat(sa).concat("0");
        }
        else if(opcode.equals("LIM")){
            opcode = Integer.toHexString(4);

            int index2 = code.indexOf(",");

            System.out.println(code);

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r")+1, index2)));
            code = code.substring(index2+1);

            System.out.println(code);

            sa = Integer.toHexString(twoComp(Integer.parseInt(code.trim())));
            if(sa.length()>2)
                sa = sa.substring(sa.length()-2);

            if(sa.length() == 1)
                sa = "0".concat(sa);


            hexCode = opcode.concat(dest).concat(sa);

        }
        else if(opcode.equals("LW")){
            opcode = Integer.toHexString(5);

            int index2 = code.indexOf(",");

            dest = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat(dest).concat(sa).concat("0");
        }
        else if(opcode.equals("SW")){
            opcode = Integer.toHexString(6);

            int index2 = code.indexOf(",");

            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sb = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat("0").concat(sa).concat(sb);
        }
        else if(opcode.equals("JGZ")){
            opcode = Integer.toHexString(7);

            int index2 = code.indexOf(",");

            sa = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1, index2)));
            code = code.substring(index2+1);

            sb = Integer.toHexString(Integer.parseInt(code.substring(code.indexOf("r") + 1).trim()));

            hexCode = opcode.concat("0").concat(sa).concat(sb);
        }
        else
            hexCode = Integer.toHexString(8).concat("000");





        return hexCode;
    }

    public static int twoComp(int a){
        if(a < 0){
            int abs = -1*a;
            int b = ~abs;
            a = b+1;
        }
        return a;
    }
}












