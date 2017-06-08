import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;
public class ex5 {

	private Graph graph;
	public Integer[] distances;
	ArrayList<GraphNode> predecessors;
	
	public ex5(Graph g) {
		this.graph=g;
	}
	
	public Integer[] getMaxDistance() {
		Integer[] dist= {-1,-1};
		for(int i=0; i< distances.length; i++){
			if (dist[0]<this.distances[i]){
				dist[0]=this.distances[i];
				dist[1]=i;
			}
		}
		return dist;
	}
	
	public void performDijkstra(GraphNode d ) {		
		PriorityQueue<GraphNode> heap = new PriorityQueue<GraphNode>(new GraphNodeComparator(this));
		
		distances = new Integer[graph.getNumOfNodes()];
		predecessors = new ArrayList<GraphNode >(graph.getNumOfNodes());
		
		for( int i = 0; i < distances.length; ++i ){
			distances[i] = null;
			predecessors.add(null);
		}
		distances[d.id()] = 0;
		
		//System.out.println("domainSize = " + graph.getDomainSize());
		//System.out.println("numOfNodes = " + graph.getNumOfNodes());
		
		heap.add( d );
		
		while( !heap.isEmpty() ) {
			GraphNode fixed = heap.poll();
			// update adjacent nodes
			for( GraphEdge out : fixed.edges ) {
				Integer oldDistance = distances[out.getEnd().id()];				
				if(oldDistance == null){
					distances[out.getEnd().id()] = distances[fixed.id()] + out.getCost();
					predecessors.set(out.getEnd().id(), fixed);
					heap.add(graph.getNode(out.getEnd().id()));
				}else if( distances[fixed.id()] + out.getCost() < oldDistance ){		
					distances[out.getEnd().id()] = distances[fixed.id()] + out.getCost();
					predecessors.set(out.getEnd().id(), fixed);
					heap.add( out.getEnd() );
				}
			}
		}		
	}

	


	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Graph g;
		try {
			BufferedReader br= new BufferedReader(new FileReader("acycpos-1.gph"));
			
		
		String brline=br.readLine();
		g= new Graph(Integer.parseInt(brline.split(" ")[0]));

	
		while ((brline=br.readLine())!=null){
				String[] linesplit= brline.split(" ");
				assert(linesplit.length==3);	
				try {
				g.addEdge(Integer.parseInt(linesplit[0])-1, Integer.parseInt(linesplit[1])-1, Integer.parseInt(linesplit[2]));
				}catch (IllegalArgumentException e){
					System.out.println("edge already exists: "+brline);
				}
				
			}		
		br.close();
		//System.out.println(g.toString());	
		//System.out.println(g.nodes.toString());
		
		
		ex5 ex = new ex5(g);
		ex.performDijkstra(g.getNode(0));
		Integer[] dist=ex.getMaxDistance();
		System.out.println("RESULT VERTEX "+dist[1]);	
		System.out.println("RESULT DIST "+dist[0]);
		
		
		}catch (FileNotFoundException e){
			System.out.println("file not found");	
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
class GraphNodeComparator implements Comparator<GraphNode>{
	private ex5 ex;
	public GraphNodeComparator(ex5 ex){
		this.ex=ex;
	}

 	public int compare(GraphNode a, GraphNode b)
    {
	 	return ex.distances[a.id()] - ex.distances[b.id()];
    }
	 
}
