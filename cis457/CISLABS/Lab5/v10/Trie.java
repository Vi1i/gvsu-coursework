/** 
 * AS path comparison does not work.
 *
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
				
				if((lowestPrefix.equals(prefix)) && (lowestPrefixLength == prefixLength)){
					if(lowestasPath > asPaths){
						//System.out.println("Case1");
						lowestPrefix = prefix;
						lowestPrefixLength = prefixLength;
						lowestasPath = asPaths;
						lowestRouter = nextRouter;
						continue;
					}
					else if(lowestasPath < asPaths){
						//System.out.println("Next router is: " + nextRouter);
						//System.out.println("Case2");
						//System.out.println(lowestasPath + " " + asPaths); 
						continue;
					}
					else if(lowestasPath == asPaths){
							continue;
					}		
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
				//build the array here
				else if(!(lowestPrefix.equals(prefix))){
						//System.out.println("Prefix is not equal");
						builder = lowestPrefix.split("\\.");
						passInt=0;
						for(int z=0;z<4; z++){
							//	System.out.println(builder[z]);
							passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
						}
						//System.out.println(passInt);
						//pass array and values to add function
						trie.add(passInt,lowestPrefixLength,lowestRouter,asPaths);	
						lowestPrefix = prefix;
						lowestPrefixLength = prefixLength;
						lowestasPath = asPaths;
						lowestRouter = nextRouter;	
				}
				else if((lowestPrefixLength != prefixLength)){
						//System.out.println("Length is not equal");
						builder = lowestPrefix.split("\\.");
						passInt=0;
						for(int z=0;z<4; z++){
							//	System.out.println(builder[z]);
							passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
						}
						//System.out.println(passInt);
						//pass array and values to add function
						trie.add(passInt,lowestPrefixLength,lowestRouter,asPaths);	
						lowestPrefix = prefix;
						lowestPrefixLength = prefixLength;
						lowestasPath = asPaths;
						lowestRouter = nextRouter;	
					
				}
			}
			
			Scanner in2 = new Scanner(new FileReader(args[1]));
			
			while(in2.hasNext()){
					passLong = 0;
					tempLine = in2.nextLine();
					builder = tempLine.split("\\.");
					
					for(int z=0;z<4; z++){
						//System.out.println(Integer.parseInt(builder[z])*Math.pow(256,3-z));
						passLong += Integer.parseInt(builder[z])*Math.pow(256,3-z);
						//System.out.println("PassInt is now: " + passLong);
					}
				//System.out.println(passLong);
				System.out.println("Input: " +tempLine+" Output: "+ trie.search(passLong));
			}
			
			trie.navigate();


		}catch(Exception e){
			System.out.println("error");
			e.printStackTrace();
		}	
	}


	private Node root;

	public Trie(){
		root = new Node();
	}

	public String search(long binaryAddress){
		return search(binaryAddress, root);
	}

	public String get(String str){
		//TODO get method
		return null;
	}
	
	
	public void navigate(){
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
				
				
		
	}
	//Returns the next router.

	private String search(long binaryAddress, Node currentNode){
		boolean print = false;
		String currentRouter = null;
		//if (binaryAddress == 1019524259){
		//		print = true;
		//	}
		//System.out.println("Currently using: " + Long.toBinaryString(binaryAddress));
		//if(print){System.out.println("Currently using for search: " + Long.toBinaryString(binaryAddress));}
		for(int i = 31; i >= 1; i--){
			if(((binaryAddress >>> (i)) & 1) == LEFT){
			//if(print){System.out.println("0");}
				if(currentNode.getLeft() == null){
						return currentRouter;
				}
				currentNode = currentNode.getLeft();
			}
			else{
				//if(print){System.out.println("1");}
				if(currentNode.getRight() == null){
						return currentRouter;
				}
				currentNode = currentNode.getRight();
			}
			if(currentNode.getNextRouter() != null){
				currentRouter = currentNode.getNextRouter();
			}
			
			
			if((currentNode.getLeft() == null) && (currentNode.getRight() == null)){
					return currentRouter;
			}
		}
		return currentNode.getNextRouter();
		


	}
	public void add(long bits, int prefixLength, String nextRouter, int ASPath){
				add(bits, root, prefixLength, nextRouter, ASPath);
		}
	
	private void add(long prefix, Node currentNode, int length, String nextRouter, int ASPath){
			//boolean print = false;
			//if (prefix == 1019478016){
			//	print = true;
			//}
			//System.out.println("Len: " + length+":"+counter);
			//if(print){System.out.println("Currently using: " + Long.toBinaryString(prefix));}
			//if (print) {System.out.println("Placing this router at the final node using above IP: " + nextRouter);}
			//if(print){System.out.println("Length is: " + length);}
			for(int i = 0; i < length; i++){	
				//System.out.println("This is the bit shift result: " + ((prefix >>> (31-i)) & 1));
					if(((prefix >>> (31-i)) & 1) == LEFT){
			//			if(print){System.out.println("0");}
						if(currentNode.getLeft() == null){
								//System.out.println("Add:LEFT");
								currentNode.setLeft(new Node());
						}
						currentNode = currentNode.getLeft();
					}else{
			//			if(print){System.out.println("1");}
						if(currentNode.getRight() == null){
								//System.out.println("Add:RIGHT");
								currentNode.setRight(new Node());
						}
						currentNode = currentNode.getRight();
				}
			}
			currentNode.setASPath(ASPath);
			currentNode.setNextRouter(nextRouter);
			
		}

		private class Node{
				private Node left;
				private Node right;

				private String nextRouter = null;

				private int ASPath;

				public Node(){
						this.left = null;
						this.right = null;
						this.ASPath = Integer.MAX_VALUE;
				}

				public String getNextRouter(){
						return nextRouter;
				}

				public int getASPath(){
						return ASPath;
				}

				public Node getLeft(){
						return left;
				}

				public Node getRight(){
						return right;
				}

				public void setLeft(Node child){
						this.left = child;
				}

				public void setRight(Node child){
						this.right = child;
				}

				public void setNextRouter(String nextRouter){
						this.nextRouter = nextRouter;
				}

				public void setASPath(int ASPath){
						this.ASPath = ASPath;
				}
		}
}



