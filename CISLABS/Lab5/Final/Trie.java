/** 
 *Lucas Crandle
 *Lawrence O'boyle
 *Brendan Koning
 */
import java.util.*;
import java.io.*;
import java.math.*;
import java.lang.Integer;

public class Trie{
	final int LEFT = 0;
	public static void main (String[] args){
		String tempLine;
		int asPaths;
		String[] asPathsInt;
		String nextRouter;
		String prefix;
		int prefixLocation;
		int prefixEndLoc;
		int nextRouteLocation;
		int prefixLength;
		int lineNum=0;
		long passInt;
		long passLong;
		Trie trie = new Trie();
		String[] builder;
		String lowestPrefix = "";
		int lowestPrefixLength = 0;
		int lowestasPath = 0;
		String lowestRouter = "";
		int sum = 0;		


		try{
			Scanner in = new Scanner(new FileReader(args[0]));

			while(in.hasNext()){
				tempLine = in.nextLine();
				prefixLocation = tempLine.indexOf("/");
				prefixEndLoc = tempLine.indexOf("|");
				prefix = tempLine.substring(0,prefixLocation);
				prefixLength = Integer.parseInt(tempLine.substring(prefixLocation+1, prefixEndLoc));	
				nextRouteLocation = tempLine.indexOf("|", (prefixLocation +4));
				asPathsInt = (tempLine.substring(prefixLocation+4, nextRouteLocation)).split("\\s+");	
				asPaths = asPathsInt.length;
				nextRouter = tempLine.substring(nextRouteLocation+1);

				if(lowestPrefix == ""){
					System.out.println("Base Case");
					lowestPrefix = prefix;
					lowestPrefixLength = prefixLength;
					lowestasPath = asPaths;
					lowestRouter = nextRouter;
					continue;
				}




				//This needs to be completed, we need to interpret the AS path to compare them to the other paths.
				//Right now, it just uses the first AS path value that is provided so that I could get it working

				if((lowestPrefix.equals(prefix)) && (lowestPrefixLength == prefixLength)&& in.hasNext()){
					if(lowestasPath > asPaths){
						//System.out.println("Case1");
						lowestPrefix = prefix;
						lowestPrefixLength = prefixLength;
						lowestasPath = asPaths;
						lowestRouter = nextRouter;
						continue;
					}		
				}else{
					//System.out.println("Prefix is not equal");
					builder = lowestPrefix.split("\\.");
					passInt=0;
					for(int z=0;z<4; z++){
						//System.out.println(builder[z]);
						passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
					}
					//System.out.println(passInt);
					//pass array and values to add function
					//System.out.println( passInt + " |" + lowestPrefixLength + "|" + lowestRouter);
					trie.add(passInt,lowestPrefixLength,lowestRouter,asPaths,Integer.parseInt(args[2]));	
					lowestPrefix = prefix;
					lowestPrefixLength = prefixLength;
					lowestasPath = asPaths;						
					lowestRouter = nextRouter;
				}	


				//Print statements
				/*System.out.println("Line Number: " + lineNum);
				  System.out.print("Prefix: "+prefix + " | ");	
				  System.out.print("Prefix Length: " + prefixLength+ " |");
				  System.out.print(" As Paths:");
				  for (String e: asPaths){
				  System.out.print(" " +e);
				  }
				  System.out.print(" | Next Router: " + nextRouter);
				  */
			}

			Scanner in2 = new Scanner(new FileReader(args[1]));
			long startTime=0;
			long endTime=0;
			long Counter=0;
			long totalTime = 0;
			while(in2.hasNext()){
				passLong = 0;
				startTime = System.nanoTime();

				tempLine = in2.nextLine();
				builder = tempLine.split("\\.");
				
				for(int z=0;z<4; z++){
					//System.out.println(Integer.parseInt(builder[z])*Math.pow(256,3-z));
					passLong += Integer.parseInt(builder[z])*Math.pow(256,3-z);
					//System.out.println("PassInt is now: " + passLong);
				}
				//System.out.println(passLong);
				endTime = System.nanoTime();
				totalTime += (endTime - startTime);
				Counter ++;
				System.out.println("Input: " +tempLine+" Output: "+ trie.search(passLong, Integer.parseInt(args[2])));
			}
			System.out.println("Average Lookup Time in nanoSeconds: " + (totalTime/Counter));
			System.out.println("Using stride Length: " + args[2]);
			Runtime run = Runtime.getRuntime();
			System.out.println("Runtime memory: "+ ((run.totalMemory() - run.freeMemory())/1000000) + " Megabytes");
			//trie.navigate();


		}catch(Exception e){
			System.out.println("error");
			e.printStackTrace();
		}	
	}


