import java.util.concurrent.ConcurrentHashMap;

import net.sourceforge.jpcap.capture.PacketCapture;


public class Server {
	
	public static void main(String[] args) {
		//start the capturing engine
		PacketCapture pc = new PacketCapture();
		
		int[] port = {1111,2222,3333,4444,5555};
		ConcurrentHashMap<String, Integer> state = new ConcurrentHashMap<String,Integer>();
		PortKnockingHandler handler = new PortKnockingHandler(port, state);
		
		try{
			//find and open the device
			String device = pc.findDevice();
			pc.open(device, false);
			
			
			//set filter
			pc.setFilter("", true); //do not capture any outgoing packets
			pc.setFilter("tcp", true); // get only tcp packets
			//only listen to port 1111, 2222, 3333, 4444, 5555, or 22
			pc.setFilter("!(src net 127.0.0.1) && tcp && ( dst port 1111 || dst port 2222 || dst port 2222 || dst port 3333" +
							" || dst port 4444 || dst port 5555 || dst port 22)", true);
			

			
			//add a PacketListener
			pc.addPacketListener(handler);
			
			
		}catch(Exception e){
			System.out.println(e);
		}
		
		try{
			pc.capture(-1);
		}catch(Exception e){
			System.out.println(e);
		}
		
	}
}
