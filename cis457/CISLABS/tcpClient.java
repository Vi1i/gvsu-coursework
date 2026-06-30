import java.io.*;
import java.net.*;

class tcpclient {
	String currIp;

	public static void main(String args[]) throws Exception {
		tcpclient client = new tcpclient();
		Socket clientSocket = new Socket("127.0.0.1", 9876);

		
		DataOutputStream outToServer = new DataOutputStream(
				clientSocket.getOutputStream());
		BufferedReader inFromServer = new BufferedReader(new InputStreamReader(
				clientSocket.getInputStream()));

		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(
				System.in));
				
		while (true){
		System.out.println("Enter a message: ");
		
		String message = inFromUser.readLine();
		if (message.equals("cmd close")){
			/*add server output message for leaving */
			clientSocket.close();
		}
		outToServer.writeBytes(message + '\n');
		String serverMessage = inFromServer.readLine();

		System.out.println("Got from server: " + serverMessage);
		
	}
	}

	public tcpclient() {
		System.out
				.println("Welcome to simple chat by Lucas Crandle and Brendan Koning");
		System.out
				.println("Please enter your IP Address so that you can communicate with other members:");
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		try {
			currIp = br.readLine();
		} catch (IOException ioe) {
			System.out.println("IO error trying to read your IP Address!");
			System.exit(1);
		}
		System.out.println("Ip Address accepted, Start chatting now:");
	}

}
