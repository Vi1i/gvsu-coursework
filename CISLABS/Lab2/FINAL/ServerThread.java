import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;

class User {
		private String username;
		private InetAddress IPAddress;
		private Socket userSocket;
		private DataOutputStream OutToClient;
		private boolean isAdmin;

		public User(InetAddress IP, String name, Socket userSocket, boolean isAdmin) {
				username = name;
				IPAddress = IP;
				this.userSocket = userSocket;
				this.isAdmin = isAdmin;
				try {
						OutToClient = new DataOutputStream(userSocket.getOutputStream());
				} catch (IOException e) {
						System.out.println("Error creating output Stream");
				}
		}

		public String toString() {
				return "Username: " + username + " IP: " + IPAddress;
		}

		public String getUserName() {
				return username;
		}

		public Socket getSocket() {
				return userSocket;
		}

		public void sendUserMessage(String message) {
				try {
						OutToClient.writeBytes(message);
				} catch (IOException e) {
						System.out.println("Error sending to user");
				}
		}

		public boolean isAdmin() {
				return isAdmin;
		}
}

class Group {
		String name;
		User admin;
		ArrayList<User> users = new ArrayList<User>();

		public Group(User admin, String groupName) {
				this.name = groupName;
				this.admin = admin;
				users.add(admin);
		}

		public String getGroupName() {
				return name;
		}

		public boolean isAdmin(User user) {
				return (admin == user);
		}

		public void addUser(User userName) {
				users.add(userName);
		}

		public void removeUser(User userName) {
				users.remove(userName);
		}

		public String getName() {
				return name;
		}

		public String getAdminName() {
				return admin.getUserName();
		}

		public void sendMessage(String message, ArrayList<User> currentUsers) {
				for (User u : currentUsers) {
						if (users.contains(u)) {
								u.sendUserMessage(message);
						}
				}
		}

		public String getUserNames() {
				String list = "Group Users: ";
				for (User u : this.users) {
						list += (u.getUserName());
						list += ", ";
				}
				return list;
		}

		public ArrayList<User> getUsers() {
				return users;
		}
}

class ServerThread implements Runnable {

		private Socket connectionSocket;
		private static ArrayList<User> currentUsers = new ArrayList<User>();
		private static ArrayList<Group> currentGroups = new ArrayList<Group>();
		private volatile boolean execute;

		public ServerThread(Socket connSocket) {
				connectionSocket = connSocket;
		}

		public void sendMessage(String message) throws IOException {
				DateFormat df = new SimpleDateFormat("HH:mm:ss");
				Date datenow = new Date();
				String completeMessage = ('[' + df.format(datenow) + "] " + message + '\n');
				System.out.print(completeMessage);
				for (User t : currentUsers) {
						DataOutputStream tempOutToClient = null;
						tempOutToClient = new DataOutputStream(t.getSocket()
										.getOutputStream());
						tempOutToClient.writeBytes(completeMessage);
				}
		}

		public void sendMessage(String message, String destUsername,
						String sourceUsername) throws IOException {
				DateFormat df = new SimpleDateFormat("HH:mm:ss");
				Date datenow = new Date();
				String completeMessage = ('[' + df.format(datenow) + "] " + '['
								+ "PRIVATE MESSAGE FROM " + sourceUsername + " TO "
								+ destUsername + "] " + message + '\n');
				if (sourceUsername.equals("SERVER")) {
						completeMessage = ('[' + df.format(datenow) + "] " + '['
										+ "SERVER MESSAGE] " + message + "\n");
				} else {
						System.out.print(completeMessage);
				}

				for (User t : currentUsers) {
						if (t.getUserName().equals(destUsername)
										|| t.getUserName().equals(sourceUsername)) {
								DataOutputStream tempOutToClient = null;
								tempOutToClient = new DataOutputStream(t.getSocket()
												.getOutputStream());
								tempOutToClient.writeBytes(completeMessage);
						} else {
						}
				}
		}

		public void sendMessage(String message, Group groupUsers,
						String sourceUsername) throws IOException {
				DateFormat df = new SimpleDateFormat("HH:mm:ss");
				Date datenow = new Date();
				String completeMessage = ('[' + df.format(datenow) + "] " + '['
								+ "GROUP MESSAGE FROM " + sourceUsername + " TO GROUP "
								+ groupUsers.getName() + "] " + message + '\n');
				System.out.print(completeMessage);
				for (User t : groupUsers.getUsers()) {
						DataOutputStream tempOutToClient = null;
						tempOutToClient = new DataOutputStream(t.getSocket()
										.getOutputStream());
						tempOutToClient.writeBytes(completeMessage);
				}
		}

