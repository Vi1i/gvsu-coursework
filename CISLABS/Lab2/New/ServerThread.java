import java.io.*;
import java.net.*;
import java.util.ArrayList;


class ServerThread implements Runnable{
	
	private Socket connectionSocket;
	private static ArrayList<Socket> currentSockets = new ArrayList<Socket>();
	private ArrayList<Groups> groups = new ArrayList<Groups>();
	
	public ServerThread(Socket connSocket){
		connectionSocket = connSocket;		
	}
	
	public void sendMessage(String message) throws IOException{
		System.out.println(currentSockets);
		System.out.println(message + " " + currentSockets.size());
		for(int i = 0; i < currentSockets.size(); i++){
			System.out.println("IM SENDING SOMETHING TO " + currentSockets.get(i));
			DataOutputStream tempOutToClient = null;
			tempOutToClient = new DataOutputStream(currentSockets.get(i).getOutputStream());
			tempOutToClient.writeBytes(message + '\n');
		}
		
	}

	public void createGroup(String groupName, String adminName){
		groups.add(new Groups(groupName, adminName));
	}

	public void removeGroup(String groupName){
		
	}

	
	public void run(){
		BufferedReader inFromClient = null;
		try {
			inFromClient = new BufferedReader(
					new InputStreamReader((this.connectionSocket).getInputStream()));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		DataOutputStream outToClient = null;
		try {
			outToClient = new DataOutputStream(
					(this.connectionSocket).getOutputStream());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		while(true){
			String clientMessage = null;
			try {
				clientMessage = inFromClient.readLine();
				if(clientMessage == null){
					System.out.println("Connection has failed, closing socket");
					connectionSocket.close();
					outToClient.close(); 
					inFromClient.close(); 
				}
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			System.out.println("1");
			System.out.println("The client said " + clientMessage);
			try {
				sendMessage(clientMessage);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
			if(clientMessage.equals("cmd close")){
					try {
						connectionSocket.close();
					} catch (IOException e){
						e.printStackTrace();
					}
			}
		}
	}
	
	
	
	

public static void main(String argv[]) throws IOException{
	
    
    int serverPort = 9876;
	ServerSocket listenSocket = new ServerSocket(serverPort);
	System.out.println("You are listening on " + serverPort);
		
	
	while(true){
		System.out.println("111");
		Socket connectionSocket = listenSocket.accept();
		currentSockets.add(connectionSocket);
		System.out.println("The current number of users is: " + currentSockets.size());		
		System.out.println("222");
		Thread t = new Thread(new ServerThread(connectionSocket));
		t.start();
		System.out.println("333");
	}
}
}
		
class user {
private Socket socket;
private String name;
	public user(Socket socket, String name){
		this.socket = socket;
		this.name = name;
	}
	public Socket getSocket(){
		return socket;
	}

	public boolean isName(String inName){
		return (inName == name);
	}
	public String getName(){
		return name;
	}

}

class groups {
	ArrayList<String> users = new ArrayList<String>();
	String groupName;
	String adminName;
	
	public groups(String name, String adminName){
		groupName = name;
		this.adminName = adminName;
		users.add(adminName);
	}
	
	public void addUser(String newUserName){
		users.add(newUserName);
	}

	public void removeUser(String removeUserName){
		users.remove(removeUserName);
	}

}




