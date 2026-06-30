/** This is just the add method and and the classes.
 * This Does not have the parseing method.
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
		String[] asPaths;
		String nextRouter;
		String prefix;
		int prefixLocation;
		int prefixEndLoc;
		int nextRouteLocation;
		int prefixLength;
		int lineNum=0;
		int passInt;
		Trie trie = new Trie();
		String[] builder;

		try{
			Scanner in = new Scanner(new FileReader(args[0]));

			while(in.hasNext()){
				tempLine = in.nextLine();
				prefixLocation = tempLine.indexOf("/");
				prefixEndLoc = tempLine.indexOf("|");
				prefix = tempLine.substring(0,prefixLocation);
				prefixLength = Integer.parseInt(tempLine.substring(prefixLocation+1, prefixEndLoc));	
				nextRouteLocation = tempLine.indexOf("|", (prefixLocation +4));
				asPaths = (tempLine.substring(prefixLocation+4, nextRouteLocation)).split(" ");		
				nextRouter = tempLine.substring(nextRouteLocation+1);
				
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
				builder = prefix.split("\\.");
				passInt=0;
				for(int z=0;z<4; z++){
					//	System.out.println(builder[z]);
					passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
				}
				//System.out.println(passInt);
				//pass array and values to add function
				trie.add(passInt,prefixLength,nextRouter,asPaths.length);	

				//make new line and increment lineNumber	
				//System.out.println();
				lineNum++;
			}
			
			Scanner in2 = new Scanner(new FileReader(args[1]));
			passInt = 0;
			while(in2.hasNext()){
					tempLine = in2.nextLine();
					builder = tempLine.split("\\.");
					
					for(int z=0;z<4; z++){
					//	System.out.println(builder[z]);
					passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
				}
				//System.out.println(passInt);
				System.out.println("Input: " +tempLine+" Output: "+ trie.search(passInt));
			}


		}catch(Exception e){
			System.out.println("error");
			e.printStackTrace();
		}	
	}


	private Node root;

	public Trie(){
		root = new Node();
	}

	public String search(int binaryAddress){
		return search(binaryAddress, root, 31);
	}

	public String get(String str){
		//TODO get method
		return null;
	}

	//Returns the next router.

	private String search(int binaryAddress, Node currentNode, int place){
		//System.out.println("Place: " + place + "prefixLength: " + prefixLength);
		if((currentNode.getRight()==null && currentNode.getLeft()==null) || place ==-1){
			return currentNode.getNextRouter();
		}
		//System.out.println(binaryAddress[0]);
		int arrayPlace;
					
		if(((binaryAddress >>> place) &1)== LEFT){
			place--;
			if(currentNode.getLeft() ==null){
				//System.out.println("LEFT");
				return currentNode.getNextRouter();}
				//System.out.println("SEARCH:Left");
			return search(binaryAddress,currentNode.getLeft(),place);
		}
		else{
			place--;
			if(currentNode.getRight() ==null){
				//System.out.println("RIGHT");
				return currentNode.getNextRouter();}
				//System.out.println("SEARCH:Left");
			return search(binaryAddress, currentNode.getRight(),place);
		}


	}
	public void add(int bits, int prefixLength, String nextRouter, int ASPath){
				add(bits, root, prefixLength, 0, nextRouter, ASPath);
		}
	
	private void add(int bits, Node node, int length, int counter, String nextRouter, int ASPath){
				//System.out.println("Len: " + length+":"+counter);
				if(counter >= length){
					if(ASPath<node.getASPath()){
							node.setNextRouter(nextRouter);
							node.setASPath(ASPath);
						}
					return;
				}
				//System.out.println(bits+"|"+((bits>>>(31-counter))&1)+"shifting amount: " + (31-counter));
				//System.out.println(31-counter);
				if(((bits >>> (31-counter)) & 1) == LEFT){
						if(node.getLeft() == null){
								//System.out.println("Add:LEFT");
								node.setLeft(new Node());
						}
						counter++;
						add(bits, node.getLeft(), length, counter, nextRouter, ASPath);
				}else{
						if(node.getRight() == null){
								//System.out.println("Add:RIGHT");
								node.setRight(new Node());
						}
						counter++;
						add(bits, node.getRight(), length, counter, nextRouter, ASPath);
				}
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



