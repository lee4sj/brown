//package directdecrypt;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.IvParameterSpec;

public class DirectDecrypt {

	public static void main(String[] args){
		if(args.length < 3){
			System.out.println("DirectDecrypt [password] [src file] [dest file]");
			System.exit(0);
		}
		
		String key = args[0];
		
		String filepath = args[1];
		
		//get 8 bytes to get the key
		if(key.length() > 8)
			key = key.substring(0, 8);
		while(key.length() < 8)
			key = key.concat("0");
		try{
			//generate key using the passcode
			DESKeySpec specs = new DESKeySpec(key.getBytes());
			SecretKey hashkey = SecretKeyFactory.getInstance("DES").generateSecret(specs);
			
			FileInputStream file = new FileInputStream(new File(filepath));
			
			byte[] nb = new byte[8];
			IvParameterSpec spec2 = new IvParameterSpec(nb);
			
			
			Cipher cipher = null;
			
			cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");
			cipher.init(2, hashkey, spec2);
			
			//get deciphered stream
			CipherInputStream in = new CipherInputStream(file, cipher);
			FileOutputStream out = new FileOutputStream(new File(args[2]));
			
			//save to the file
			byte[] buffer = new byte[1024];
			int ret = 0;
			while((ret = in.read(buffer)) >= 0){
				out.write(buffer, 0, ret);
			}
			
			out.close();
		}catch(Exception e){
			e.printStackTrace();
			System.exit(0);
		}
		
		System.exit(1);
		
	}
}
