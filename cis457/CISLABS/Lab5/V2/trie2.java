/** This is just the add method and and the classes.
  * This Does not have the parseing method.
  *
  */

public class Trie{
		final int LEFT = 0;
		private Node root;

		public Trie(){
				root = new Node( );
		}

		public void add(String str){
				root.add(str, root, 0);
		}

		public String get(String str){
				//TODO get method
		}

		private void add(String str, Node node, int place){
				if(str.charAt(place) != null){
						if(Characater.getNumericValue(str.charAt(place)) == LEFT){
								if(node.getLeft() == null){
										node.setLeft(new Node());
								}
								add(str, node.getLeft(), place++);
						}else{
								if(node.getRight() == null){
										node.setRight(new Node());
								}
								add(str, node.getRight(), place++);
						}
				}
				//This would be where data would be set if we need to keep any.
		}
}

public class Node{
		private Node left;
		private Node right;

		private String data = null;

		public Node(String data, boolean end){
				this.left = null;
				this.right = null;
				this.data = data;
		}

		public String getData(){
				return data;
		}

		public Node getLeft(){
				return left;
		}

		public Node getRight(){
				return right;
		}

		public setLeft(Node child){
				this.left = child;
		}

		public setRight(Node child){
				this.right = child;
		}
}