	private Node root;

	public Trie(){
		root = new Node();
	}

	public String search(long binaryAddress, int inputPrefix){
		return search(binaryAddress, root, inputPrefix);
	}

	public String get(String str){
		//TODO get method
		return null;
	}


	/*public void navigate(){
	  Node ourNode = root;
	  int i=0;
	  int currentRow = 0;
	  int currentColumn = 1;
	  while(i != 2){
	  System.out.println("0 to go left, 1 to go right, 2 to quit");
	  Scanner in = new Scanner(System.in);
	  i = Integer.parseInt(in.nextLine());
	  if(i==0){
	  currentRow++;
	  currentColumn--;
	  System.out.println("Current Node: " + "( " + currentRow + ", " + currentColumn + ")");
	  ourNode = ourNode.getLeft();
	  System.out.println("Next Router here is: " + ourNode.getNextRouter());
	  }
	  else if(i==1){
	  currentRow++;
	  currentColumn++;
	  System.out.println("Current Node: " + "( " + currentRow + ", " + currentColumn + ")");
	  ourNode = ourNode.getRight();
	  System.out.println("Next Router here is: " + ourNode.getNextRouter());
	  }
	  else if(i==2){
	  break;
	  }
	  else{
	  System.out.println("Invalid Input, try again");

	  i=0;
	  }
	  }



	  }*/
	//Returns the next router.

	private String search(long prefix, Node currentNode, int inputPrefix){
		long data = 0;
		String returnRouter = null;
		for(int i = 0; i < 32; i++){
			data = 0;	
			switch(inputPrefix){
				case 1:
					data = ((prefix >>> (31-i)) & 1);
					break;
				case 2:
					for(int z = 0; z<2;z++){
						data += Math.pow(2,1-z)*((prefix >>> (31-(i+z))) & 1);
					}
					i += 1;
					break;
				case 3:
					for(int z=0; z<3; z++){
						data += Math.pow(2,2-z)*((prefix >>> (31-(i+z)) & 1));
					}
					i += 2;
					break;
				default:
					System.out.println("Error in private void add()\n");
					break;
			}
			if(currentNode.getNextRouter() != null){
				returnRouter = currentNode.getNextRouter();
			}
			//System.out.println("I: " + i);
			//System.out.println("search Data: "+data+ currentNode.getNextRouter());
			if(currentNode.getChild((int)data) == null){
				return returnRouter;
			}
			currentNode = currentNode.getChild((int)data);
		}
		return returnRouter;
	}
	public void add(long bits, int prefixLength, String nextRouter, int ASPath, int inputPrefix){
		add(bits, root, prefixLength, nextRouter, inputPrefix);
	}

