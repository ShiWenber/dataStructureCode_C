import java.io.FileReader;
import java.util.Scanner;

public class AntTSP {

    private final int NB_CITIES;
    private double[][] distMatrix;
    private double[][] pheromone;
    private CityPoint[] cities;

    /**
     * 构造函数和初始化
     * @param filename
     * @param antCount
     */
    public AntTSP(String filename, int antCount) {
        read_instance(filename);
    }

    public static void main(String[] args) {
        System.out.println("");
        AntTSP antTsp = new AntTSP(args[0],10);
        antTsp.solve();
        antTsp.getBestTourLength();
        antTsp.getBestTour();
    }

    private void getBestTour() {
    }

    private void getBestTourLength() {
    }

    private void solve() {
    }

    /**
     * 读取TSP问题的实例文件
     * @param filename
     * @return
     */
    private int read_instance(String filename) {
        Scanner fin;
        
        try {
            fin = new Scanner(new FileReader(filename));
            while (fin.hasNextLine()) {
                String line = fin.nextLine();
                // 读取 DIMENSION 的数值作为 NB_CITIES
                // 如果line以"DIMENSION:"开始
                if (line.startsWith("DIMENSION:")) {
                    String nb_cities = (line.replaceAll("DIMENSION:", ""));
                    System.out.println("nb_cities: " + nb_cities);
                }

                if (line.equals("NODE_COORD_SECTION")) {
                    break;
                }
            }
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
        for (int i = 0; i < NB_CITIES; i++) {
            int city = fin.nextInt();
            double x = fin.nextDouble();
            double y = fin.nextDouble();
            CityPoint cityPoint = new CityPoint(city, x, y);
            this.cities[i] = cityPoint;
        }
    }

    private void createAnts(int antCount) {
        AntAgent(NB_CITIES, i +2, this.distMatrix, this) // 提供共享信息
    }

}

class AntAgent {
    // dist和phe是所有agents共享的
    private final double[][] distMatrix; 
    private double[][] pheromone;
    private int StartCity, CityCount;

    // 表示一条路径，这个变量是agent独有的
    private int tour[];

    public void findTour() {
    }

    /**
     * 释放信息素
     */
    public void liberatePheromone() {
    } 
    
}
