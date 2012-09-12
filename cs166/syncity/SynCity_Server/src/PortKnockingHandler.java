import net.sourceforge.jpcap.capture.PacketListener;
import net.sourceforge.jpcap.net.Packet;
import net.sourceforge.jpcap.net.TCPPacket;

import java.io.IOException;
import java.util.Timer;
import java.util.concurrent.*;


public class PortKnockingHandler implements PacketListener{
	
	/*
	 * state:
	 * this map will map the source IP address to a number representing the state of the IP address in portknocking.
	 * KEY: source IP address
	 * VALUE: the index of port array in which the last successful portknocking is stored, or
	 * 		  -(number of failed portknocking) until -4, or
	 * 		  -5 if the port succeeded in portknocking.
	 */
	ConcurrentHashMap<String, Integer> state;
	
	/*
	 * lastTimeConn:
	 * this maps the source IP address to the time when the last packet is received in milliseconds.
	 * KEY: source IP address
	 * VALUE: the time when the last packet is received.
	 */
	ConcurrentHashMap<String, Long> lastTimeConn;
	
	/*
	 * blacklist:
	 * this maps the source IP address to the time when the last received packet is blocked in milliseconds 
	 * specified by System.currentTimeinMilli method.
	 * KEY: source IP address
	 * VALUE: the time when the last received packet is blocked in milliseconds
	 */
	ConcurrentHashMap<String, Long> blacklist;
	
	
	int[] port;
	int portLeng;
	Runtime run;
	Timer timer;
	
	final long blockDelay = 5;	//blacklist time period
	final int openDelay = 5;	//period of opening the port 22
	final long portknockingTimeout = 5;	//timeout period for portknocking
	final int numOfAllowedAttempts = 3; //number of allowed failed portknocking
	
