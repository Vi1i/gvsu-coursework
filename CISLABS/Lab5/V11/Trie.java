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
						//	System.out.println(builder[z]);
						passInt += Integer.parseInt(builder[z])*Math.pow(256,3-z);
					}
					//System.out.println(passInt);
					//pass array and values to add function
					//System.out.println( passInt + " |" + lowestPrefixLength + "|" + lowestRouter);
					trie.add(passInt,lowestPrefixLength,lowestRouter,asPaths);	
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

	public String search(long binaryAddress){
		return search(binaryAddress, root);
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

	private String search(long binaryAddress, Node currentNode){
		int sLength = 0;
		long oneOrZero = -1;
		//System.out.println("entering search");
		for(int i = 0; i < 32; i++){
			sLength = getStrideLength(binaryAddress, 32, i);
			oneOrZero = ((binaryAddress >>> (31-i)) & 1);
			if(currentNode.getChild(sLength, oneOrZero) == null){
				int tempLength = sLength;
				while(tempLength >1){
					tempLength--;
					//System.out.println("Trying new sLength: " + sLength);
					if(currentNode.getChild(sLength,oneOrZero) != null){
						currentNode = currentNode.getChild(sLength,oneOrZero);
						sLength = tempLength;
						tempLength = 0;		
					}else if(sLength ==1){
						return currentNode.getNextRouter();
					}
				}
			}else{
				currentNode = currentNode.getChild(sLength, oneOrZero);
			}
			//System.out.println("not null"+ sLength + "| " + oneOrZero);
			//currentNode = currentNode.getChild(sLength, oneOrZero);
			i+= (sLength-1);
		}
		return currentNode.getNextRouter();
		


	}
	public void add(long bits, int prefixLength, String nextRouter, int ASPath){
				add(bits, root, prefixLength, nextRouter, ASPath);
		}
	
	private void add(long prefix, Node currentNode, int length, String nextRouter, int ASPath){
			int sLength = 0;
			long oneOrZero = 0;
			//System.out.println("entering");
			for(int i = 0; i < length; i++){	
				sLength = getStrideLength(prefix, length, i);
				oneOrZero = ((prefix >>> (31-i)) & 1);
					if(currentNode.getChild(sLength, oneOrZero) == null){
						//System.out.println("" + sLength + " " + oneOrZero);
						currentNode.setChild(new Node(), sLength, oneOrZero);
					}
				currentNode = currentNode.getChild(sLength, oneOrZero);
				i += (sLength-1);
			}
			currentNode.setASPath(ASPath);
			currentNode.setNextRouter(nextRouter);
			
		}
		private int getStrideLength(long prefix, int length, int position){
			if((length - position)==1){
				return 1;
			}
			if((length-position)==2){
				if(((prefix >>> (31-position)) &1)==((prefix >>> (31-position+1)) &1)){
					return 2;
				}else{
					return 1;
				}
			}
			long sameVal = ((prefix >>> (31-position)) &1);
			if(sameVal == (prefix >>> (31-(position+1))&1)){
				if(sameVal == ((prefix >>> (31-(position+2))) &1)){
					return 3;
				}
				return 2;
			}else{	
				return 1;
			}
			
		}

		private class Node{
				private Node[] children;

				private String nextRouter = null;
				private int strideLength;
				private int ASPath;

				public Node(){
						children= new Node[6];
						this.ASPath = Integer.MAX_VALUE;
						int length = 0;
				}

				public String getNextRouter(){
						return nextRouter;
				}

				public int getASPath(){
						return ASPath;
				}

				public Node[] getChildren(){
						return children;
				}
				public int getLength(){
					return strideLength;
				}

				public void setChild(Node child, int sLength, long oneOrZero){
					child.setStrideLength(sLength);
					if(sLength == 1){
						if(oneOrZero == 0){
							children[0] = child;
						}
						children[1] = child;
					}
					if(sLength == 2){
						if(oneOrZero == 0){
							children[2] = child;
						}
						children[3] = child;
					}
					if(sLength == 3){
						if(oneOrZero == 0){
							children[4] = child;
						}
						children[5] = child;
					}
				}
				public Node getChild(int sLength, long oneOrZero){
					if(sLength == 1){
						if(oneOrZero == 0){
							return children[0];
						}
						return children[1];
					}
					if(sLength == 2){
						if(oneOrZero == 0){
							return children[2];
						}
						return children[3];
					}
					if(sLength == 3){
						if(oneOrZero == 0){
							return children[4];
						}
						return children[5];
					}
					return null;
				}
				public void setNextRouter(String nextRouter){
						this.nextRouter = nextRouter;
				}

				public void setASPath(int ASPath){
						this.ASPath = ASPath;
				}
				public void setStrideLength(int setLength){
					strideLength= setLength;
				}
		}
}



