import java.io.*;
import java.net.*;
import java.util.ArrayList;


class ServerThread implements Runnable{
	
	private Socket connectionSocket;

	public ServerThread(Socket connSocket){
		connectionSocket = connSocket;		
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
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			System.out.println("1");
			System.out.println("The client said " + clientMessage);
			try {
				outToClient.writeBytes(clientMessage + '\n');
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
			if(clientMessage.equals("cmd close")){
					try {
						connectionSocket.close();
					} catch (IOException e)
						e.printStackTrace();
					}
			}
		}
	
	
	
	}

public static void main(String argv[]) throws IOException{
	
    
        int serverPort = 9876;
	ServerSocket listenSocket = new ServerSocket(serverPort);
	System.out.println("You are listening on " + serverPort);
	ArrayList<Socket> currentSockets = new ArrayList<Socket>();	
	
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
		





