/** This is just the add method and and the classes.
  * This Does not have the parseing method.
  *
  */
import java.util.*;
import java.io.*;
import java.math.*;

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
				//test call to the Trie
				System.out.print("\nCalling trie add");
				String binaryString = new BigInteger(prefix.getBytes()).toString(2);
				trie.add(binaryString, nextRouter);

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
				root = new Node(-1);
		}

		public void add(String str, String nextRouter){
			add(str,nextRouter, root, 0);
		}

		public String get(String str){
				//TODO get method
			return null;
		}

		private void add(String str, String nextRouter, Node node, int place){
				if(str.length() < (place-1)){
					int tempData = Character.getNumericValue(str.charAt(place));
						if(tempData == LEFT){
								if(node.getLeft() == null){
										node.setLeft(new Node(tempData));
								}
								add(str,nextRouter, node.getLeft(), place++);
						}else{
								if(node.getRight() == null){
										node.setRight(new Node(tempData));
								}
								add(str,nextRouter, node.getRight(), place++);
						}
				}
				if(str.length() == (place-1)){
				//TODO compare for shortest AS paths before just setting this.
					node.setNextRouter(nextRouter);
				}
		}


public class Node{
		private Node left;
		private Node right;
		private Node parent;
		private int data;
		private String nextRouter;

		public Node(int data){
				this.left = null;
				this.right = null;
				this.parent = null;
				this.data = data;
		}

		public int getData(){
				return data;
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
		public void setNextRouter(String nextRouter){
			this.nextRouter = nextRouter;
		}
}
}
