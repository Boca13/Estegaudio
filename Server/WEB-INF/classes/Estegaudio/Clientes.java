package Estegaudio;

import java.util.*;

public class Clientes {
	public List<Cliente> clientes = new LinkedList<Cliente>();
	
	public boolean existe(String id, String ip, String port){
		if(clientes.size()!=0){
			int c;
			for(c=0; c<clientes.size(); c++){
				String pid=clientes.get(c).id;
				String pip=clientes.get(c).ip;
				String pport=clientes.get(c).port;
				if((clientes.get(c).id.equals(id))&&(clientes.get(c).ip.equals(ip))&&(clientes.get(c).port.equals(port)))
					return true;
			}
			}
			return false;
	}
	
	public boolean existe(String id){
		if(clientes.size()!=0){
		int c;
		for(c=0; c<clientes.size(); c++){
			if(clientes.get(c).id.equals(id))
				return true;
		}
		}
		return false;
	}
	
	public int buscar(String id){
		int c;
		for(c=0; c<clientes.size(); c++){
			if(clientes.get(c).id.equals(id))
				return c;
		}
		return -1;
	}
}
