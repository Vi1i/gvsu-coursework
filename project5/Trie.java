import java.lang.Integer;

public class Trie{
		final int LEFT = 0;
		private Node root;

		public Trie(){
				root = new Node( );
		}

		public void add(int[] bits, String nextRouter, int ASPath){
				int[] tempBits = new int[4];
				for(int z = 0; z < 5; z++){
						tempBits[z] = bits[z];
				}
				add(tempBits, root, bits[5], 0, nextRouter, ASPath);
		}

		public String get(String str){
				//TODO get method
				return "";
		}

		private void add(int[] bits, Node node, int length, int counter, String nextRouter, int ASPath){
				int place = 0;

				if(counter == length && ASPath < node.getASPath()){
						node.setNextRouter(nextRouter);
						node.setASPath(ASPath);
				}

				if(counter < 8){
						place = 0;
				}else if(counter < 16){
						place = 1;
				}else if(counter < 24){
						place = 2;
				}else if(counter < 32){
						place = 3;
				}

				if(((bits[place] >> (7 - (counter % 8))) & 1) == LEFT){
						if(node.getLeft() == null){
								node.setLeft(new Node());
						}
						add(bits, node.getLeft(), length, counter++, nextRouter, ASPath);
				}else{
						if(node.getRight() == null){
								node.setLeft(new Node());
						}
						add(bits, node.getRight(), length, counter++, nextRouter, ASPath);
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
