/**
 * This is a java implementation of simulated annealing for
 * graph coloring problem
 * @author JiangHua
 *
 */
package ynu.ls.coloring; 
import java.util.*;
import java.io.*;
public class GraphColoringWithSA {
	
	public static void main(String[] args) {
		Scanner in=new Scanner(System.in);
                Graph G=readDimacsInstance(in.next()) ;//读取图文件
		int nbColor=in.nextInt();//颜色数
		int nbRound=in.nextInt();//求解次数
		System.out.println(G);
		SimaColoring sancol=new SimaColoring(G,nbColor);
		for (int r = 1; r <= nbRound; r++) {
			int[] sol=sancol.search(); //执行搜索，返回解
			System.out.printf("Round %2d ",r);
			G.verifySolution(sol,nbColor); //验证解的正确性
		}
	}
	/**
	 * 从文件中读入图
	 * @param filename
	 * @return Graph 对象
	 */
	public static Graph readDimacsInstance(String filename){
		//System.out.println(filename);
		Graph gh=null;
		try {
			
			Scanner fin=new Scanner(new File(filename));
			while(fin.hasNextLine()) {
				String line =fin.nextLine();
				Scanner sline=new Scanner(line);
				if(line.startsWith("p")) {
					sline.next();sline.next();
					int numV=sline.nextInt();
					int numE=sline.nextInt();
					gh=new Graph(numV, numE);
				}else if(line.startsWith("e")) {
					assert(gh!=null);
					sline.next();
					gh.addEdge(sline.nextInt(),sline.nextInt());
				}
				sline.close();
			}
			//	System.out.println("File inputed");
			fin.close();
			return gh;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} 
		return null;
	}
}

class Graph{
	public final int verNum; //顶点数
	public final int edgeNum; //边数
	private byte[][] adjMatrix; // 邻接矩阵 
	private Vector<ArrayList<Integer>> adjList; //邻接表 
	
	public Graph(int nbVertex, int nbEdge) {
		this.verNum=nbVertex;
		this.edgeNum=nbEdge;
		adjMatrix=new byte[nbVertex+1][nbVertex+1];
		adjList=new Vector<ArrayList<Integer>>(verNum+1);
		for(int i=0;i<=nbVertex;i++)
			adjList.add(new ArrayList<Integer>()); 
	}
	
	//为图添加一条边
	public boolean addEdge(int lnode,int rnode) {
		if(lnode==rnode || lnode > verNum || rnode>verNum || lnode <1  || rnode <1) {
			return false;
		}
		if(adjMatrix[lnode][rnode]==0) {
			adjMatrix[lnode][rnode]=1;
			adjList.get(lnode).add(rnode);
			adjList.get(rnode).add(lnode);
		}
		return true;
	}
	/**
	 * return the adjacent list of vertexID
	 * @param vertexID
	 * @return
	 */
	public ArrayList<Integer> getNeighbors(int vertexID){
		return adjList.get(vertexID);
	}
	
	/**
	 * 对解sol的合法性进行验证，如果合法输出"verified!",否则，输出 "failed!"
	 * @param sol 解：为一个整型数组，顶点 i 的颜色为 sol[i]
	 * @param nbColor 颜色数： 1， 2，3 ，..., nbColor
	 */
	public void verifySolution(int[] sol,int nbColor) {
		if(sol.length<=this.verNum){
			System.out.println("failed!");
			return ;
		}
		for(int v=1;v<=this.verNum;v++) {
			int color=sol[v];
			for(int neibor:this.adjList.get(v)) {
				assert(sol[neibor]>=1 && sol[neibor]<=nbColor);
				if(sol[neibor]==color){
					System.out.println("failed!");
					return ;
				}
			}
		}
		System.out.println("verified!");
	}
	public String toString() {
		return String.format("Graph with %d vertices %d edges\n", this.verNum,this.edgeNum);
	}
}


