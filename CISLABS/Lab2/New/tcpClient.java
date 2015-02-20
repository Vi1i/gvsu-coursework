import java.io.*;
import java.net.*;

class tcpclient {


	public static void main(String args[]) throws Exception {
		tcpclient client = new tcpclient();
BufferedReader inFromUser = new BufferedReader(new InputStreamReader(
				System.in));
		System.out.println("Type IP address of the server:");
		String ip = inFromUser.readLine();
		Socket clientSocket = new Socket(ip, 9876);		
		System.out.println("IP address accepted, please enter your username:");
		String username = inFromUser.readLine();
		
		Thread recThread = new Thread(new clientRecieve(clientSocket));
		Thread sendThread = new Thread(new clientSend(clientSocket, username));
		recThread.start();
		sendThread.start();
		/*
		DataOutputStream outToServer = new DataOutputStream(
				clientSocket.getOutputStream());
		BufferedReader inFromServer = new BufferedReader(new InputStreamReader(
				clientSocket.getInputStream()));

		
		System.out.println("Type cmd close to exit."+ "\n To message everyone just type, to message a group type Group:groupname message, to message a person type User:username message.");
		while (true){
		System.out.println("Enter a message: ");
		
		String message = inFromUser.readLine();
		if (message.equals("cmd close")){
			//add server output message for leaving 
			clientSocket.close();
		}
		outToServer.writeBytes(message + '\n');
		String serverMessage = inFromServer.readLine();

		System.out.println("Got from server: " + serverMessage);
		
		}*/
	}
	}

class clientSend implements Runnable {
	String sendMessage;
	Socket clientSocket;
	BufferedReader userInput;
	DataOutputStream outToServer;
	String username;

	public clientSend(Socket clientSocket, String username) {
		this.clientSocket = clientSocket;
		this.username = username;
		
	}

	public void run() {
		userInput = new BufferedReader(new InputStreamReader(System.in));
System.out.println("created user input buffer");
try{		outToServer = new DataOutputStream(clientSocket.getOutputStream());}catch (IOException e){
	System.out.println("Error making output stream to server");
	System.exit(1);
}
	System.out.println("Created data output stream");
		outToServer.writeBytes(username);
	while(true){
		System.out.println("enter message to send:");
		try{	
			sendMessage = userInput.readLine();			
			outToServer.writeBytes(sendMessage + "\n");
		} catch(IOException z){
			System.out.println("Error sending message to server");
		}
	}
}
}

class clientRecieve implements Runnable {
	String message;
	Socket clientSocket;
	BufferedReader inFromServer;

	public clientRecieve(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	public void run() {
		try {
			inFromServer = new BufferedReader(new InputStreamReader(
					clientSocket.getInputStream()));
		} catch (IOException e) {
			System.out.println("error getting server message, closing program");
			System.exit(1);
		}
	while(true){
		try{
		message = inFromServer.readLine();
		if(message == null){
			System.out.println("Server has crashed, exiting program");
			System.exit(1);
		}
		}catch(IOException e){
			System.out.println("Error reading message from server");
		}
		
			System.out.println("Got from server:" + message);
		

	}

}
}
