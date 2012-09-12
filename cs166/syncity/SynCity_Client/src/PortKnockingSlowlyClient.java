import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;


public class PortKnockingSlowlyClient {
	
	/**
	 * PortKnockingSlowlyClient should knock 5 input ports of the input server.
	 * The response from the server is not expected.
	 * This program should pause for 5500 milliseconds between the knocks. 
	 */
	public static void main(String[] args){
		
		//Check if the right number of arguments were inputed.
		if(args.length < 6){
			System.out.println("portknock server_ip port1 port2 port3 port4 port5");
		}
		
		//Assign ports as given by the arguments.
		String serverIP = args[0];
		int[] port = new int[5];
		for(int a = 0; a < 5; a++){
			port[a] = Integer.parseInt(args[a+1]);
		}
		
		InetAddress addr;
		InetSocketAddress sockAddr;
		try{
			addr = InetAddress.getByName(serverIP);
			for(int a = 0; a < 5; a++){
				Socket sock = new Socket();
				sockAddr = new InetSocketAddress(addr, port[a]);
				try{
					sock.connect(sockAddr, 500);
				}catch(SocketTimeoutException e){
					System.out.println(" knocked port[" + port[a] + "].");
				}
				Thread.sleep(5000);
			}
			System.out.println("Finished portknocking.");
		}catch(Exception e){
			System.out.println("ERROR: ");
			System.err.println(e);
			System.exit(0);
		}
		
		
	}
}
