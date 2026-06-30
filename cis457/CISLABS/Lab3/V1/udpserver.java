import java.io.*;
import java.net.*;

class chacheObject {

}

class udpserver {
	public static void main(String args[]) throws Exception {
		DatagramSocket serverSocket = null;
		byte[] recvData = null;
		byte[] rootServerData = null;
		byte[] sendData = null;
		DatagramPacket recvPacket = null;
		DatagramPacket sendPacket = null;
		DatagramPacket sendToClientPacket = null;
		String message = null;
		InetAddress sourceIPAddress = null;
		InetAddress destIPAddress = null;
		int port = 0;
		String newMessage = null;
		udpserver server = new udpserver();

		while (true) {
			serverSocket = new DatagramSocket(9876);
			recvData = new byte[1024];
			rootServerData = new byte[1024];
			sendData = new byte[1024];
			recvPacket = new DatagramPacket(recvData, recvData.length);
			serverSocket.receive(recvPacket);

			sourceIPAddress = recvPacket.getAddress();
			destIPAddress = InetAddress.getByName("198.41.0.4");
			port = recvPacket.getPort();
			System.out.println("Test: "+ (new String(recvData, "US-ASCII")));
			for (int i = 0; i < 1024; i++) {
				System.out.print(recvData[i]+"|");
			}
			System.out.println(sourceIPAddress + " " + port);
			//TODO check cache

			// build packet
			sendPacket = new DatagramPacket(recvData, recvData.length,
					destIPAddress, 53);
			
			// make recursive call
			try{
			rootServerData = (server.recursiveResolver(sendPacket)).getData();
			}catch(Exception e){
				e.printStackTrace();
			}

			// Add it to cache

			// respond to client
			sendToClientPacket = new DatagramPacket(rootServerData,
					rootServerData.length, sourceIPAddress, port);
			serverSocket.send(sendToClientPacket);
			serverSocket.close();
		}
	}

	private DatagramPacket recursiveResolver(DatagramPacket packet) throws Exception {
		//if(!keepSearching(packet.getData())){
		//	return packet;
		//}
		
		DatagramSocket outSock = new DatagramSocket();
		outSock.setSoTimeout(5000);
		outSock.send(packet);
		byte[] inData = new byte[1024];
		DatagramPacket inPack =  new DatagramPacket(inData, inData.length);
		outSock.receive(inPack);
		byte[] recData = inPack.getData();
		for(int z= 0; z<200;z++){
			System.out.print(recData[z]+ "*");
		}
		return inPack;
		//return recursiveResolver(inPack); 
	}

	private boolean keepSearching(byte[] pack){
		return false;
	}
	private InetAddress getNextServerAddress(byte[] pack){
		return null;
	}
}
