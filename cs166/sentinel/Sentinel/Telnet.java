

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class Telnet {
	
	private static final byte IAC = (byte)255;
	private static final byte DO = (byte)253;
	private static final byte WONT = (byte)252;
	
	private static final int DELAY_TIME = 2000;
	
	private OutputStream writer = null;
	private InputStream reader = null;
	
	private static byte[] toTelnet(String s) { return (s + "\r\n").getBytes(); }

	public boolean connect(String IpAddress, int port, String username, String password) {
		Socket s;
		try {
			s = new Socket(IpAddress, port);
		} catch (UnknownHostException e) {
			e.printStackTrace();
			return false;
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		
		try {
			reader = s.getInputStream();
			writer = s.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		
		// Used to negotiate options.
		new Thread() {
			public void run() {
				while(true) {
					byte byteRead = -1;
					byte[] output = {IAC, WONT, 0};
					try {
						byteRead = (byte)reader.read();
						if(byteRead == IAC) {
							byteRead = (byte)reader.read();
							if(byteRead == DO) {
								byteRead = (byte)reader.read();
								output[2] = byteRead;
								writer.write(output);
							}
						} else {
							System.out.print((char)byteRead);
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}.start();
		
		try {
			Thread.sleep(DELAY_TIME);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		try {
			writer.write(toTelnet(username));
			Thread.sleep(DELAY_TIME);
			writer.write(toTelnet(password));
			Thread.sleep(DELAY_TIME);
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	public boolean sendCommands(String[] commands) {
		try {
			for(int i = 0; i < commands.length; i++) {
				writer.write(toTelnet(commands[i]));
				Thread.sleep(DELAY_TIME);
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
}
