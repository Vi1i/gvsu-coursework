import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;


class User{
	private String username;
	private InetAddress IPAddress;
	private Socket userSocket;
	private DataOutputStream OutToClient;

	public User(InetAddress IP, String name, Socket userSocket){
		username = name;
		IPAddress = IP;
		this.userSocket = userSocket;
		try {
			OutToClient = new DataOutputStream(userSocket.getOutputStream());
		}catch (IOException e){
			System.out.println("Error creating output Stream");
		}
	}
	
	public String toString(){
		return "Username: " + username + " IP: " + IPAddress;
	}
	
	public String getUserName(){
		return username;
	}
	public Socket getSocket(){
		return userSocket;
	}
	
	public void sendUserMessage(String message){
		try{
			OutToClient.writeBytes(message);		
		}catch(IOException e){
			System.out.println("Error sending to user");
		}
	}
		
}

class Group{
	String name;
	String adminName;
	ArrayList<String> users = new ArrayList<String>();

	public Group(String adminName, String groupName){
		this.name = groupName;
		this.adminName = adminName;
		users.add(adminName);
	}

	public boolean isAdmin(String user){
		return(adminName == user);
	}

	public void addUser(String userName){
		users.add(userName);
	}

	public void removeUser(String userName){
		users.remove(userName);	
	}
	public String getName(){
		return name;	
	}
}


class ServerThread implements Runnable{
	
	private Socket connectionSocket;
	//private static ArrayList<Socket> currentSockets = new ArrayList<Socket>();
	private static ArrayList<User> currentUsers = new ArrayList<User>();
	private ArrayList<Group> currentGroups = new ArrayList<Group>();
	private volatile boolean execute;

	public ServerThread(Socket connSocket){
		connectionSocket = connSocket;		
	}
	
	public void sendMessage(String message) throws IOException{
		
		DateFormat df = new SimpleDateFormat("HH:mm:ss");
		Date datenow = new Date();		
		String completeMessage = ('[' + df.format(datenow) + "] " + message + '\n');
		System.out.print(completeMessage);
		for(User t : currentUsers){
			DataOutputStream tempOutToClient = null;
			tempOutToClient = new DataOutputStream(t.getSocket().getOutputStream());
			tempOutToClient.writeBytes(completeMessage);
		}
		
	}

	public void sendUserList(String toUser){
		String List = "Current Users: ";
		User tempUser = null;
		for(User t : currentUsers){
			List += (t.getUserName()+ ", ");
			if(t.getUserName() == toUser){
				tempUser = t;
			}
		}
		List += "\n";
		tempUser.sendUserMessage(List);
		
		
	}

	public void sendToUser(String message,String userName){
		for(User t : currentUsers){
			if(t.getUserName()==userName){
				t.sendUserMessage(message);
			}
		}
	}
	public void createGroup(String admin, String groupName){
		currentGroups.add(new Group(admin, groupName));
	}
	public void groupCommands(String command){
		String sendingToUser = command
		for(User t: currentUsers)
	}
	
	public void stopExecuting(){
		this.execute = false;
	}

	
	public void run(){
		BufferedReader inFromClient = null;
		String sendToUserName = null;
		String sendToGroupName = null;
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
			currentUser = new User(userIP, username, this.connectionSocket);
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
			
			if(clientMessage == null || clientMessage.equals("cmd close")){
				stopExecuting();
				
				try{
				this.connectionSocket.close();
				} catch (Exception e2){
				}
				currentUsers.remove(currentUser);
				try{
				sendMessage(currentUser.getUserName() + " has left the chat room.");
				} catch (Exception e4) {
				}
				
				currentUsers.remove(currentUser);
				break;		
			
			} else if(clientMessage.equals("ALLUSERS")){
				sendUserList(currentUser.getUserName());
			}else if(clientMessage.equals("CREATEGROUP")){
				createGroup(currentUser.getUserName());
			}else if(clientMessage.startsWith("USERSEND:")){
				outToClient.writeBytes("Who would you like to send this message to? \n");
				sendToUserName = inFromClient.readLine();
				outToClient.writeBytes("What is your message for " + sendToUserName + "? \n");
				clientMessage = inFromClient.readLine();
				sendToUser(clientMessage, sendToUserName);
			}else {

				try {
					sendMessage(completeMessage);
				} catch (Exception e) {
				
					stopExecuting();
				
					try{
					this.connectionSocket.close();
					} catch (Exception e2){
					}
					//currentSockets.remove(this.connectionSocket);
					try{
					sendMessage(currentUser.getUserName() + " has left the chat room.");
					} catch (Exception e4) {
					}
				
					currentUsers.remove(currentUser);
					break;		
				
			}	}	
			
			
		}
	}
	
	
	
	

public static void main(String argv[]) throws IOException{
	
    
    int serverPort = 9876;
	ServerSocket listenSocket = new ServerSocket(serverPort);
	System.out.println("You are listening on " + serverPort);
		
	while(true){
		Socket connectionSocket = listenSocket.accept();
		//currentSockets.add(connectionSocket);
		System.out.println("The current number of users is: " + currentUsers.size()+1);		
		Thread t = new Thread(new ServerThread(connectionSocket));
		t.start();
	}
}
}
		





