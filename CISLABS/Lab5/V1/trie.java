import java.util.*;
import java.io.*;

public class trie{

	public static void main (String[] args){
		String tempLine;
		String[] asPaths;
		String nextRouter;
		String prefix;
		int prefixLocation;
		int nextRouteLocation;
		int prefixLength;
		int lineNum=0;
		
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
				
				//make new line and increment lineNumber	
				System.out.println();
				lineNum++;
			}
		}catch(Exception e){
			System.out.println("error");
		}	
	}

class Node{
	int value;
	int strideLength;
	Node parent;
	Node LeftChild;
	Node RightChild;
	
	
	public Node(Node parent, int value, int strideLength){
		this.parent = parent;
		this.value = value;
		this.strideLength = strideLength;
	}

	public void setRightChild(Node tempRChild){
		this.RightChild = tempRChild;
	}

	public void setLeftChild(Node tempLChild){
		this.LeftChild = tempLChild;
	}
	
	public Node getRightChild(){
		return RightChild;
	}

	public Node getLeftChild(){
		return LeftChild;
	}
}
}
