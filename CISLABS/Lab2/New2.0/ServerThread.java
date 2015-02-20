import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;


class User{
	private String username;
	private InetAddress IPAddress;
	
	
	public User(InetAddress IP, String name){
		username = name;
		IPAddress = IP;
	}
	
	public String toString(){
		return "Username: " + username + " IP: " + IPAddress;
	}
	
	public String getUserName(){
		return username;
	}
		
}


class ServerThread implements Runnable{
	
	private Socket connectionSocket;
	private static ArrayList<Socket> currentSockets = new ArrayList<Socket>();
	private static ArrayList<User> currentUsers = new ArrayList<User>();
	private volatile boolean execute;

	public ServerThread(Socket connSocket){
		connectionSocket = connSocket;		
	}
	
	public void sendMessage(String message) throws IOException{
		
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		Date datenow = new Date();		
		String completeMessage = ('[' + df.format(datenow) + "] " + message + '\n');
		System.out.print(completeMessage);
		for(int i = 0; i < currentSockets.size(); i++){
			DataOutputStream tempOutToClient = null;
			tempOutToClient = new DataOutputStream(currentSockets.get(i).getOutputStream());
			tempOutToClient.writeBytes(completeMessage);
		}
		
	}
	
	public void stopExecuting(){
		this.execute = false;
	}

	
	public void run(){
		BufferedReader inFromClient = null;
		try {
			inFromClient = new BufferedReader(
					new InputStreamReader((this.connectionSocket).getInputStream()));
		} catch (SocketException e) {
		   	stopExecuting();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		DataOutputStream outToClient = null;
		try {
			outToClient = new DataOutputStream(
					(this.connectionSocket).getOutputStream());
		} catch (SocketException e) {
		   	stopExecuting();
		} catch (IOException e) {
			e.printStackTrace();
		}
				
		try {
			outToClient.writeBytes("Welcome to the chatroom! Choose a username: \n");
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		
		String username = null;
		
		try {
			username = inFromClient.readLine();
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		
		User currentUser = null;
		
		if(username != null){
			InetAddress userIP = this.connectionSocket.getInetAddress();
			currentUser = new User(userIP, username);
			System.out.println(currentUser);
			currentUsers.add(currentUser);		
			this.execute = true;
			try{
			sendMessage(currentUser.getUserName() + " has connected.");
			}
			catch (Exception e12){
			}
		}
		else{
			currentSockets.remove(this.connectionSocket);
			stopExecuting();
		}
		
		while(this.execute){
			String clientMessage = null;
			String completeMessage = null;
			try {
				clientMessage = inFromClient.readLine();
			} catch (SocketException e) {
				stopExecuting();
				break;
			} catch (IOException e1) {
				//e1.printStackTrace();
			}
			
			completeMessage = (currentUser.getUserName() + ": " + clientMessage);
			
			if(clientMessage == null){
				stopExecuting();
				
				try{
				this.connectionSocket.close();
				} catch (Exception e2){
				}
				currentSockets.remove(this.connectionSocket);
				try{
				sendMessage(currentUser.getUserName() + " has left the chat room.");
				} catch (Exception e4) {
				}
				
				currentUsers.remove(currentUser);
				break;		
			
			}
			try {
				sendMessage(completeMessage);
			} catch (SocketException e) {
				
				stopExecuting();
				
				try{
				this.connectionSocket.close();
				} catch (Exception e2){
				}
				currentSockets.remove(this.connectionSocket);
				try{
				sendMessage(currentUser.getUserName() + " has left the chat room.");
				} catch (Exception e4) {
				}
				
				currentUsers.remove(currentUser);
				break;		
				
			}catch (IOException e) {
				//e.printStackTrace();
			}
		
			if(clientMessage.equals("cmd close")){
					try {
						connectionSocket.close();
					} catch (IOException e){
						//e.printStackTrace();
					}
			}
			
			
			
		}
	}
	
	
	
	

public static void main(String argv[]) throws IOException{
	
    
    int serverPort = 9876;
	ServerSocket listenSocket = new ServerSocket(serverPort);
	System.out.println("You are listening on " + serverPort);
		
	while(true){
		Socket connectionSocket = listenSocket.accept();
		currentSockets.add(connectionSocket);
		System.out.println("The current number of users is: " + currentSockets.size());		
		Thread t = new Thread(new ServerThread(connectionSocket));
		t.start();
	}
}
}
		





