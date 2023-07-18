package ynu.ls.coloring;
import java.util.ArrayList;
import java.util.concurrent.CompletableFuture;
import java.util.stream.IntStream;

// 模拟退火算法

public class SimaColoring {
    private Graph G;
    private int[] best_sol;
    private int nbColor;
    private int[][] adj_score;
    private int conflict;
    private int max_iter;

    public SimaColoring(Graph g, int nbColor) {
        this.G = g;
        this.best_sol = new int[g.verNum + 1];
        this.nbColor = nbColor;
        this.adj_score = new int[g.verNum + 1][nbColor + 1];
    }

    public int[] search() {
        int[] sol = new int[G.verNum + 1];
        // 初始化产生随机解
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            sol[i] = (int) (Math.random() * this.nbColor) + 1;
        });
        // stream.parallel() 并行流加速
        IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
            IntStream.range(1, nbColor + 1).parallel().forEach(j -> {
                adj_score[i][j] = 0;
            });
        });
        System.arraycopy(sol, 0, this.best_sol, 0, sol.length);

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

        // 设置退火参数
        double T = 1000.0; // 初始温度
        // 设置冷却速率(退火系数)
        double alpha = 0.99;
        int iter = 1;
        int max_iter = 1000;
        double end_T = 0.01; // 终止温度
		// int maxMoveScore = moveScore[1][1]; // 一次iter 中最大的move-score
		// 初始化为最小值
		int maxMoveScore = Integer.MIN_VALUE;
		int bestMove_v = 1;
		int bestMoveColor_j = 1;

		int maxMoveScore_nonTabu = Integer.MIN_VALUE;
		int bestMove_v_nonTabu = 1;
		int bestMoveColor_j_nonTabu = 1;

        int moveScore_tmp = 0;

        int random_v = 0;
        int random_c_i = 0;
        int random_c_j = 0;
        int random_moveScore = Integer.MIN_VALUE;
        int conflictDelta = 0;
        int flag = 0;
        while(T > end_T) {
            flag = 0; // 没有接受该解
            // 产生一个随机解 
            // // 产生一个随机值在[1, G.verNum]之间的整数
            random_v = randInt(G.verNum) + 1;
            assert (random_v >= 1 && random_v <= G.verNum);
            random_c_i = randInt(nbColor) + 1;
            assert (random_c_i >= 1 && random_c_i <= nbColor);
            random_c_j = randInt(nbColor) + 1;
            assert (random_c_j >= 1 && random_c_j <= nbColor);
            random_moveScore = this.adj_score[random_v][random_c_i] - this.adj_score[random_v][random_c_j]; 
            conflictDelta = -random_moveScore; 
            // 更好的解
            if (conflictDelta < 0) {
                System.out.println("conflict:"+ conflict_local);
                System.out.println("conflictDelta:" + conflictDelta);
                move(sol, random_v, random_c_i, random_c_j);
                conflict_local += conflictDelta;
                flag = 1;
            } else {
                // 以一定概率接受更差的解
                if (Math.random() < Math.exp(-conflictDelta / T)) {
                    System.out.println("conflict:"+ conflict_local);
                    System.out.println("conflictDelta:" + conflictDelta);
                    move(sol, random_v, random_c_i, random_c_j);
                    conflict_local += conflictDelta;
                    flag = 1;
                } 
            }
            if (calConflict(sol) < calConflict(this.best_sol)) {
                System.arraycopy(sol, 0, this.best_sol, 0, sol.length);
            }
            if (conflict_local <= 0) {
                break;
            }
            T *= alpha;
            assert conflict_local == calConflict(sol);
            System.out.println("con:" + conflict_local);
            System.out.println("2con:" + calConflict(this.best_sol));
            System.out.println("T-"+ T);
        }


        // -----------
        // --------

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
     * @param v        改变的顶点
     * @param c_i      顶点原来的颜色
     * @param c_j      顶点改变后的颜色
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

    /**
     * 找到最大的moveScore及对应的顶点和颜色
     * 
     * @param iter
     * @param left_up_r    查找最大值的范围左上角的行数
     * @param left_up_c    .. 列
     * @param right_down_r 查找最大值的范围右下角的行数
     * @param right_down_c .. 列
     * @return int[] {maxMoveScore, bestMove_v, bestMoveColor_j,
     *         maxMoveScore_nonTabu, bestMove_v_nonTabu, bestMoveColor_j_nonTabu,
     *         flag}
     */
    public int[] findMax(int iter, int left_up_r, int left_up_c, int right_down_r, int right_down_c) {
        int maxMoveScore = Integer.MIN_VALUE;
        int bestMove_v = 0;
        int bestMoveColor_j = 0;
        int flag = 0;

        int maxMoveScore_nonTabu = Integer.MIN_VALUE;
        int bestMoveColor_j_nonTabu = 0;
        int bestMove_v_nonTabu = 0;
        int moveScore_tmp = 0;
        // int max_score = ; // 一次iter 中最大的move-score
        for (int i = left_up_r; i < right_down_r + 1; i += 1) {
            // 若当前点已经无冲突
            if (adj_score[i][sol[i]] == 0) {
                continue;
            }
            for (int j = left_up_c; j < right_down_c + 1; j += 1) {
                // 跳过原地移动，防止无意义的移动
                if (sol[i] == j) {
                    continue;
                }
                moveScore_tmp = adj_score[i][sol[i]] - adj_score[i][j];
                // 记录所有的分数，找出最大的分数
                if ((moveScore_tmp > maxMoveScore) || (moveScore_tmp == maxMoveScore && randInt(10) > 5)) {
                    maxMoveScore = moveScore_tmp;
                    bestMove_v = i;
                    bestMoveColor_j = j;
                    flag = 1;
                }
                if ((moveScore_tmp > maxMoveScore_nonTabu && tabuTable[i][j] < iter)
                        || (moveScore_tmp == maxMoveScore_nonTabu && tabuTable[i][j] < iter && randInt(10) > 5)) {
                    maxMoveScore_nonTabu = moveScore_tmp;
                    bestMove_v_nonTabu = i;
                    bestMoveColor_j_nonTabu = j;
                    flag = 1;
                }
            }
        }
        return new int[] { maxMoveScore, bestMove_v, bestMoveColor_j, maxMoveScore_nonTabu, bestMove_v_nonTabu,
                bestMoveColor_j_nonTabu, flag };
    }

    public int[] merge(int[] dataLs1, int[] dataLs2) {
        int maxMoveScore = Integer.MIN_VALUE;
        int bestMove_v = 0;
        int bestMoveColor_j = 0;
        int maxMoveScore_nonTabu = Integer.MIN_VALUE;
        int bestMoveColor_j_nonTabu = 0;
        int bestMove_v_nonTabu = 0;
        int flag = 0;

        // 比较dataLs1 和 dataLs2
        if (dataLs1[6] == 1 && dataLs2[6] == 1) {
            flag = 1;
            if (dataLs1[0] > dataLs2[0]) {
                maxMoveScore = dataLs1[0];
                bestMove_v = dataLs1[1];
                bestMoveColor_j = dataLs1[2];
            } else {
                maxMoveScore = dataLs2[0];
                bestMove_v = dataLs2[1];
                bestMoveColor_j = dataLs2[2];
            }

            if (dataLs1[3] > dataLs2[3]) {
                maxMoveScore_nonTabu = dataLs1[3];
                bestMove_v_nonTabu = dataLs1[4];
                bestMoveColor_j_nonTabu = dataLs1[5];
            } else {
                maxMoveScore_nonTabu = dataLs2[3];
                bestMove_v_nonTabu = dataLs2[4];
                bestMoveColor_j_nonTabu = dataLs2[5];
            }
        } else if (dataLs1[6] == 0 && dataLs2[6] == 0) {
            flag = 0;
        } else {
            flag = 1;
            if (dataLs1[6] == 1) {
                maxMoveScore = dataLs1[0];
                bestMove_v = dataLs1[1];
                bestMoveColor_j = dataLs1[2];
                maxMoveScore_nonTabu = dataLs1[3];
                bestMove_v_nonTabu = dataLs1[4];
                bestMoveColor_j_nonTabu = dataLs1[5];
            } else {
                maxMoveScore = dataLs2[0];
                bestMove_v = dataLs2[1];
                bestMoveColor_j = dataLs2[2];
                maxMoveScore_nonTabu = dataLs2[3];
                bestMove_v_nonTabu = dataLs2[4];
                bestMoveColor_j_nonTabu = dataLs2[5];
            }
        }
        return new int[] { maxMoveScore, bestMove_v, bestMoveColor_j, maxMoveScore_nonTabu, bestMove_v_nonTabu,
                bestMoveColor_j_nonTabu, flag };
    }

}
