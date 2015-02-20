/** This is just the add method and and the classes.
  * This Does not have the parseing method.
  *
  */
import java.util.*;
import java.io.*;
import java.math.*;
import java.nio.*;

public class Trie{
	
	public static void main (String[] args){
		String tempLine;
		String[] asPaths;
		String nextRouter;
		String prefix;
		int prefixLocation;
		int nextRouteLocation;
		int prefixLength;
		int lineNum=0;
		Trie trie = new Trie();
		
		try{
			Scanner in = new Scanner(new FileReader("sample.txt"));
			
			while(in.hasNext()){
				tempLine = in.nextLine();
				prefixLocation = tempLine.indexOf("/");
				prefix = tempLine.substring(0,prefixLocation);
				prefixLength = Integer.parseInt(tempLine.substring(prefixLocation+1, prefixLocation+3));	
				nextRouteLocation = tempLine.indexOf("|", (prefixLocation +4));
				asPaths = (tempLine.substring(prefixLocation+4, nextRouteLocation)).split(" ");		
				nextRouter = tempLine.substring(nextRouteLocation+1);
				//Print statements
				System.out.println("Line Number: " + lineNum);
				System.out.print("Prefix: "+prefix + " | ");	
				System.out.print("Prefix Length: " + prefixLength+ " |");
				System.out.print(" As Paths:");
				for (String e: asPaths){
					System.out.print(" " +e);
				}
				System.out.print(" | Next Router: " + nextRouter);

				//Take the periods out of the prefix
				//String noPeriodPrefix = prefix.replace(".","");
				//build the array here
				int[] binaryIntArray = new int[prefixLength];
			
				String[] builder = prefix.split("\\.");
				prefixLength = prefixLength/8;
				for(int z=0;z<prefixLength ; z++){
					System.out.println(builder[z]);
					int temp = Integer.parseInt(Integer.toBinaryString(Integer.parseInt(builder[z])));
					//while(temp>0){
						//TODO build up the int[] array with the integer temp.
					//}

				}
				// take the prefix and then reverse it, and turn it into a binary string, then parse it back to an int.
				//String binaryString = new StringBuilder(Integer.toBinaryString((Integer.parseInt(noPeriodPrefix)))).reverse().toString();
	
				//System.out.println("\n"+binaryString);
				//call add in the trie Class
				//trie.add(binaryInt, nextRouter, asPaths.length());

				//make new line and increment lineNumber	
				System.out.println();
				lineNum++;
			}
			
		}catch(Exception e){
			System.out.println("error");
		}	
		}
	
	
	
		private Node root;

		public Trie(){
				root = new Node();
		}

		public void add(int[] binaryAddress, String nextRouter, int asPathCount){
			add(binaryAddress, nextRouter, root, asPathCount);
		}
		
		public String search(int[] binaryAddress){
			return search(binaryAddress, root);
		}

		public String get(String str){
				//TODO get method
			return null;
		}
		
		//Returns the next router.
		private String search(int[] binaryAddress, Node currentNode){
			//This follows the trie until the end of the string, then returns the router address at the final node.
			for(int i = 0; i < binaryAddress.length; i++){
					if(binaryAddress[i] == 0){
							currentNode = currentNode.getLeft();
					}else{
							currentNode = currentNode.getRight();
					}
			}
			return currentNode.getNextRouter();
		}
		
		
		private void add(int[] binaryAddress, String nextRouter, Node currentNode, int asPathCount){
				
				//Follows the trie, creating a node if there is not one.
				for(int i = 0; i < binaryAddress.length; i++){
						if(binaryAddress[i] == 0){
								if(currentNode.getLeft() == null){
										currentNode.setLeft(new Node());
								}
								//Always sets the current node to the next node, even if a new one isn't made.
								currentNode = currentNode.getLeft();
						}else{
								if(currentNode.getRight() == null){
										currentNode.setRight(new Node());
								}
								currentNode = currentNode.getRight();

						}
				}
				
				//After arriving at the final node, we evaluate what is in this node.
				//If the asPathCount is 0 in a node, this is an empty node. If so, we put our current data in this node.
				if(currentNode.getPathCount() == 0){	
					currentNode.setNextRouter(nextRouter, asPathCount);
				}
				//If the path count that is currently in the node is greater than our current path count, we replace the information.
				else if(currentNode.getPathCount() > asPathCount){
					currentNode.setNextRouter(nextRouter, asPathCount);
				}
				
				//If the path count is the same, we do nothing, because we keep earlier entries if the path count is the same.
				//If the path count is greater, we do nothing.
					
		}


public class Node{
		private Node left;
		private Node right;
		private Node parent;
		private int asPathCount;
		private String nextRouter;

		public Node(){
				this.left = null;
				this.right = null;
				this.parent = null;
				this.asPathCount = 0;
		}

		public Node getLeft(){
				return left;
		}
		
		public String getNextRouter(){
			return nextRouter;
		}

		public Node getRight(){
				return right;
		}
		
		public int getPathCount(){
			return asPathCount;
		}

		public void setLeft(Node child){
				this.left = child;
		}

		public void setRight(Node child){
				this.right = child;
		}
				
		public void setParent(Node parent){
			this.parent = parent;
		}
		
		public void setNextRouter(String nextRouter, int asPathCount){
			this.nextRouter = nextRouter;
			this.asPathCount = asPathCount;
		}
}
}


		
