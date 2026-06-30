import java.io.*;
import java.net.*;

class ClientThread implements Runnable {

		private Socket clientSocket;

		public ClientThread(Socket connSocket) {
				clientSocket = connSocket;
		}

		public void run() {
				BufferedReader inFromServer = null;
				try {
						inFromServer = new BufferedReader(new InputStreamReader(
												(this.clientSocket).getInputStream()));
				} catch (IOException e) {
						e.printStackTrace();
				}
				while (true) {
						String serverMessage = null;
						try {
								serverMessage = inFromServer.readLine();
						} catch (SocketException e) {
								System.out.println("The client is closing");
								System.exit(1);
						} catch (Exception e1) {
						}
						if (serverMessage == null) {
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
				Socket clientSocket = null;
				try{
						clientSocket = new Socket(ip, 9876);
				} catch(Exception e){
						System.out.println("invalid IP address");
						System.exit(1);
				}
				BufferedReader inFromServer = new BufferedReader(new InputStreamReader(
										(clientSocket).getInputStream()));
				System.out.println(inFromServer.readLine());
				DataOutputStream outToServer = new DataOutputStream(
								clientSocket.getOutputStream());

				outToServer.writeBytes(inFromUser.readLine() + '\n');

				Thread t = new Thread(new ClientThread(clientSocket));
				t.start();
				while (true) {
						String message = inFromUser.readLine();
						if (message.equals("cmd close")) {
								clientSocket.close();
								System.out.println("The client is closing.");
								System.exit(1);
						}
						outToServer.writeBytes(message + '\n');
				}
		}
}