	public PortKnockingHandler(int[] portSeq, ConcurrentHashMap<String, Integer> stateMap){
		portLeng = portSeq.length;
		port = new int[portLeng];
		for(int i=0; i<portLeng; i++){
			port[i] = portSeq[i];
		}
		state = stateMap;
		run = Runtime.getRuntime();
		timer = new Timer();
		blacklist = new ConcurrentHashMap<String, Long>();
		lastTimeConn = new ConcurrentHashMap<String, Long>();
	}
	
	
	/**
	 * packetArrived: handles the portknocking sequence.
	 * The method analyze the packet's knocking and then opens the port 22 for 5 seconds or
	 * until the successful connection.
	 * 
	 * @param packet	the incoming packet
	 */
	@Override
	public void packetArrived(Packet packet) {
		
		//first get the source address
		TCPPacket tcpPack = null;
		tcpPack = (TCPPacket)packet;
		
		String srcAddress = tcpPack.getSourceAddress();
		if(srcAddress == null)//then just drop the packet.
			return;
		

		
		//check if the client is blacklisted
		Long lastConn = blacklist.get(srcAddress);
		if(lastConn != null){
			//if the client is blacklisted less than a 5 seconds ago, drop the packet
			if((System.currentTimeMillis() - lastConn.longValue() < (blockDelay * 1000))){
				return;
			}
			
			//otherwise, remove the client from the blacklist and proceed.
			blacklist.remove(srcAddress);
		}
		
		
		
		int destPort = tcpPack.getDestinationPort(); 
		Integer tempCurrentPort = state.get(srcAddress);
		
		//if the client has connected before,
		if(tempCurrentPort != null){
			
			int curPort = tempCurrentPort.intValue();
			
					
			//if the client succeeded in portknocking before the port 22 is closed,
			//accept the packet and close the port 22.
			if(curPort == -5){
				System.out.println(srcAddress + ": succeeded in receiving packet from the port 22.");
				String command = "iptables -D INPUT -p tcp -s " + srcAddress + " --dport 22 -j ACCEPT";
				
				try {
					run.exec(command);
					System.out.println(srcAddress + ": closed the port 22.");
				} catch (IOException e) {
					e.printStackTrace();
				}
				
				state.remove(srcAddress);
				return;
			}
			
			//else if(curPort is not -5 but is destined to port 22), simply drop the packet.
			if(destPort == 22)
				return;
			
				
			//otherwise
			
			//if the client sent the packet after timeout, 
			//set curPort to -1, since it failed the last attempt.
			Long lastTime = lastTimeConn.get(srcAddress);
			if(lastTime != null && (System.currentTimeMillis() - lastTime.longValue()) > (portknockingTimeout*1000)){
				System.out.println(srcAddress + ": TIMEOUT!");
				curPort = -1;
				state.put(srcAddress, -1);
				lastTimeConn.remove(srcAddress);
			}
			
			//if first in portknocking sequence
			if(curPort < 0){
				//if succeeded in the current portknocking sequence, update the list.
				if(destPort == port[0]){
					System.out.println(srcAddress + ": knocked port[" + port[0]+"]");
					state.put(srcAddress, new Integer(0));
					lastTimeConn.put(srcAddress, new Long(System.currentTimeMillis()));
				}
				//if previous failed attempts + this failed attempt = number of allowed failed attempts,
				//add the client to the blacklist.
				else if((curPort-1) == (-1 * numOfAllowedAttempts)){
					System.out.println(srcAddress + ": added to the blacklist.");
					state.remove(srcAddress);
					lastTimeConn.remove(srcAddress);
					blacklist.put(srcAddress, System.currentTimeMillis());
				}
				//otherwise,
				//increment the failed attempts of the clients.
				else{
					System.out.println(srcAddress + ": failed in portknocking!");
					state.put(srcAddress, curPort-1);
					lastTimeConn.remove(srcAddress);
				}
				return;
			}
			
			//if the portknocking is in the last step
			if(curPort == portLeng-2){
				//if the knocking port matches the last sequence port,
				//open the port 22 for the client and close it after 5 secs.
				if(destPort == port[portLeng-1]){
					System.out.println(srcAddress + ": succeeded in portknocking");

					String command = "iptables -A INPUT -p tcp -s " + srcAddress + " --dport 22 -j ACCEPT";
					
					try {
						run.exec(command);
					} catch (IOException e) {
						e.printStackTrace();
					}
					
					state.put(srcAddress, new Integer(-5));
					lastTimeConn.remove(srcAddress);
					
					timer.schedule(new ClosePortTask(state, srcAddress), openDelay*1000);
				}
				//if failed,
				//mark the client with 1 failed attempt.
				else{
					System.out.println(srcAddress + ": failed in portknocking!");
					state.put(srcAddress, -1);
					lastTimeConn.remove(srcAddress);
				}
				return;
			}
			
			//if nth in portknocking sequence,
			for(int a =0; a<portLeng-2; a++){
				//if the client is in ath portknocking sequence
				if(curPort == a){				
					//if succeeded in (a+1)th portknocking,
					//update the list.
					if(destPort == port[a+1]){
						System.out.println(srcAddress + ": knocked port[" + port[a+1] +"]");
						state.put(srcAddress, new Integer(a+1));
						lastTimeConn.put(srcAddress, System.currentTimeMillis());
					}
					//if failed,
					//mark the client with 1 failed attempts.
					else{
						System.out.println(srcAddress + ": failed in portknocking!");
						state.put(srcAddress, -1);
						lastTimeConn.remove(srcAddress);
					}
					return;
				}
			}
		}
		
		//if the client has NOT connected before
		else{
			//if the client succeed the first portknocking sequence,
			//add the client to the list.
			if(destPort == port[0]){
				System.out.println(srcAddress + ": knocked port[" + port[0] +"]");
				state.put(srcAddress, new Integer(0));
				lastTimeConn.put(srcAddress, new Long(System.currentTimeMillis()));
			}
			//if failed,
			//mark the client with 1 failed attempts.
			else{
				System.out.println(srcAddress + ": failed in portknocking!");
				state.put(srcAddress, -1);
			}
			return;
		}
	}
}
