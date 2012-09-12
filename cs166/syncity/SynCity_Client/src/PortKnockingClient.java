import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;


public class PortKnockingClient {
	
	/**
	 * PortKnockingClient should knock 5 input ports of the input server.
	 * The response from the server is not expected.
	 * This program should pause for 500 milliseconds between the knocks. 
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
		
		//set up for socket connection.
		InetAddress addr;
		InetSocketAddress sockAddr;
		try{
			addr = InetAddress.getByName(serverIP);
			for(int a = 0; a < 5; a++){
				Socket sock = new Socket();
				sockAddr = new InetSocketAddress(addr, port[a]);
				try{
					//connect to the server and timeout in 500 millisecs.
					sock.connect(sockAddr, 500);
				}catch(SocketTimeoutException e){
					//This part should be reached since the server will not respond at all.
					System.out.println(" knocked port[" + port[a] + "].");
				}
			}
			System.out.println("Finished portknocking.");
		}catch(Exception e){
			System.out.println("ERROR: ");
			System.err.println(e);
			System.exit(0);
		}
		
		
	}
}
