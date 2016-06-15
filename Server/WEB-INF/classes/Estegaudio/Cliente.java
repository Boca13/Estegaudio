package Estegaudio;

public class Cliente {
	String id;
	String ip;
	String port;
	
	public Cliente(String ID, String IP, String Port){
		id=ID;
		ip=IP;
		port=Port;
	}
	
	public String getId(){
		return id;
	}
	
	public String getIp(){
		return ip;
	}
	
	public String getPort(){
		return port;
	}
}
