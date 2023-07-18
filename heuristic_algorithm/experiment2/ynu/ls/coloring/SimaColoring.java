package ynu.ls.coloring;

import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;
import java.util.stream.IntStream;

import javax.print.event.PrintEvent;

// 模拟退火算法

public class SimaColoring {
    private Graph G;
    private int[] best_sol;
    private int nbColor;
    private int[][] adj_score;
    private int conflict;
    private int[] sol;

    public SimaColoring(Graph g, int nbColor) {
        this.G = g;
        this.best_sol = new int[g.verNum + 1];
        this.nbColor = 54;
        this.adj_score = new int[g.verNum + 1][this.nbColor + 1];
        this.sol = new int[G.verNum + 1];
        // 初始化产生随机解
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            sol[i] = (int) (Math.random() * this.nbColor) + 1;
        });
        // stream.parallel() 并行流加速
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            IntStream.range(1, this.nbColor + 1).parallel().forEach(j -> {
                adj_score[i][j] = 0;
            });
        });

        // 并行加速 初始化
        CompletableFuture<Void> task1 = CompletableFuture.runAsync(() -> calAdjScore(sol));
        CompletableFuture<Integer> task3 = CompletableFuture.supplyAsync(() -> calConflict(sol));

        CompletableFuture<Void> allTasks = CompletableFuture.allOf(task1, task3);
        allTasks.join();

        int conflict_local = 0;

        try {
            // 获取calConflict的返回值
            conflict_local = task3.get();
        } catch (Exception e) {
            System.out.println(e);
        }
        this.conflict = conflict_local;
        System.out.println(conflict_local);

    }

    public int[] search() {
        // 记录开始时间
        long startTime = System.currentTimeMillis();

        int[] sol = new int[G.verNum + 1];
        // 初始化产生随机解
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            sol[i] = (int) (Math.random() * this.nbColor) + 1;
        });

        // stream.parallel() 并行流加速
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            IntStream.range(1, this.nbColor + 1).parallel().forEach(j -> {
                adj_score[i][j] = 0;
            });
        });

        // 并行加速 初始化
        CompletableFuture<Void> task1 = CompletableFuture.runAsync(() -> calAdjScore(sol));
        CompletableFuture<Integer> task3 = CompletableFuture.supplyAsync(() -> calConflict(sol));

        CompletableFuture<Void> allTasks = CompletableFuture.allOf(task1, task3);
        allTasks.join();

        int conflict_local = 0;

        try {
            // 获取calConflict的返回值
            conflict_local = task3.get();
        } catch (Exception e) {
            System.out.println(e);
        }
        this.conflict = conflict_local;
        System.out.println(conflict_local);

        // int conflict_local = this.conflict;

        // 设置退火参数
        double T = 100.0; // 初始温度
        // 设置冷却速率(退火系数)
        double alpha = 0.9999;
        double end_T = 0.001; // 终止温度
        int iter = 1000; // 等温迭代次数
        int iter_count = 0; // 迭代次数

        int random_v = 0;
        int random_c_i = 0;
        int random_c_j = 0;
        int random_moveScore = Integer.MIN_VALUE;
        int conflictDelta = 0;
        while (T > end_T && this.conflict > 0) {
            for (int i = 0; i < iter; i++, iter_count++) {
                // 产生一个随机解
                // // 产生一个随机值在[1, G.verNum]之间的整数
                random_v = randInt(G.verNum) + 1;
                assert (random_v >= 1 && random_v <= G.verNum);
                // random_c_i = randInt(nbColor) + 1;
                // assert (random_c_i >= 1 && random_c_i <= nbColor);
                random_c_i = sol[random_v]; // c_i不可随机，必须是当前顶点的颜色
                random_c_j = randInt(nbColor) + 1;
                random_moveScore = this.adj_score[random_v][random_c_i] - this.adj_score[random_v][random_c_j];
                conflictDelta = -random_moveScore;
                assert (random_c_j >= 1 && random_c_j <= nbColor);
                if (random_c_i == random_c_j) {
                    continue;
                }
                // 若当前点已经无冲突
                if (adj_score[random_v][random_c_i] == 0) {
                    continue;
                }

                // // 若当前点已经无冲突则概率接受差解
                // if (adj_score[random_v][sol[random_v]] == 0) {
                // continue;
                // } else if (Math.random() < 0.7) {
                // continue;
                // }
                // 更好的解
                if (conflictDelta < 0) {
                    move(sol, random_v, random_c_i, random_c_j);
                    conflict_local += conflictDelta;
                    // if (conflict_local != calConflict(sol)) {
                    // System.out.println("conflict_local: " + conflict_local);
                    // System.out.println("calConflict(sol): " + calConflict(sol));
                    // }
                    assert (conflict_local == calConflict(sol));
                } else {
                    // 以一定概率接受更差的解
                    // if (Math.exp(-(double)conflictDelta / T) < 0.5) {
                    //     System.out.println((Math.exp(-(double)conflictDelta / T)));
                    // }
                    if (Math.random() < Math.exp(-(double)conflictDelta / T)) {
                        move(sol, random_v, random_c_i, random_c_j);
                        conflict_local += conflictDelta;
                        // if (conflict_local != calConflict(sol)) {
                        // System.out.println("conflict_local: " + conflict_local);
                        // System.out.println("calConflict(sol): " + calConflict(sol));
                        // }
                        assert (conflict_local == calConflict(sol));
                    }
                }

                iter_count++;
            }
            if (conflict_local < this.conflict) {
                // 保存最优解
                this.conflict = conflict_local;
                System.arraycopy(sol, 0, this.best_sol, 0, sol.length);
                System.out.println(this.conflict);
            }
            T *= alpha;
        }

        // -----------

        // --------
        long endTime = System.currentTimeMillis();
        System.out.println("iter_count：" + iter_count);
        System.out.println("SA算法运行时间：" + (endTime - startTime) + "ms");
        System.out.println(T);
        System.out.println(this.conflict);
        return this.best_sol;
    }

    /**
     * 产生随机整数
     * 
     * @param i
     * @return
     */
    private int randInt(int i) {
        return (int) (Math.random() * i);
    }

    /**
     * 计算冲突数
     * 
     * @param sol
     * @return
     */
    public int calConflict(int[] sol) {
        int res = 0;
        for (int i = 1; i < G.verNum + 1; i++) {
            ArrayList<Integer> neighbors = G.getNeighbors(i);
            // todo
            for (int v : neighbors) {
                if (sol[v] == sol[i]) {
                    res++;
                }
            }
        }
        assert (res % 2 == 0);
        // 冲突边会重复计算，因此要除以2
        return res / 2;
    }

    public void calAdjScore(int[] sol) {
        // for (int i = 1; i < G.verNum + 1; i++) {
        // ArrayList<Integer> neighbors = G.getNeighbors(i);
        // for (int v : neighbors) {
        // this.adj_score[i][sol[v]] += 1;
        // }
        // }
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            ArrayList<Integer> neighbors = G.getNeighbors(i);
            for (int v : neighbors) {
                synchronized (this) {
                    this.adj_score[i][sol[v]] += 1;
                }
            }
        });
    }

    // /**
    // * 计算移动评分表
    // *
    // * @param sol
    // */
    // public void calMoveScore(int[] sol, int[][] moveScore2) {
    // int conflict_neighbor_origin = 0;
    // int conflict_neighbor_new = 0;
    // for (int i = 1; i < G.verNum + 1; i++) {
    // for (int color = 1; color < nbColor + 1; color++) {
    // conflict_neighbor_new = 0;
    // conflict_neighbor_origin = 0;
    // // 计算原有冲突数
    // ArrayList<Integer> neighbors = G.getNeighbors(i);
    // for (int v : neighbors) {
    // if (sol[v] == sol[i]) {
    // conflict_neighbor_origin++;
    // }
    // if (sol[v] == color) {
    // conflict_neighbor_new++;
    // }
    // }
    // moveScore2[i][color] = conflict_neighbor_origin - conflict_neighbor_new;
    // }
    // }
    // }

    /**
     * 移动顶点并更新moveScore,sol表
     * 
     * @param sol 当前解
     * @param v   改变的顶点
     * @param c_i 顶点原来的颜色
     * @param c_j 顶点改变后的颜色
     * @return 返回冲突数的变化
     */
    public void move(int[] sol, int v, int c_i, int c_j) {
        // 更新范围：v和v的邻居
        // 更新邻居的moveScore
        sol[v] = c_j;

        // // 更新 adj_score
        ArrayList<Integer> neighbors = G.getNeighbors(v);
        for (int u : neighbors) {
            this.adj_score[u][c_i]--;
            this.adj_score[u][c_j]++;
        }
    }

    /**
     * 对比两个二维数组，查看数据是否一致
     * 
     * @param moveScore
     * @param moveScore2
     * @return
     */
    public boolean equal_array(int[][] moveScore, int[][] moveScore2) {
        for (int i = 1; i < G.verNum + 1; i++) {
            for (int j = 1; j < nbColor + 1; j++) {
                if (moveScore[i][j] != moveScore2[i][j]) {
                    System.out.println("error");
                    return false;
                }
            }
        }
        return true;
    }
}