		public void sendUserList(String toUser) {
				String list = "Current Users: ";
				User tempUser = null;
				for (User t : currentUsers) {
						list += (t.getUserName() + ", ");
						if (t.getUserName() == toUser) {
								tempUser = t;
						}
				}
				list += "\n";
				if (toUser.equals("SERVER")) {
						System.out.println(list);
				} else {
						tempUser.sendUserMessage(list);
				}
		}

		public void sendGroupList(String toUser) {
				String list = "Current Groups: ";
				for (Group g : currentGroups) {
						list += (g.getGroupName() + ", ");
				}
				try {
						sendMessage(list, toUser, "SERVER");
				} catch (Exception e23) {
				}
		}

		public int createGroup(User admin, String groupName) {
				boolean groupExists = false;
				for (Group g : currentGroups) {
						if ((g.getName()).equals(groupName)) {
								groupExists = true;
						}
				}
				if (!groupExists) {
						currentGroups.add(new Group(admin, groupName));
						return 1;
				}
				return 0;
		}

		public void sendToGroup(String message, String groupName, String fromUser) {
				for (Group g : currentGroups) {
						if ((g.getName()).equals(groupName)) {
								try {
										sendMessage(message, g, fromUser);
								} catch (IOException e) {
										e.printStackTrace();
								}
						}
				}

		}

		public int addRemoveUserToGroup(User userName, String groupName, int addRem) {
				for (Group g : currentGroups) {
						if ((g.getName()).equals(groupName)) {
								if (addRem == 1) {
										g.addUser(userName);
								} else {
										g.removeUser(userName);
								}
								return 1;
						}
				}
				return 0;
		}

		public void stopExecuting() {
				this.execute = false;
		}

		public boolean checkUserName(String user) {
				for (User t : currentUsers) {
						if ((t.getUserName()).equals(user)) {
								return false;
						}
				}
				if (user.equals(null)) {
						return false;
				}
				return true;
		}

		public int kickUser(User fromUser, String toUser) {
				boolean canKick = false;
				for (User u : currentUsers) {
						if (u.equals(fromUser)) {
								if (u.isAdmin()) {
										canKick = true;
								}
						}
				}
				if (canKick) {
						for (User us : currentUsers) {
								if (us.getUserName().equals(toUser)) {
										try {
												sendMessage("You have been kicked.", us.getUserName(),
																"SERVER");
												for (Group g : currentGroups) {
														try {
																g.removeUser(us);
														} catch (Exception e17) {
														}
												}
										} catch (IOException e1) {
												e1.printStackTrace();
										}
										currentUsers.remove(us);

										try {
												us.getSocket().close();
										} catch (Exception e) {
										}
										return 1;
								}
						}
				}
				return 0;
		}

