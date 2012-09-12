import java.io.IOException;
import java.util.TimerTask;
import java.util.concurrent.ConcurrentHashMap;


public class ClosePortTask extends TimerTask{
	
	ConcurrentHashMap<String, Integer> state;
	String srcAddress;
	
	public ClosePortTask(ConcurrentHashMap<String, Integer> list, String sourceAddress){
		state = list;
		srcAddress = sourceAddress;
	}
	
	@Override
	public void run() {
		
		//if the client has already sent a packet through port 22, do nothing.
		if(state.get(srcAddress) == null || state.get(srcAddress) != -5){
			return;
		}
		
		//otherwise,
		//close the port and update the list.
		String command = "iptables -D INPUT -p tcp -s " + srcAddress + " --dport 22 -j ACCEPT";
		
		Runtime runtime = Runtime.getRuntime();
		try {
			runtime.exec(command);
			System.out.println(srcAddress + ": closed the port 22.");
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		state.remove(srcAddress);
	}

}
