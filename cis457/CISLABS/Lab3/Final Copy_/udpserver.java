import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;



class cacheObject {
	String websiteName;
	int timeToLive;
	byte[] pack;
	Date creationDate;

	public cacheObject(String websiteName, int timeToLive, byte[] pack) {
		this.websiteName = websiteName;
		this.timeToLive = timeToLive;
		this.pack = pack;
		creationDate = new Date();
	}

	public String getWebsiteName() {
		return websiteName;
	}

	public int getTTL() {
		return timeToLive;
	}

	public boolean isExpired() {
		Date currentDate = new Date();
		if (((currentDate.getTime() - creationDate.getTime()) / 1000) > timeToLive) {
			return true;
		}
		return false;
	}

	public byte[] getPack() {
		return pack;
	}

}

class udpserver implements Runnable {
	public ArrayList<cacheObject> cache = new ArrayList<cacheObject>();

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
		
		/*Scanner in = new Scanner(System.in);
		System.out
				.println("Enter the port number you would like to listen on:");
		int listenPort = in.nextInt();
		*/
		
		int listenPort = args[0];
		
		
		
		(new Thread(server)).start();
		while (true) {
			serverSocket = new DatagramSocket(listenPort);
			recvData = new byte[1024];
			rootServerData = new byte[1024];
			sendData = new byte[1024];
			recvPacket = new DatagramPacket(recvData, recvData.length);

			// Receive the packet
			serverSocket.receive(recvPacket);

			// Client IP address
			sourceIPAddress = recvPacket.getAddress();

			// Client port
			port = recvPacket.getPort();

			// check cache
			boolean inCache = false;
			byte[] cacheSend = null;
			int lengthOfSite = recvPacket.getData()[16] & 0xFFFF;
			// System.out.println(lengthOfSite);
			String tempInWebsite = new String(Arrays.copyOfRange(
					recvPacket.getData(), 12, 21 + lengthOfSite));
			String inWebsite = (tempInWebsite.substring(1, 4)
					+ tempInWebsite.substring(5, tempInWebsite.length() - 4) + tempInWebsite
					.substring(tempInWebsite.length() - 3));
			System.out.println(inWebsite);
			for (cacheObject e : server.cache) {
				if (e.getWebsiteName().equals(inWebsite)) {
					inCache = true;
					cacheSend = e.getPack();
					break;
				}
			}
			if (inCache) {
				System.out.println("Sending from cache");
				// set the ID to proper packet
				cacheSend[0] = recvPacket.getData()[0];
				cacheSend[1] = recvPacket.getData()[1];
				sendToClientPacket = new DatagramPacket(cacheSend,
						cacheSend.length, sourceIPAddress, port);
				serverSocket.send(sendToClientPacket);
				serverSocket.close();
			} else {
				// Root server IP
				destIPAddress = InetAddress.getByName("198.41.0.4");

				System.out.println("Receiving from "
						+ sourceIPAddress.toString() + " On port: " + port);

				// build packet to send to root
				sendPacket = new DatagramPacket(recvData, recvData.length,
						destIPAddress, 53);

				// make recursive call
				try {
					rootServerData = (server.recursiveResolver(sendPacket))
							.getData();
				} catch (Exception e) {
					e.printStackTrace();
				}

				int questionCount = (server.combineBytes(rootServerData[4],
						rootServerData[5])) & 0xFFFF;
				int answerCount = (server.combineBytes(rootServerData[6],
						rootServerData[7])) & 0xFFFF;
				int authorityCount = (server.combineBytes(rootServerData[8],
						rootServerData[9])) & 0xFFFF;
				int additionalCount = (server.combineBytes(rootServerData[10],
						rootServerData[11])) & 0xFFFF;

				/*
				 * System.out.println("Question Count: " + questionCount);
				 * System.out.println("Answer Count: " + answerCount);
				 * System.out.println("Authority Count: " + authorityCount);
				 * System.out.println("Additional Count: " + additionalCount);
				 */

				// Add it to cache
				int i = 0;
				for (int i1 = 0; i1 < questionCount; i1++) {
					while (rootServerData[12 + i] != 0) {
						i += rootServerData[12 + i];
						i++;
					}
				}
				i += 16;
				int ttl = ((rootServerData[i + 7] & 0xFF) << 24
						| (rootServerData[i + 8] & 0xFF) << 16
						| (rootServerData[i + 9] & 0xFF) << 8 | (rootServerData[i + 10] & 0xFF));

				// System.out.println(ttl);
				InetAddress answerIpAddress = InetAddress.getByAddress(
						"java.sun.com",
						Arrays.copyOfRange(rootServerData, i + 13, i + 17));
				// System.out.println(answerIpAddress.getCanonicalHostName());
				server.cache
						.add(new cacheObject(inWebsite, ttl, rootServerData));

				// respond to client
				sendToClientPacket = new DatagramPacket(rootServerData,
						rootServerData.length, sourceIPAddress, port);
				serverSocket.send(sendToClientPacket);
				serverSocket.close();
			}
		}
	}

	private DatagramPacket recursiveResolver(DatagramPacket packet)
			throws Exception {
		DatagramSocket nameSocket = new DatagramSocket();
		nameSocket.setSoTimeout(10000);
		byte[] responseData = new byte[1024];
		DatagramPacket responsePacket = new DatagramPacket(responseData,
				responseData.length);
		InetAddress tempServerIpAddress = null;

		// sends to name server
		nameSocket.send(packet);

		// Response Packet from name server
		nameSocket.receive(responsePacket);
		nameSocket.close();
		responseData = responsePacket.getData();
		int questionCount = (combineBytes(responseData[4], responseData[5])) & 0xFFFF;
		int answerCount = (combineBytes(responseData[6], responseData[7])) & 0xFFFF;
		System.out.println("NUMBER OF ANSWERS: " + answerCount);
		int authorityCount = (combineBytes(responseData[8], responseData[9])) & 0xFFFF;
		int additionalCount = (combineBytes(responseData[10], responseData[11])) & 0xFFFF;

		int i = 0;
		// Looping through to find end of query section.
		for (int i1 = 0; i1 < questionCount; i1++) {
			while (responseData[12 + i] != 0) {
				i += responseData[12 + i];
				i++;
			}
			i += 5;
			// System.out.println("THE QUERIES SECTION IS DONE AT BYTE " +
			// (12+i));
		}
		int i3 = 12 + i;
		int aCounter = 0;
		int nsCounter = 0;
		if (answerCount > 0) {

			//loops through answers to see what type they are.
			for (int answerInc = 0; answerInc < answerCount; answerInc++) {
				if (responseData[i3 + 3] == 1) {
					aCounter++;
					i3 += 16;
				} else if (responseData[i3 + 3] == 5) {
					//NS detected, increment ns counter and figure out data length of this awnser.
					nsCounter++;
					i3 += 12 + responseData[i3 + 11];
				}
			}
		}
		// TODO change back to aCounter
		if (aCounter > 0) {
			return responsePacket;
		}
		System.out.println("Number of NS Answers: "+ nsCounter);
		System.out.println("Number of type A Answers: "+ aCounter);
		System.out
				.println("Only name servers in answers, continuing recursion");

		i = i3;
		// System.out.println("I is " + i);

		//System.out.println("Start of authority is at byte " + i);
		int j = i;
		for (int j1 = 0; j1 < authorityCount; j1++) {
			/*int name = (combineBytes(responseData[j], responseData[j + 1])) & 0xFFFF;
			 System.out.println("Name is " + name);
			int type = (combineBytes(responseData[j + 2], responseData[j + 3])) & 0xFFFF;
			 System.out.println("Type is " + type);
			int serverClass = (combineBytes(responseData[j + 4],
					responseData[j + 5])) & 0xFFFF;
			 System.out.println("Class is " + serverClass);
			/*long timeToLive = (combineBytes(responseData[j + 6],
					responseData[j + 7], responseData[j + 8],
					responseData[j + 9])) & 0xFFFFFFFFFL;*/
			// System.out.println("Time to live is " + timeToLive);
			int dataLength = (combineBytes(responseData[j + 10],
					responseData[j + 11])) & 0xFFFF;
			// System.out.println("Data length: " + dataLength);
			for (int j2 = 0; j2 < dataLength; j2++) {
				// System.out.print(responseData[j+j2+12]);
			}
			// System.out.println();
			j = j + 12 + dataLength;
		}

		int startOfAdditional = j;

		//System.out.println("Additional Responses");

		for (int j1 = 0; j1 < additionalCount - 1; j1++) {
			/*
			 * int name = (combineBytes(responseData[j], responseData[j+1])) &
			 * 0xFFFF; //System.out.println("Name is " + name); int type =
			 * (combineBytes(responseData[j+2], responseData[j+3])) & 0xFFFF;
			 * //System.out.println("Type is " + type); int serverClass =
			 * (combineBytes(responseData[j+4], responseData[j+5])) & 0xFFFF;
			 * //System.out.println("Class is " + serverClass); int timeToLive =
			 * (combineBytes(responseData[j+6],
			 * responseData[j+7],responseData[j+8], responseData[j+9])) &
			 * 0xFFFFFFFF; //System.out.println("Time to live is " +
			 * timeToLive);
			 */
			int dataLength = (combineBytes(responseData[j + 10],
					responseData[j + 11])) & 0xFFFF;
			// System.out.println(" Data length is: " + dataLength);
			if (dataLength == 4 && tempServerIpAddress == null) {
				System.out.print("Next IPv4 Address: ");
				byte[] tempipArray = Arrays.copyOfRange(responseData, (j + 12),
						(j + 12 + dataLength));
				tempServerIpAddress = InetAddress.getByAddress("java.sun.com",
						tempipArray);
				System.out.println(tempServerIpAddress.getHostAddress());

			}
			// System.out.println();
			j = j + 12 + dataLength;
		}

		// Trim off ending of packet
		int place = 0;
		byte[] tempData = packet.getData();
		for (int c = tempData.length - 1; c > 0; c--) {
			if (tempData[c] > 1) {
				place = c;
				break;
			}
		}
		tempData = Arrays.copyOfRange(tempData, 0, place + 1);
		// set number of additional records to 0, for some reason this was at 1
		tempData[10] = 0;
		tempData[11] = 0;
		System.out.println("Sending next packet to: "
				+ tempServerIpAddress.getHostAddress());
		DatagramPacket nextPack = new DatagramPacket(tempData, tempData.length,
				tempServerIpAddress, 53);
		return recursiveResolver(nextPack);
	}

	public short combineBytes(byte a, byte b) {
		short ab = (short) ((a << 8) | b);
		return ab;
	}

//	public short combineBytes(byte a, byte b, byte c, byte d) {
//		short ab = (short) ((a << 8) | b);
//		short cd = (short) ((c << 8) | d);
//		short abcd = (short) ((ab << 16) | cd);
//		return abcd;
//	}

	public void run() {
		System.out
				.println("Enter 'cache' to view the cache, and 'EXIT' to exit the program");
		Scanner s = new Scanner(System.in);
		String input = null;
		while (true) {
			input = s.nextLine();
			if (input.equals("cache")) {
				printCache();
			}
			if (input.equals("EXIT")) {
				System.out.println("Exiting Program");
				s.close();
				System.exit(1);
			}
		}

	}

	public void printCache() {
		System.out.println("Start of Cache:");
		Iterator<cacheObject> i = cache.iterator();
		cacheObject e;
		while (i.hasNext()) {
			e = i.next();
			if (!e.isExpired()) {
				System.out.println("Cache: " + e.getWebsiteName() + "  TTL: "
						+ e.getTTL() + "Seconds");
			} else {
				i.remove();
			}
		}
		System.out.println("End of Cache");
	}

}