	private void add(long prefix, Node currentNode, int length, String nextRouter,int inputPrefix){
		long data = 0;
		long tempData = 0;
		boolean flag = false;
		//System.out.println("Binary: ");
		//for(int z = 0; z<length; z++){
		//	System.out.print(((prefix >>> (31-z)) & 1));
		//}
		//System.out.println(" ");
		//System.out.println("LENGTH IS: " + length);
		Node tempNode = null;
		for(int i = 0; i < length; i++){
			data = 0;	
			switch(inputPrefix){
				case 1:
					data = ((prefix >>> (31-i)) & 1);
					break;
				case 2:
					for(int z = 0; z<2;z++){
						data += Math.pow(2,1-z)*((prefix >>> (31-(i+z))) & 1);
					}
					
					i += 1;
					if(i == length){
						if((prefix >>> (32 - length) & 1) == 1){
							currentNode.setChild(new Node(nextRouter), 2);
							currentNode.setChild(new Node(nextRouter), 3);
						}
						else{
							currentNode.setChild(new Node(nextRouter), 0);
							currentNode.setChild(new Node(nextRouter), 1);
						}
						flag = true;
					}
					break;
				case 3:
					for(int z=0; z<3; z++){
						data += Math.pow(2,2-z)*((prefix >>> (31-(i+z)) & 1));
					}
					//System.out.println("DATA IS: " + data);
					i += 2;
					if(i == length+1){
						if((prefix >>> (32 - length) & 1) == 1){
							currentNode.setChild(new Node(nextRouter), 4);
							currentNode.setChild(new Node(nextRouter), 5);
							currentNode.setChild(new Node(nextRouter), 6);
							currentNode.setChild(new Node(nextRouter), 7);
						}
						else{
							currentNode.setChild(new Node(nextRouter), 0);
							currentNode.setChild(new Node(nextRouter), 1);
							currentNode.setChild(new Node(nextRouter), 2);
							currentNode.setChild(new Node(nextRouter), 3);
						}
						flag = true;
					}
					else if(i == length){
						if((prefix >>> (32-length) & 1) == 1){
							//System.out.println("First 1");
							if((prefix >>> (31-length) & 1) == 1){
								//System.out.println("Second 1");
								currentNode.setChild(new Node(nextRouter), 7);
								currentNode.setChild(new Node(nextRouter), 6);
							}
							else{
								//System.out.println("Second 0");
								currentNode.setChild(new Node(nextRouter), 5);
								currentNode.setChild(new Node(nextRouter), 4);
							}
						}
						else{
							//System.out.println("First 0");
							if((prefix >>> (31 - length) & 1) == 1){
							//	System.out.println("Second 1");
								currentNode.setChild(new Node(nextRouter), 2);
								currentNode.setChild(new Node(nextRouter), 3);
							}
							else{
							//	System.out.println("Second 0");
								currentNode.setChild(new Node(nextRouter), 1);
								currentNode.setChild(new Node(nextRouter), 0);
							}
						}
						flag = true;
								
					}							
					break;
				default:
					System.out.println("Error in private void add()\n");
					break;
			}
			if(flag == true){
				break;
			}
			//System.out.println("Data: " + data);		
			if(currentNode.getChild((int)data) == null){
				//System.out.println("adding child");
				currentNode.setChild(new Node(), (int)data);
			}
			currentNode = currentNode.getChild((int)data);
		}
		//currentNode.setNextRouter("Test");
		//System.out.println(tempNode.getNextRouter());
		
		currentNode.setNextRouter(nextRouter);

	}

	private class Node{
		private Node[] children;

		private String nextRouter = null;

		public Node(){
			children= new Node[8];
		}
		public Node(String nextRouter){
			this.nextRouter = nextRouter;
			children = new Node[8];
		}

		public String getNextRouter(){
			return nextRouter;
		}
		public void addRouterToChildren(int place, String nextRouter){
			
			switch(place){
				case 0:
					for(int i=0; i< 4; i++){
						children[i] = new Node(nextRouter);
					}
				case 1: 
					children[1] = new Node(nextRouter);
					break;
				case 2:
					children[2] = new Node(nextRouter);
					children[3] = new Node(nextRouter);	
					break;
				case 3: 
					children[3] = new Node(nextRouter);
					break;
				case 4:	
					for(int i = 4; i < 8; i++){
						children[i] = new Node(nextRouter);
					}
					break;
				case 5:
					children[5] = new Node(nextRouter);
					break;
				case 6:
					children[6] = new Node(nextRouter);
					children[7] = new Node(nextRouter);
					break;
				case 7:
					children[7] = new Node(nextRouter);
					break;
				default:
					System.out.println("Out of bounds in adding nextRouter");
			}
		}
		public Node[] getChildren(){
			return children;
		}

		public void setChild(Node child, int place){
			children[place] = child;
		}
		public Node getChild(int place){
			//System.out.println(place);
			return children[place];
		}
		public void setNextRouter(String nextRouter){
			this.nextRouter = nextRouter;
		}

	}
}



