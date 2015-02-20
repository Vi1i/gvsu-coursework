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
				InetAddress sourceIPAddress = null;
				InetAddress destIPAddress = null;
				int port = 0;
				udpserver server = new udpserver();

				int listenPort = Integer.parseInt(args[0]);

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

						boolean inCache = false;
						byte[] cacheSend = null;
						int lengthOfSite = recvPacket.getData()[16] & 0xFFFF;
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
								try{
										// Root server IP
										destIPAddress = InetAddress.getByName("198.41.0.4");

										System.out.println("Receiving from "
														+ sourceIPAddress.toString() + " On port: " + port);

										// build packet to send to root
										sendPacket = new DatagramPacket(recvData, recvData.length,
														destIPAddress, 53);

										// make recursive call

										rootServerData = (server.recursiveResolver(sendPacket))
												.getData();
								} catch (Exception e) {
										System.out.println("NOT WORKING");
										// Root server IP
										destIPAddress = InetAddress.getByName("192.228.79.201");

										System.out.println("Receiving from "
														+ sourceIPAddress.toString() + " On port: " + port);

										// build packet to send to root
										sendPacket = new DatagramPacket(recvData, recvData.length,
														destIPAddress, 53);

										// make recursive call
										rootServerData = (server.recursiveResolver(sendPacket))
												.getData();
								}

								int questionCount = (server.combineBytes(rootServerData[4],
														rootServerData[5])) & 0xFFFF;
								int answerCount = (server.combineBytes(rootServerData[6],
														rootServerData[7])) & 0xFFFF;
								int authorityCount = (server.combineBytes(rootServerData[8],
														rootServerData[9])) & 0xFFFF;
								int additionalCount = (server.combineBytes(rootServerData[10],
														rootServerData[11])) & 0xFFFF;

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

								InetAddress answerIpAddress = InetAddress.getByAddress(
												"java.sun.com",
												Arrays.copyOfRange(rootServerData, i + 13, i + 17));
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

				if (answerCount == 0 && additionalCount == 0) {
						System.out.println("Invalid domain name, returning packet");
						return responsePacket;
				}
				int i = 0;
				// Looping through to find end of query section.
				for (int i1 = 0; i1 < questionCount; i1++) {
						while (responseData[12 + i] != 0) {
								i += responseData[12 + i];
								i++;
						}
						i += 5;
				}
				int i3 = 12 + i;
				int aCounter = 0;
				if (answerCount > 0) {
						InetAddress tempAddress = null;
						// loops through answers to see what type they are.
						for (int answerInc = 0; answerInc < answerCount; answerInc++) {
								if (responseData[i3 + 3] == 1) {
										// 12 13 14 15
										tempAddress = InetAddress.getByAddress(Arrays.copyOfRange(
																responseData, (i3 + 12), (i3 + 16)));
										System.out.println("answer Address': "
														+ tempAddress.getCanonicalHostName());
										aCounter++;
										i3 += 16;
								} else if (responseData[i3 + 3] == 5) {
										// NS detected, increment ns counter and figure out data
										// length of this answer.
										i3 += 12 + responseData[i3 + 11];
								}
						}
				}
				if (aCounter > 0) {
						System.out
								.println("Found TypeA answer, returning packet to client.");
						return responsePacket;
				}
				System.out.println("Only name servers in answers, continuing recursion");

				i = i3;

				int j = i;
				for (int j1 = 0; j1 < authorityCount; j1++) {
						int dataLength = (combineBytes(responseData[j + 10],
												responseData[j + 11])) & 0xFFFF;
						for (int j2 = 0; j2 < dataLength; j2++) {
						}
						j = j + 12 + dataLength;
				}


				for (int j1 = 0; j1 < additionalCount - 1; j1++) {
						int dataLength = (combineBytes(responseData[j + 10],
												responseData[j + 11])) & 0xFFFF;
						if (dataLength == 4 && tempServerIpAddress == null) {
								System.out.print("Next IPv4 Address: ");
								byte[] tempipArray = Arrays.copyOfRange(responseData, (j + 12),
												(j + 12 + dataLength));
								tempServerIpAddress = InetAddress.getByAddress("java.sun.com",
												tempipArray);
								System.out.println(tempServerIpAddress.getHostAddress());

						}
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