		public void run() {
				BufferedReader inFromClient = null;
				String sendToUserName = null;
				String sendToGroupName = null;
				String groupName = null;
				String userName = null;
				try {
						inFromClient = new BufferedReader(new InputStreamReader(
												(this.connectionSocket).getInputStream()));
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
						outToClient
								.writeBytes("Welcome to the chatroom! Choose a username: \n");
				} catch (IOException e2) {
						e2.printStackTrace();
				}

				String username = null;
				while (true) {
						try {
								username = inFromClient.readLine();
								if (!(checkUserName(username))) {
										outToClient
												.writeBytes("Username not accepted, please enter a different name: \n");
								} else {
										break;
								}
						} catch (IOException e2) {
								e2.printStackTrace();
						}
				}

				User currentUser = null;

				if (username != null) {
						InetAddress userIP = this.connectionSocket.getInetAddress();
						currentUser = new User(userIP, username, this.connectionSocket,
										(currentUsers.size() == 0));
						System.out.println(currentUser);
						currentUsers.add(currentUser);
						this.execute = true;
						try {
								sendMessage(currentUser.getUserName() + " has connected.");
								if (currentUser.isAdmin()) {
										sendMessage(currentUser.getUserName() + " is an admin.");
								}
						} catch (Exception e12) {
						}
				} else {
						stopExecuting();
				}

				try {
						sendMessage("For a list of commands, type cmd HELP.",
										currentUser.getUserName(), "SERVER");
				} catch (IOException e3) {
						e3.printStackTrace();
				}

				while (this.execute) {
						String clientMessage = null;
						String completeMessage = null;
						try {
								clientMessage = inFromClient.readLine();
						} catch (SocketException e) {
								stopExecuting();
								break;
						} catch (IOException e1) {
						}

						completeMessage = (currentUser.getUserName() + ": " + clientMessage);

						/* This is if the client unexpectedly leaves or chooses to leave */
						if (clientMessage == null || clientMessage.equals("cmd CLOSE")) {
								stopExecuting();
								try {
										this.connectionSocket.close();
								} catch (Exception e2) {
								}
								currentUsers.remove(currentUser);
								for (Group g : currentGroups) {
										{
												try {
														g.removeUser(currentUser);
												} catch (Exception t1) {

												}
										}
								}
								try {
										sendMessage(currentUser.getUserName()
														+ " has left the chat room.");
								} catch (Exception e4) {
								}

								currentUsers.remove(currentUser);
								break;

						} else if (clientMessage.equals("cmd ALLUSERS")) {
								sendUserList(currentUser.getUserName());
						}
						else if (clientMessage.equals("cmd CREATEGROUP")) {
								try {
										sendMessage("What would you like to call your group?",
														currentUser.getUserName(), "SERVER");
										groupName = inFromClient.readLine();
										if (createGroup(currentUser, groupName) == 1) {
												sendMessage("The group [" + groupName
																+ "] has been created.");
										} else {
												sendMessage(
																"Group creation failed. The group name is taken.",
																currentUser.getUserName(), "SERVER");
										}
								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd USERSEND")) {
								try {
										sendMessage("Who would you like to send your message to?",
														currentUser.getUserName(), "SERVER");
										sendUserList(currentUser.getUserName());
										sendToUserName = inFromClient.readLine();
										boolean found = false;
										for (int i = 0; i < currentUsers.size(); i++) {
												if (found == true) {
												} else if (currentUsers.get(i).getUserName()
																.equals(sendToUserName)) {
														sendMessage("Type your message for "
																		+ sendToUserName,
																		currentUser.getUserName(), "SERVER");
														clientMessage = inFromClient.readLine();
														sendMessage(clientMessage, sendToUserName,
																		currentUser.getUserName());
														found = true;
												} else if (i == (currentUsers.size() - 1)) {
														sendMessage("There is no user with the username "
																		+ sendToUserName + ".");
												}
										}

								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd GROUPSEND")) {
								try {
										sendMessage(
														"What group would you like to send your message to?",
														currentUser.getUserName(), "SERVER");
										sendGroupList(currentUser.getUserName());
										sendToGroupName = inFromClient.readLine();
										boolean groupExist = false;
										boolean partOfGroup = false;
										for (Group g : currentGroups) {
												if (g.getGroupName().equals(sendToGroupName)) {
														groupExist = true;
												}
												for (User u : g.getUsers()) {
														if (u.getUserName() == currentUser.getUserName()) {
																partOfGroup = true;
														}
												}

												if (groupExist == true && partOfGroup == true) {
														sendMessage("What is your message for group "
																		+ " [" + sendToGroupName + "]?",
																		currentUser.getUserName(), "SERVER");
														clientMessage = inFromClient.readLine();
														sendToGroup(clientMessage, sendToGroupName,
																		currentUser.getUserName());
												} else if (!groupExist) {
														sendMessage("There is not a group with that name.",
																		currentUser.getUserName(), "SERVER");
												} else if (!partOfGroup) {
														sendMessage("You are not a member of that group.",
																		currentUser.getUserName(), "SERVER");
												}
										}
								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd JOINGROUP")) {
								try {
										sendMessage("What group would you like to join?",
														currentUser.getUserName(), "SERVER");
										sendGroupList(currentUser.getUserName());
										groupName = inFromClient.readLine();
										if (addRemoveUserToGroup(currentUser, groupName, 1) == 0) {
												sendMessage("Group does not exist.",
																currentUser.getUserName(), "SERVER");
										} else {
												sendMessage("You have been added to group " + groupName
																+ ".", currentUser.getUserName(), "SERVER");
										}
								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd LEAVEGROUP")) {
								try {
										sendMessage("What group would you like to leave?",
														currentUser.getUserName(), "SERVER");
										sendGroupList(currentUser.getUserName());
										groupName = inFromClient.readLine();
										if (addRemoveUserToGroup(currentUser, groupName, 0) == 0) {
												outToClient.writeBytes("Group does not exist\n");
										} else {
												outToClient.writeBytes("Leave successful \n");
										}
								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd KICKUSER")) {
								try {
										sendMessage("What user would you like to kick?",
														currentUser.getUserName(), "SERVER");
										sendUserList(currentUser.getUserName());
										userName = inFromClient.readLine();
										if (kickUser(currentUser, userName) == 0) {
												sendMessage("Could not kick user.",
																currentUser.getUserName(), "SERVER");
										} else {
												sendMessage(currentUser.getUserName() + " kicked "
																+ userName + ".");
										}
								} catch (Exception e) {
								}
						}
						else if (clientMessage.equals("cmd GROUPKICK")) {
								try {
										sendMessage("What group do you want to kick someone from?",
														currentUser.getUserName(), "SERVER");
										sendGroupList(currentUser.getUserName());
										groupName = inFromClient.readLine();
										boolean groupExist = false;
										boolean adminGroup = false;
										String kickUserName = "";
										Group currentGroup = null;
										for (Group g : currentGroups) {
												if (g.getGroupName().equals(groupName)) {
														groupExist = true;
														currentGroup = g;
												}
										}
										if (groupExist) {
												adminGroup = currentGroup.isAdmin(currentUser);
										}
										if (!groupExist) {
												sendMessage("There is no group with that name.",
																currentUser.getUserName(), "SERVER");
										} else if (!adminGroup) {
												sendMessage(
																"You are not the admin of this group. The admin is "
																+ currentGroup.getAdminName() + ".",
																currentUser.getUserName(), "SERVER");
										} else {
												sendMessage("Who do you want to kick from the group?",
																currentUser.getUserName(), "SERVER");
												sendMessage(currentGroup.getUserNames(),
																currentUser.getUserName(), "SERVER");
												kickUserName = inFromClient.readLine();
												User kickUser = null;
												for (User u : currentGroup.users) {
														if (u.getUserName().equals(kickUserName)) {
																sendMessage(u.getUserName()
																				+ " has been kicked from the group "
																				+ currentGroup.getGroupName() + ".");
																kickUser = u;
														}
												}
												if (kickUser == null) {
														sendMessage("No one has that name in the group.",
																		currentUser.getUserName(), "SERVER");
												} else {
														currentGroup.removeUser(kickUser);
												}
										}
								} catch (Exception t5) {
								}
						}
						else if (clientMessage.equals("cmd GROUPMEMBERS")) {
								try {
										sendMessage("What group do you want to see the users for?",
														currentUser.getUserName(), "SERVER");
										sendGroupList(currentUser.getUserName());
										groupName = inFromClient.readLine();
										boolean groupExist = false;
										Group currentGroup = null;
										for (Group g : currentGroups) {
												if (g.getGroupName().equals(groupName)) {
														groupExist = true;
														currentGroup = g;
												}
										}
										if (!groupExist) {
												sendMessage("There is not a group with that name.",
																currentUser.getUserName(), "SERVER");
										} else {
												sendMessage(currentGroup.getUserNames(),
																currentUser.getUserName(), "SERVER");
										}
								} catch (Exception ie12) {
								}
						}
						else if (clientMessage.equals("cmd HELP")) {
								try {
										sendMessage(
														"cmd CLOSE - Closes the client and removes you from all groups. \n\n"
														+ "cmd CREATEGROUP - Creates a group after prompting for a name. Creator is admin and the only one that can kick. \n\n"
														+ "cmd GROUPSEND - Sends a message to all members in the group. Members not in the group cannot see these messages. \n\n"
														+ "cmd GROUPKICK - Kick someone from a group. Only the group creator can do this. \n\n"
														+ "cmd GROUPJOIN - Join someone else's group. \n\n"
														+ "cmd GROUPLEAVE - Leave a group that you are a part of. \n\n"
														+ "cmd KICKUSER - Remove a user from the entire chatroom. Only the chatroom creator (first member) can do this. \n\n"
														+ "cmd GROUPMEMBERS - See all members of a specified group. \n\n"
														+ "cmd USERSEND - Sends a message to only 1 specified user. \n\n"
														+ "cmd ALLUSERS - Prints a list of all currently connected users. \n\n"
														+ "cmd HELP - Shows all commands and what they do. \n\n",
														currentUser.getUserName(), "SERVER");
								} catch (IOException e) {

										e.printStackTrace();
								}
						} else {

								try {
										sendMessage(completeMessage);
								} catch (Exception e) {

										stopExecuting();

										try {
												this.connectionSocket.close();
										} catch (Exception e2) {
										}
										try {
												sendMessage(currentUser.getUserName()
																+ " has left the chat room.");
										} catch (Exception e4) {
										}

										currentUsers.remove(currentUser);
										break;
								}
						}
				}
		}

		public static void main(String argv[]) throws IOException {
				int serverPort = 9876;
				ServerSocket listenSocket = new ServerSocket(serverPort);

				System.out.println("You are listening on Port: " + serverPort);

				while (true) {
						Socket connectionSocket = listenSocket.accept();
						System.out.println("The current number of users is: "
										+ (currentUsers.size() + 1));
						Thread t = new Thread(new ServerThread(connectionSocket));
						t.start();
				}
		}
}
