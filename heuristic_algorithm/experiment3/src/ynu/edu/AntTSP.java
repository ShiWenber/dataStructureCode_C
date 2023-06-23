package ynu.edu;


import java.awt.*;
import java.io.*;
import java.lang.ref.PhantomReference;
import java.lang.reflect.Array;
import java.util.Scanner;
import java.util.concurrent.CountDownLatch;
import java.util.function.DoubleUnaryOperator;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import javax.print.event.PrintEvent;
import javax.security.auth.PrivateCredentialPermission;

import org.w3c.dom.ranges.Range;

import java.util.ArrayList;
import java.util.Arrays;


public class AntTSP {
    protected int nbCities;

    protected double[][] distMatrix;// 从 1 开始
    protected ArrayList<CityPoint> cities = new ArrayList<CityPoint>(); // 从0开始 第 i 个城市的坐标为 cities.get(i - 1)
    // 信息素浓度 pheromone[i][j]表示从城市i到城市j的信息素浓度
    protected double[][] pheromones;
    protected ArrayList<Ant> ants = new ArrayList<Ant>(); // 从0开始 第 i 个蚂蚁的路径为 ants.get(i - 1).tour
    protected Ant bestAnt; // 最优解

    protected Ant tempBestAnt; // 临时最优解

    // 超参部分
    protected int antNum = 30; // 蚂蚁数量
    protected int maxIter = 300; // 最大迭代次数
    protected double q = 10; // 信息素增加强度
    protected double rho = 0.2; // 信息素挥发系数
    protected int k = 100; // k 次相同的解则停止迭代
    protected double a = 1; // 信息素重要程度
    protected double b = 5; // 启发函数重要程度
    protected double init_pheromone = 100; // 初始信息素浓度


    public AntTSP(String filename, int antNum, int maxIter, double q, double rho, int k, double a, double b,
            double init_pheromone) {
        this.antNum = antNum;
        this.maxIter = maxIter;
        this.q = q;
        this.rho = rho;
        this.k = k;
        this.a = a;
        this.b = b;
        this.init_pheromone = init_pheromone;





        // TODO: initialize nbCities, distMatrix, and cities from the TSP instance file
        // buildDistMatrix(this.distMatrix, filename);
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
                    this.cities.add(new CityPoint(no, x, y));
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Failed to open file " + filename);
        }

