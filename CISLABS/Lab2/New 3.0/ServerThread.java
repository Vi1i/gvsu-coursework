import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;


class User{
	private String username;
	private InetAddress IPAddress;
	private Socket userSocket;
	private DataOutputStream OutToClient;
	private boolean isAdmin;

	public User(InetAddress IP, String name, Socket userSocket, boolean isAdmin){
		username = name;
		IPAddress = IP;
		this.userSocket = userSocket;
		this.isAdmin = isAdmin;
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

	public boolean getAdmin(){
		return isAdmin;
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
	public void sendMessage(String message, ArrayList<User> currentUsers){
		for(User u : currentUsers){
			if(users.contains(u.getUserName())){
				u.sendUserMessage(message); 
			}
		}
	}
}


class ServerThread implements Runnable{
	
	private Socket connectionSocket;
	//private static ArrayList<Socket> currentSockets = new ArrayList<Socket>();
	private static ArrayList<User> currentUsers = new ArrayList<User>();
	private static ArrayList<Group> currentGroups = new ArrayList<Group>();
	private volatile boolean execute;
	private static String adminUser;

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

	public void sendToUser(String message,String userNameTo, String userNameFrom){
		for(User t : currentUsers){
			if((t.getUserName()).equals(userNameTo)){
				t.sendUserMessage(("User: " + userNameFrom + " Says: ")+message + "\n");
			}
		}
	}
	public int createGroup(String admin, String groupName){
		boolean groupExists = false;
		for(Group g : currentGroups){
			if ((g.getName()).equals(groupName)){
				groupExists = true;
			}
		}
		if(!groupExists){
			currentGroups.add(new Group(admin, groupName));
			return 1;
		}
		return 0;
	}
	public void sendToGroup(String message, String groupName, String fromUser){
		for(Group g : currentGroups){
			if((g.getName()).equals(groupName)){
				g.sendMessage((("User: " + fromUser+ " Says for group "+ groupName + ": ")+message + "\n"), currentUsers);
			}
		}

	}
	public int addRemoveUserToGroup(String userName, String groupName, int addRem){
		for(Group g : currentGroups){
			if((g.getName()).equals(groupName)){
				if(addRem == 1){
					g.addUser(userName);
				}		
				else{
					g.removeUser(userName);
				}
				return 1;
			}
		}
		return 0;

	}
	
	public void stopExecuting(){
		this.execute = false;
	}
	public boolean checkUserName(String user){
		for(User t : currentUsers){
			if((t.getUserName()).equals(user)){
				return false;
			}
		}
		if(user.equals(null)){
			return false;
		}
		return true;
	}
	public int killUser(String fromUser, String toUser){
		boolean isAdmin = false;
		for(User u: currentUsers){
			if(u.getUserName().equals(fromUser))
				if(u.getAdmin())
					isAdmin = true;
		}
		if(isAdmin){
			for(User us: currentUsers){
				if(us.getUserName().equals(toUser)){
					currentUsers.remove(us);
					us.sendUserMessage("KILLEDBYADMIN:");
					try{
					us.getSocket().close();
					}catch(Exception e){}
					return 1;
				}
			}
		}
		return 0;
	}
	
	public void run(){
		BufferedReader inFromClient = null;
		String sendToUserName = null;
		String sendToGroupName = null;
		String groupName = null;
		String userName=null;
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
		while(true){
			try {
				username = inFromClient.readLine();
				if(!(checkUserName(username))){
					outToClient.writeBytes("username not accepted, please enter a different name: \n");
				} else{
					break;
				}
			} catch (IOException e2) {
				// TODO Auto-generated catch block
				e2.printStackTrace();
			}
		}
				User currentUser = null;
		
		if(username != null){
			InetAddress userIP = this.connectionSocket.getInetAddress();
			currentUser = new User(userIP, username, this.connectionSocket,(currentUsers.size()==0));
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
			
			} else if(clientMessage.equals("ALLUSERS:")){
				sendUserList(currentUser.getUserName());
			}else if(clientMessage.equals("CREATEGROUP:")){
				try{
					outToClient.writeBytes("What name would you like to give your group? \n");
					groupName = inFromClient.readLine();
					if(createGroup(currentUser.getUserName(), groupName) == 1){
						outToClient.writeBytes("Group created successfully \n");
					}else{
						outToClient.writeBytes("Group creation failed");
					}				
				} catch(Exception e){
				}
			}else if(clientMessage.startsWith("USERSEND:")){
				try{
					outToClient.writeBytes("Who would you like to send this message to? \n");
					sendToUserName = inFromClient.readLine();
					outToClient.writeBytes("What is your message for " + sendToUserName + "? \n");
					clientMessage = inFromClient.readLine();
					sendToUser(clientMessage, sendToUserName, currentUser.getUserName());
				} catch(Exception e){
				}
			}else if(clientMessage.startsWith("GROUPSEND:")){
				try{
					outToClient.writeBytes("What group would you like to send this message to? \n");
					sendToGroupName = inFromClient.readLine();
					outToClient.writeBytes("What is your message for the group " + sendToGroupName + "? \n");
					clientMessage = inFromClient.readLine();
					sendToGroup(clientMessage, sendToGroupName, currentUser.getUserName());
				} catch(Exception e){
				}
			}else if(clientMessage.startsWith("JOINGROUP:")){
				try{
					outToClient.writeBytes("What group would you like to join? \n");
					groupName = inFromClient.readLine();
					if (addRemoveUserToGroup(currentUser.getUserName(), groupName,1)==0){
						outToClient.writeBytes("Group does not exist\n");
					}	
				} catch(Exception e){
				}
			}else if(clientMessage.startsWith("LEAVEGROUP:")){
				try{
					outToClient.writeBytes("What group would you like to Leave? \n");
					groupName = inFromClient.readLine();
					if(addRemoveUserToGroup(currentUser.getUserName(), groupName,0)==0){
						outToClient.writeBytes("Group does not exist\n");
					}else{
						outToClient.writeBytes("Leave succesful \n");
					}
					} catch(Exception e){
				}
			}else if(clientMessage.startsWith("KILLUSER:")){
				try{
					outToClient.writeBytes("What user would you like to kill? \n");
					userName = inFromClient.readLine();
					if(killUser(currentUser.getUserName(), userName)==0){
						outToClient.writeBytes("Could not kill specified user \n");
					}else{
						outToClient.writeBytes("Killed user: " + userName + "\n");
					}
					} catch(Exception e){
				}
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
		System.out.println("The current number of users is: " + (currentUsers.size()+1));		
		Thread t = new Thread(new ServerThread(connectionSocket));
		t.start();
	}
}
}
		





