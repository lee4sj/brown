//package directdecrypt;

import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class DLFromServer {
	static public void main (String[] args){
		
		if(args.length < 3){
			System.out.println("DLFromServer [host] [file] [dest]");
			System.exit(0);
		}
		
		String host = args[0];
		String music = args[1];
		String dest = args[2];
		
		
		BufferedWriter out = null;
		BufferedReader in = null;
		try{

            //connect to the server
			Socket server = new Socket(host, 8000);
			out = new BufferedWriter(new OutputStreamWriter(server.getOutputStream()));
			in = new BufferedReader(new InputStreamReader(server.getInputStream()));
			
			FileOutputStream file = new FileOutputStream(new File(dest));

            //authentication
			String res = in.readLine();
			out.write("ACCOUNT_VALIDATION_REQUEST\n");
			out.flush();
			out.write("sboger\n");
			out.flush();
			out.write("sboger\n");
			out.flush();
			res = in.readLine();

            //request file
			out.write("FILE_REQUEST\n");
			out.flush();
			out.write(music + "\n");
			out.flush();
			
			res = in.readLine();
			int file_size = Integer.parseInt(res.split(" ")[1]);
			
			out.write("BEGIN_SONG_SEND\n");
			out.flush();
			
			
			int tot_len = 0;
			byte[] buf = new byte[1024];
			//save to the file
			InputStream in_file = server.getInputStream();
			
			while(tot_len < file_size){
				int ret = in_file.read(buf);
				file.write(buf, 0, ret);
				tot_len += ret;
			}
			
			out.close();
			in.close();
			file.close();
			server.close();
			
		}catch(Exception e){
			e.printStackTrace();
			System.exit(0);
		}
		
		
	}
}
