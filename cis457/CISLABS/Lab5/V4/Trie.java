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

		final int LEFT = 0;
		private Node root;

		public Trie(){
				root = new Node();
		}

		public void add(int binaryInt, String nextRouter, int asPathCount){
			//add(binaryInt,nextRouter, root, 0, asPathCount);
		}

		public String get(String str){
				//TODO get method
			return null;
		}

		private void add(int binaryInt, String nextRouter, Node node, int place, int asPathCount){
				if(binaryInt>0){
					int tempData = binaryInt%10;
						if(tempData == LEFT){
								if(node.getLeft() == null){
										node.setLeft(new Node());
								}
								add(tempData/10,nextRouter, node.getLeft(), place++, asPathCount);
						}else{
								if(node.getRight() == null){
										node.setRight(new Node());
								}
								add(tempData/10,nextRouter, node.getRight(), place++, asPathCount);
						}
				}
				if(binaryInt ==0){
				//TODO compare for shortest AS paths before just setting this.
					node.setNextRouter(nextRouter, asPathCount);
				}
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
		public void setParent(Node parent){
			this.parent = parent;
		}
		public void setNextRouter(String nextRouter, int asPathCount){
			this.nextRouter = nextRouter;
			this.asPathCount = asPathCount;
		}
}
}
