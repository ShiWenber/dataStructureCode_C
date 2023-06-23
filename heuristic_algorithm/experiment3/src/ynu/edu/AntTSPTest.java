package ynu.edu;

import java.io.*;
import java.util.*;

public class AntTSPTest {

    private static int NB_CITIES; // 城市数量
    private static double distMatrix[][]; // 距离矩阵
    private static ArrayList<CityPoint> cityPoints = new ArrayList<>();// 城市坐标

    public static void main(String[] args) {

        AntTSP antTSP = new AntTSP("a280.tsp");

        int[] bestTour = antTSP.solve(); // 返回蚁群算法计算的最佳路径

        for (int i = 0; i < bestTour.length; i++)
            System.out.print(bestTour[i] + " ");
        System.out.println();
        // 对结构进行测试

        buildDistMatrix("a280.tsp");
        double bestValue = getTourLength(bestTour); // 计算路径总长度
        int[] optTour = getOptTour("a280.opt.tour"); // 读取已知的最优解
        double optValue = getTourLength(optTour); // 最优解的路径总长度
        System.out.println("best " + bestValue);
        System.out.println("opt " + optValue);
        double gap = (bestValue - optValue) / optValue;
        if (gap >= 0 && gap <= 0.05)
            System.out.println("The gap is " + gap + ", passed!"); // 通过当与最优解的距离小于10%
        else
            System.out.println("The gap is " + gap + ", failed!");
    }

    private void checkTour(int tour[]) {
        int[] flag = new int[NB_CITIES];
        assert (tour.length == NB_CITIES);
        for (int i = 0; i < NB_CITIES; i++)
            flag[i] = 0;

        for (int i = 0; i < NB_CITIES; i++) {
            assert (flag[tour[i]] == 0);
            flag[tour[i]] = 1;
        }

    }

    private static double getTourLength(int[] tour) {
        double optLength = 0;
        // int[] flag=new int[NB_CITIES]
        for (int i = 0; i < NB_CITIES - 1; i++)
            optLength += distMatrix[tour[i]][tour[i + 1]];
        optLength += distMatrix[tour[0]][tour[NB_CITIES - 1]];
        return optLength;
    }

    private static int[] getOptTour(String filename) {
        Scanner fin;
        int[] optTour = new int[NB_CITIES];
        try {
            fin = new Scanner(new FileReader(filename));
            while (fin.hasNextLine()) {
                String line = fin.nextLine();
                if (line.startsWith("TOUR_SECTION"))
                    break;
            }
            int i = 0;
            while (fin.hasNextInt()) {
                int node = fin.nextInt();
                if (node == -1)
                    break;
                optTour[i++] = node;
            }
            assert i == NB_CITIES;
            double optLength = 0;
            for (i = 0; i < NB_CITIES - 1; i++)
                optLength += distMatrix[optTour[i]][optTour[i + 1]];
            return optTour;
        } catch (FileNotFoundException e) {
            System.out.println("Failed to open opt tour file " + filename);
        }
        return optTour;
    }

    private static void buildDistMatrix(String filename) {
        Scanner fin;
        try {
            fin = new Scanner(new FileReader(filename));
            while (fin.hasNextLine()) {
                String line = fin.nextLine();
                if (line.startsWith("NODE_COORD_SECTION"))
                    break;
            }
            while (fin.hasNextLine()) {
                String line = fin.nextLine();
                if (line.startsWith("EOF"))
                    break;
                try (Scanner in = new Scanner(line)) {
                    int no = in.nextInt();
                    int x = in.nextInt();
                    int y = in.nextInt();
                    cityPoints.add(new CityPoint(no, x, y));
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Failed to open file " + filename);
        }

        if (cityPoints.size() == 0) {
            System.out.println("Read Instance " + filename + " Failed!");
            return;
        } else {
            int nb_city = cityPoints.size();
            CityPoint[] cities = new CityPoint[nb_city + 1];
            for (CityPoint p : cityPoints) {
                cities[p.getCityNo()] = p;
            }
            distMatrix = new double[nb_city + 1][nb_city + 1];
            for (int i = 1; i <= nb_city; i++) {
                for (int j = i + 1; j <= nb_city; j++) {
                    int _x = cities[i].getX() - cities[j].getX();
                    int _y = cities[i].getY() - cities[j].getY();
                    distMatrix[i][j] = distMatrix[j][i] = Math.sqrt(_x * _x + _y * _y);
                }
            }
            NB_CITIES = cityPoints.size();
        }

    }
}

class CityPoint {
    private int cityNo;
    private int x;
    private int y;

    public CityPoint(int no, int x, int y) {
        this.cityNo = no;
        this.x = x;
        this.y = y;
    }

    public int getCityNo() {
        return this.cityNo;
    }

    public int getX() {
        return this.x;
    }

    public int getY() {
        return this.y;
    }
}
