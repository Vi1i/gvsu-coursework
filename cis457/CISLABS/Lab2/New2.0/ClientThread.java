import java.io.*;
import java.net.*;

class ClientThread implements Runnable{
	
	private Socket clientSocket;
	
	public ClientThread(Socket connSocket){
		clientSocket = connSocket;
	}
	
	public void run(){
		BufferedReader inFromServer = null;
		try {
			inFromServer = new BufferedReader(
						new InputStreamReader((this.clientSocket).getInputStream()));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		while(true){
		String serverMessage = null;
		try {
			serverMessage = inFromServer.readLine();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if(serverMessage == null){
			System.out.println("Server has crashed, exiting program");
			System.exit(1);
		}
		System.out.println(serverMessage);
		}
	}
	
	public static void main(String args[]) throws Exception {
		
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(
				System.in));
		System.out.println("Type IP address of the server:");
		String ip = inFromUser.readLine();
		Socket clientSocket = new Socket(ip, 9876);	
		BufferedReader inFromServer = new BufferedReader(
						new InputStreamReader((clientSocket).getInputStream()));
		System.out.println(inFromServer.readLine());
		DataOutputStream outToServer = new DataOutputStream(
				clientSocket.getOutputStream());
		
		outToServer.writeBytes(inFromUser.readLine() + '\n');
		
		Thread t = new Thread(new ClientThread(clientSocket));
		t.start();
		
		System.out.println("Type cmd close to exit."+ "\n To message everyone just type, to message a group type Group:groupname message, to message a person type User:username message.");
		
		while (true){
			String message = inFromUser.readLine();
				if (message.equals("cmd close")){
					/*add server output message for leaving */
					clientSocket.close();
				}
				outToServer.writeBytes(message + '\n');
		}

		
	}
}
	