        if (cities.size() == 0) {
            System.out.println("Read Instance " + filename + " Failed!");
            return;
        } else {
            int nb_city = cities.size();
            this.distMatrix = new double[nb_city + 1][nb_city + 1];
            CityPoint[] local_cities = new CityPoint[nb_city + 1];
            for (CityPoint p : this.cities) {
                local_cities[p.getCityNo()] = p;
            }
            for (int i = 1; i <= nb_city; i++) {
                for (int j = i + 1; j <= nb_city; j++) {
                    int _x = local_cities[i].getX() - local_cities[j].getX();
                    int _y = local_cities[i].getY() - local_cities[j].getY();
                    this.distMatrix[i][j] = Math.sqrt(_x * _x + _y * _y);
//                    避免相同坐标位置点造成的距离为0的情况
                    this.distMatrix[j][i] = this.distMatrix[i][j];
                }
            }
            this.nbCities = this.cities.size();
        }
        // 初始化信息素浓度
        // System.out.println("distMatrix: " + this.distMatrix + " * " + this.distMatrix[0].length);
        this.pheromones = new double[nbCities + 1][nbCities + 1];
        for (int i = 1; i <= nbCities; i++) {
            for (int j = i + 1; j <= nbCities; j++) {
                // 信息素初始化浓度为1
                this.pheromones[i][j] = this.init_pheromone;
                this.pheromones[j][i] = this.pheromones[i][j];
            }
        }
//        for (int i = 0; i < antNum; i++) {
//            // 随机放置m只蚂蚁的起点
//            int startCity = (int) (Math.random() * nbCities) + 1;
//            // 为每只蚂蚁建立禁忌表tabuk，将各蚂蚁的初始节点置入禁忌表中
//            Ant ant = new Ant(startCity, this.nbCities);
//            this.ants.add(ant);
//        }
//        this.bestAnt = this.ants.get(0);
        System.out.println("Read Instance " + filename + " Successfully!");
    }

    public int[] solve() {

//        总时间
        long whole_startTime = System.currentTimeMillis();
//        根据超参生成log_name
        String log_name = "log_" + "antNum-" + this.antNum + "_maxIter-" + this.maxIter + "_q-" + this.q + "_rho-" + this.rho + "_k-" + this.k + "_a-" + this.a + "_b-" + this.b + "_initP" + this.init_pheromone + ".csv";
        System.out.println("log_name: " + log_name);
//        记录日志文件 log.csv

        Logger logger = null;
        try {
            logger = new Logger("log/" + log_name);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }


        //         重新初始化所有变量
        for (int i = 1; i <= nbCities; i++) {
            for (int j = i + 1; j <= nbCities; j++) {
                // 信息素初始化浓度为
                this.pheromones[i][j] = this.init_pheromone;
                this.pheromones[j][i] = this.pheromones[i][j];
            }
        }
        // TODO: implement the ant colony optimization algorithm to find the best tour
        // 随机放置m只蚂蚁的起点，为每只蚂蚁建立禁忌表tabuk，将各蚂蚁的初始节点置入禁忌表中;
        for (int i = 0; i < antNum; i++) {
            // 随机放置m只蚂蚁的起点
            int startCity = (int) (Math.random() * nbCities) + 1;
            // 为每只蚂蚁建立禁忌表tabuk，将各蚂蚁的初始节点置入禁忌表中
            Ant ant = new Ant(startCity, this.nbCities);
            this.ants.add(ant);
        }

//        this.bestAnt = this.ants.get(0);

        this.bestAnt = new Ant(this.ants.get(0));
        // 这里 bestAnt 的 getPathLength 为 0
        this.tempBestAnt = this.ants.get(0);

        int count = 1;
        double best_length = Double.MAX_VALUE; // 记录最优解的长度
        int best_count = 0; // 记录最优解连续出现的次数
        while (count <= maxIter) {
            // 对每只蚂蚁，根据信息素浓度和启发函数，选择下一个节点;
//			记录开始时间
            long startTime = System.currentTimeMillis();
            // TODO 添加并行

            // 清空蚂蚁的禁忌表，重新放置蚂蚁的起点;
//            Stream<Ant> antStream = ants.parallelStream();
//            antStream.parallel().forEach(ant -> {
//                // 随机放置m只蚂蚁的起点
//                int startCity = (int) (Math.random() * nbCities) + 1;
//                // 让每只蚂蚁重新开始
//                ant.reset(startCity);
//            });


//            并行过程中不可比较最小值，必须等待并行结束
            Stream<Ant> antStream = ants.parallelStream();
            antStream.parallel().forEach(ant -> {
                // 随机放置m只蚂蚁的起点
                int startCity = (int) (Math.random() * nbCities) + 1;
                assert startCity > 0 && startCity <= nbCities;
                // 让每只蚂蚁重新开始
                ant.reset(startCity);
//            for (Ant ant : ants) {
                // 对每只蚂蚁，完成一个complete tour; 这里不可并行会有错误
                // IntStream.range(0, ynu.edu.AntTSP.this.nbCities).forEach(i -> {
//                不必计算最后一个节点，最后一个点必然选择
                for (int i = 0; i < AntTSP.this.nbCities; i++) {
//                    ant.updateNextCityProbabilities(AntTSP.this.distMatrix, AntTSP.this.pheromones, this.a, this.b);
//                    ant.chooseNextCity(this.pheromones, this.distMatrix);
                    ant.move1step(this.pheromones, this.distMatrix, this.a, this.b);
//                    System.out.println("ant_visitedCityIds: " + ant.visitedCityIds.size());
                }

            });
//            }


//            并行过程中不可比较最小值，必须等待并行结束，这里没必要并行，反而提高开销，也没有明显提速
            antStream = ants.parallelStream();
            antStream.parallel().forEach(ant -> {
//            for (Ant ant : this.ants) {
//                assert ant.visitedCityIds.size() == nbCities : ant_id + "ant.visitedCityIds.size()" + ant.visitedCityIds.size() + "nbCities" + nbCities;
                if (this.tempBestAnt.getPathLength() == 0 && ant.getPathLength() != 0) {
                    this.tempBestAnt = ant;
                }
                if (ant.getPathLength() < this.tempBestAnt.getPathLength() && ant.getPathLength() != 0) {
                    this.tempBestAnt = ant;
                }
//            }
            });

            assert tempBestAnt != null;
            assert tempBestAnt.getTour().length == nbCities + 1 : "bestAnt.getTour().length" + tempBestAnt.getTour().length + "nbCities" + nbCities;
            // 记录结束时间
            long endTime = System.currentTimeMillis();


            // 中间过程
//            System.out.println(count + "time: " + (endTime - startTime) + "ms");
//            System.out.println(count + "bestPathLength: " + tempBestAnt.getPathLength());

            // 更新信息素浓度矩阵;
            updatePheromone();


            // ⑷判断是否达到停止条件，若达到则输出最优解，否则返回⑵;
            // 如果100次迭代后，最优解没有变化，就认为已经收敛
            if (bestAnt.getPathLength() > this.tempBestAnt.getPathLength() || this.bestAnt.getPathLength() == 0) {
                best_length = tempBestAnt.getPathLength();
                best_count = 0;
                this.bestAnt = new Ant(this.tempBestAnt);
            } else if (bestAnt.getPathLength() == this.tempBestAnt.getPathLength()) {
                best_count++;
            }

            // k 次相同的最优解，表明已经收敛
            if (best_count >= this.k) {
                //                输出总时间
                System.out.println("total time: " + (System.currentTimeMillis() - whole_startTime) / 1000 + "s");
                // 输出最优解
                return bestAnt.getTour();
            }


            try {
                logger.log(count, this.tempBestAnt.getPathLength(), endTime - startTime);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            count++;
        }

        // 关闭文件资源
        try {
            logger.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        //                输出总时间
        System.out.println("total time: " + (System.currentTimeMillis() - whole_startTime) / 1000 + "s");
        return bestAnt.getTour();
    }

    /**
     * 更新信息素浓度矩阵
     */
    private void updatePheromone() {
        IntStream.range(0, nbCities).parallel().forEach(i -> {
            IntStream.range(0, nbCities).parallel().forEach(j -> {
                AntTSP.this.pheromones[i][j] = rho * AntTSP.this.pheromones[i][j];
//                AntTSP.this.pheromones[j][i] = AntTSP.this.pheromones[i][j];
            });
        });
        // IntStream.range(0, antNum).parallel().forEach(i -> {
        // Ant ant = ants.get(i);

        // });
        Stream<Ant> antStream = ants.parallelStream();
        antStream.parallel().forEach(i -> {
            // antStream.forEach(i -> {
//        for (Ant i : this.ants) {
            // 信息素增加强度
            double res[][] = i.getDeltaPheromones(this.distMatrix);
            // double deltaP = res[0];
            // int from = (int)res[1];
            // int to = (int)res[2];
//            IntStream.range(0, res.length).parallel().forEach(j -> {
            for (int j = 0; j < res.length; j++) {
                int from = (int) res[j][0];
                int to = (int) res[j][1];
                double deltaP = res[j][2];
                // 每只蚂蚁都找到完全路径后再更新信息素浓度矩阵
                AntTSP.this.pheromones[from][to] += deltaP;
//                AntTSP.this.pheromones[to][from] = AntTSP.this.pheromones[from][to];
//            });
            }
//        }
        });
    }

    private class PheromoneMatrix {
        // TODO: define the pheromone matrix class and its behavior
    }

    private void buildDistMatrix(double[][] distMatrix, String filename) {
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
                    this.cities.add(new CityPoint(no, x, y));
                }
            }
        } catch (FileNotFoundException e) {
            System.out.println("Failed to open file " + filename);
        }

        if (cities.size() == 0) {
            System.out.println("Read Instance " + filename + " Failed!");
            return;
        } else {
            int nb_city = cities.size();
            distMatrix = new double[nb_city + 1][nb_city + 1];
            CityPoint[] local_cities = new CityPoint[nb_city + 1];
            for (CityPoint p : this.cities) {
                local_cities[p.getCityNo()] = p;
            }
            for (int i = 1; i <= nb_city; i++) {
                for (int j = i + 1; j <= nb_city; j++) {
                    int _x = local_cities[i].getX() - local_cities[j].getX();
                    int _y = local_cities[i].getY() - local_cities[j].getY();
                    distMatrix[i][j] = Math.sqrt(_x * _x + _y * _y);
                    distMatrix[j][i] = distMatrix[i][j];
                }
            }
            this.nbCities = this.cities.size();
        }

    }

    // 蚂蚁只有在AntTSP类中才能更简单的共享信息素浓度矩阵
    private class Ant {
        private int lastCityId;
        private int currentCityId;
        private ArrayList<Integer> visitedCityIds = new ArrayList<>();
        private double pathLength = 0.0;
        private double[] nextCityProbabilities; // 从 1 开始

        public int[] getTour() {
//            System.out.println("nbCities: " + nbCities);
//            System.out.println("visitedCityIds.size(): " + visitedCityIds.size());
            assert visitedCityIds.size() == nbCities : "visitedCityIds.size()" + visitedCityIds.size() + " != nbCities" + nbCities;
            int[] tour = new int[nbCities + 1];
            for (int i = 1; i < nbCities + 1; i++) {
                tour[i] = visitedCityIds.get(i - 1);
            }
            return tour;
        }

        public Ant(int startCityId, int nbCities) {
            this.currentCityId = startCityId;
            this.visitedCityIds.add(startCityId);
            this.nextCityProbabilities = new double[nbCities + 1];
        }

        /**
         * 拷贝构造函数
         */
        public Ant(Ant ant) {
            this.lastCityId = ant.lastCityId;
            this.currentCityId = ant.currentCityId;
            this.visitedCityIds = new ArrayList<>(ant.visitedCityIds);
            this.pathLength = ant.pathLength;
            this.nextCityProbabilities = new double[ant.nextCityProbabilities.length];
            System.arraycopy(ant.nextCityProbabilities, 0, this.nextCityProbabilities, 0, ant.nextCityProbabilities.length);
        }

        public void reset(int startCityId) {
            this.currentCityId = startCityId;
            this.pathLength = 0.0;
            this.visitedCityIds.clear();
            this.visitedCityIds.add(startCityId);
        }

        public void updateNextCityProbabilities(double[][] distMatrix, double[][] pheromone, double alpha, double beta) {
            if (this.visitedCityIds.size() == nbCities || this.visitedCityIds.size() == nbCities - 1) {
                return;
            }
            // 概率总和，用于计算概率分布
            double total = 0.0;
            // 更新概率分布
            for (int i = 1; i < nbCities + 1; i++) {
                if (!visitedCityIds.contains(i)) {
//                    最后一个点
                    // probabilities[i] = Math.pow(ynu.edu.AntTSP.this.pheromones[currentCityId][i],
                    // ynu.edu.AntTSP.this.a) *
                    // Math.pow(1.0 /ynu.edu.AntTSP.this.distMatrix[currentCityId][i], ynu.edu.AntTSP.this.b);
                    double dist = distMatrix[currentCityId][i];
                    if (dist == 0.0) {
                        dist = 1e-7;
                    }
                    assert dist != 0.0; // a280.tsp 有两个城市坐标相同,171 和 172 因此可能出现除0错误，必须检查并在初始化时处理为一个很小的数
                    this.nextCityProbabilities[i] = Math.pow(pheromones[currentCityId][i], alpha) *
                            Math.pow(1.0 / dist, beta);
                    assert Double.isInfinite(this.nextCityProbabilities[i]) == false;
                    assert Double.isNaN(this.nextCityProbabilities[i]) == false;
//                   保证未访问过的城市去向道路的概率不为0，避免蚂蚁找不到路
                    if (this.nextCityProbabilities[i] == 0.0) {
                        this.nextCityProbabilities[i] = 1e-7;
                    }
                    assert this.nextCityProbabilities[i] > 0.0 : "nextCityProbabilities: " + Arrays.toString(this.nextCityProbabilities) + "\n" +
                            "pheromones: " + Arrays.toString(pheromones[currentCityId]) + "\n" +
                            "currentCityId: " + currentCityId + "\n" +
                            "visitedCityIds: " + visitedCityIds.size() + "\n";
                    total += this.nextCityProbabilities[i];
                } else {
                    this.nextCityProbabilities[i] = 0.0;
                }
            }
            // 计算概率分布
//            if (total == 0 && visitedCityIds.size() == nbCities) {
//                return;
//            }
            assert total != 0.0 : "nextCityProbabilities: " + Arrays.toString(this.nextCityProbabilities) + "\n" +
                    "pheromones: " + Arrays.toString(pheromones[currentCityId]) + "\n" +
                    "currentCityId: " + currentCityId + "\n" +
                    "visitedCityIds: " + visitedCityIds.size() + "\n";

            for (int i = 1; i < nbCities + 1; i++) {
                this.nextCityProbabilities[i] /= total;
                assert Double.isNaN(this.nextCityProbabilities[i]) == false;
            }
//			System.out.println("nextCityProbabilities: " + Arrays.toString(this.nextCityProbabilities));
        }

        /**
         * 问题：有可能没有移动，问题出现的原因，nextCityProbabilities[i] = NaN nextCityProbabilities中出现了NaN
         *
         * @param pheromones
         * @param distances
         */
        public void chooseNextCity(double[][] pheromones, double[][] distances) {
            int moved = 0;
            // 从外部类获取nbCities
            // int nbCities = ynu.edu.AntTSP.this.nbCities;
            int nbCities = pheromones.length - 1;
            if (this.visitedCityIds.size() == nbCities) {
                this.lastCityId = this.currentCityId;
                this.currentCityId = this.visitedCityIds.get(0);
                updatePath(AntTSP.this.distMatrix);
//                删除最后一个点
                this.visitedCityIds.remove(this.visitedCityIds.size() - 1);
                moved = 1;
                return;
            }
            //                    只剩一个不用轮盘赌
            if (this.visitedCityIds.size() == nbCities - 1) {
//                找到不在路径中的
                for (int i = 1; i < nbCities + 1; i++) {
                    if (!visitedCityIds.contains(i)) {
                        this.lastCityId = this.currentCityId;
                        currentCityId = i;
                        // 计算路径长度
                        updatePath(AntTSP.this.distMatrix);
                        moved = 1;
                        break;
                    }
                }
                assert moved == 1 : "k " + k;
                return;
            }

            // 选择下一个城市，轮盘赌策略 TODO (更加随机的改动)
            double rand = Math.random();
            assert rand >= 0.0 && rand <= 1.0;
            double cumulativeProbability = 0.0;
            int k = 0;
            for (int i = 1; i < nbCities + 1; i++) {
                k = i;
                if (!visitedCityIds.contains(i)) {
//                    if (visitedCityIds.size() == nbCities - 2) {
//                        System.out.println("i: " + i);
//                        System.out.println("nextCityProbabilities[i]: " + nextCityProbabilities[i]);
//                        System.out.println("cumulativeProbability: " + cumulativeProbability);
//                    }
                    cumulativeProbability += this.nextCityProbabilities[i];
                    if (rand <= cumulativeProbability) {
                        // 加入禁忌
                        this.lastCityId = this.currentCityId;
                        currentCityId = i;
                        // 计算路径长度
                        updatePath(AntTSP.this.distMatrix);
                        moved = 1;
                        break;
                    }
                }
            }
            assert moved == 1 : "k " + k + " size " + this.visitedCityIds.size() + " nextCityProbabilities: " + Arrays.toString(this.nextCityProbabilities);
        }

        public void move1step(double[][] pheromones, double[][] distMatrix, double alpha, double beta) {
            int temp = this.visitedCityIds.size();
            updateNextCityProbabilities(distMatrix, pheromones, alpha, beta);
            chooseNextCity(pheromones, distMatrix);
//            assert temp + 1 == this.visitedCityIds.size() : "temp: " + temp + ", visitedCityIds.size(): " + this.visitedCityIds.size();
        }

        /**
         * 更新路径长度，在 currentCityId 被更新之后调用
         *
         * @param distances
         */
        public void updatePath(double[][] distances) {
            // int lastCityId = visitedCityIds.get(visitedCityIds.size() - 1);
            int lastCityId = this.lastCityId;
            pathLength += distances[lastCityId][currentCityId];
            visitedCityIds.add(currentCityId);
        }

        /**
         * 输出 deltaPheromones，以及该段路径的起点城市编号，终点城市编号
         * 注意使用的路径长度是蚂蚁一次迭代完成后的路径长度
         *
         * @param distances
         * @return [startCityId, endCityId, deltaPheromones] * (nbCities - 1)
         */
        public double[][] getDeltaPheromones(double[][] distances) {
            assert visitedCityIds.size() == nbCities;
            // 设置三维的deltaPheromones数组
            double[][] deltaPheromones = new double[visitedCityIds.size()][3];
            // 遍历蚂蚁访问过的城市
            IntStream.range(0, visitedCityIds.size() - 1).forEach(i -> {
                int startCityId = visitedCityIds.get(i);
                int endCityId = visitedCityIds.get(i + 1);
                deltaPheromones[i][0] = startCityId;
                deltaPheromones[i][1] = endCityId;
                deltaPheromones[i][2] = AntTSP.this.q / pathLength;
            });
            // 获得最后一行的信息素浓度
            deltaPheromones[visitedCityIds.size() - 1][0] = visitedCityIds.get(visitedCityIds.size() - 1);
            deltaPheromones[visitedCityIds.size() - 1][1] = visitedCityIds.get(0);
            deltaPheromones[visitedCityIds.size() - 1][2] = AntTSP.this.q / pathLength;
            return deltaPheromones;
        }

        public ArrayList<Integer> getVisitedCityIds() {
            return visitedCityIds;
        }

        public double getPathLength() {
            return pathLength;
        }
    }
}