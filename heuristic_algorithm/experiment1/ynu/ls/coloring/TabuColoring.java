package ynu.ls.coloring;

import ynu.ls.coloring.Graph;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.SimpleFileVisitor;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;
import java.util.Vector;
import java.util.concurrent.CompletableFuture;
import java.util.stream.IntStream;

import javax.lang.model.util.ElementScanner6;
import javax.management.relation.RelationSupportMBean;
import javax.print.event.PrintEvent;

public class TabuColoring {
	private Graph G;
	// 颜色数
	private int nbColor;
	// 二维表作为禁忌表，第0列表顶点，第1列-第k列表示颜色，值为禁忌次数
	private int[][] tabuTable;
	private int[][] moveScore;
	private int[][] moveScore2;
	private int[] best_sol;
	private int[][] adj_score;
	private int iter;
	private int conflict;

	public TabuColoring(Graph G, int nbColor) {
		this.G = G;
		this.nbColor = nbColor;
		this.tabuTable = new int[G.verNum + 1][nbColor + 1];
		this.moveScore = new int[G.verNum + 1][nbColor + 1]; // moveScore[i][j] 表示顶点i的颜色变为j时的分数，分数越大越好
		this.moveScore2 = new int[G.verNum + 1][nbColor + 1]; // moveScore2[i][j] 表示顶点i的颜色变为j时的分数，分数越大越好
		this.best_sol = new int[G.verNum + 1];
		this.adj_score = new int[G.verNum + 1][nbColor + 1];
		this.iter = 1;
		IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
			// 产生从1~nbColor的随机数
			best_sol[i] = (int) (Math.random() * nbColor) + 1;
		});

		// 初始化最优解，这样记录最优解的时候方便跟禁忌中找到的局部最优解比较，todo:但是比较耗时
		// int sol_best[] = sol.clone(); // 是深拷贝？ TODO
		// int sol_best[] = Arrays.copyOf(sol, sol.length);

		// stream.parallel() 并行流加速
		IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
			IntStream.range(1, nbColor + 1).parallel().forEach(j -> {
				tabuTable[i][j] = 0;
				moveScore[i][j] = 0;
				adj_score[i][j] = 0;
			});
		});

		// 初始化adj_score
		// calAdjScore(best_sol);

		// // 初始化移动评分表
		// calMoveScore(best_sol, moveScore);
		// // calMoveScore(best_sol, moveScore2);

		// // 解评分，仅计算初始化的一次，后面只做增量式更新
		// int conflict2 = calConflict(best_sol);

		// 并行加速
		CompletableFuture<Void> task1 = CompletableFuture.runAsync(() -> calAdjScore(best_sol));
		CompletableFuture<Void> task2 = CompletableFuture.runAsync(() -> calMoveScore(best_sol, moveScore));
		CompletableFuture<Integer> task3 = CompletableFuture.supplyAsync(() -> calConflict(best_sol));

		CompletableFuture<Void> allTasks = CompletableFuture.allOf(task1, task2, task3);
		allTasks.join();

		this.conflict = 0;

		try {
			// 获取calConflict的返回值
			this.conflict = task3.get();
		} catch (Exception e) {
			System.out.println(e);
		}
	}

	/**
	 * 执行禁忌搜索，返回解 sol[i] 表示第 i 个顶点的颜色
	 * 
	 * @return
	 */
	public int[] search() {
		// 开始记时
		long startTime = System.currentTimeMillis();
		// 初始化随机解
		// for (int i = 1; i < G.verNum + 1; i++) {
		// // 产生从1~nbColor的随机数
		// best_sol[i] = (int) (Math.random() * nbColor) + 1;
		// }
		// 随机挑选5%的节点，随机改变颜色
		Random random = new Random();
		Set<Integer> set = new HashSet<>();
		while (set.size() < G.verNum * 0.05) {
			set.add(random.nextInt(G.verNum) + 1);
		}
		int[] idx = set.stream().mapToInt(Integer::valueOf).toArray();
		// 随机改变颜色
		for (int i = 0; i < idx.length; i++) {
			best_sol[idx[i]] = (int) (Math.random() * nbColor) + 1;
		}

		// stream.parallel() 并行流加速
		IntStream.range(1, G.verNum + 1).parallel().forEach(i -> {
			IntStream.range(1, nbColor + 1).parallel().forEach(j -> {
				tabuTable[i][j] = 0;
				moveScore[i][j] = 0;
				adj_score[i][j] = 0;
			});
		});

		// 初始化adj_score
		// calAdjScore(best_sol);

		// // 初始化移动评分表
		// calMoveScore(best_sol, moveScore);
		// // calMoveScore(best_sol, moveScore2);

		// // 解评分，仅计算初始化的一次，后面只做增量式更新
		// int conflict2 = calConflict(best_sol);

		// 并行加速
		CompletableFuture<Void> task1 = CompletableFuture.runAsync(() -> calAdjScore(best_sol));
		CompletableFuture<Void> task2 = CompletableFuture.runAsync(() -> calMoveScore(best_sol, moveScore));
		CompletableFuture<Integer> task3 = CompletableFuture.supplyAsync(() -> calConflict(best_sol));

		CompletableFuture<Void> allTasks = CompletableFuture.allOf(task1, task2, task3);
		allTasks.join();

		this.conflict = 0;

		try {
			// 获取calConflict的返回值
			this.conflict = task3.get();
		} catch (Exception e) {
			System.out.println(e);
		}

		// assert (conflict == conflict2);

		this.iter = 1; // 迭代次数
		int max_iter = 60000; // 最大迭代次数
		// int randIdx = 500; // 禁忌表中随机选择的禁忌次数
		int randIdx = 4000;
		int sm_randIdx = 1000; // 禁忌表中随机选择的禁忌次数
		// 注意一个iter才更新一次sol，且只有比sol_best好的才更新sol_best
		// int x = 5;
		int x = randInt(10);

		while (conflict > 0 && iter < max_iter) {
			int tt = iter + conflict * x + randInt(randIdx);
			// int tt = iter + (int) (1.0 / conflict * randIdx * 10) + randInt(sm_randIdx);
			// int tt2 = iter + conflict * 10 + randInt(randIdx);
			System.out.println("conflict: " + conflict);
			// // 输出轮数
			System.out.print("---------iter: ");
			System.out.println(iter);
			// choose a best move (v, c_i, c_j) from non-tabu moves
			// 有非禁忌移动就从非禁忌移动中选择一个最好的，否则从禁忌移动中选择第一个move-score为正的
			int flag = 0; // 一次iter 中是否存在非禁忌的移动
			// 记录在禁忌期的最优移动
			// int maxMoveScore = moveScore[1][1]; // 一次iter 中最大的move-score
			// 初始化为最小值
			int maxMoveScore = Integer.MIN_VALUE;
			int bestMove_v = 1;
			int bestMoveColor_j = 1;
			flag = 0;
			// if (tabuTable[1][1] >= iter) {
			// flag = 1;
			// }
			// 记录非禁忌的最优移动
			int maxMoveScore_nonTabu = Integer.MIN_VALUE;
			int bestMove_v_nonTabu = 1;
			int bestMoveColor_j_nonTabu = 1;
			// int step = 2;
			int step = 1;
			// start 是 1, 2 中的随机数
			// int start = randInt(2) + 1;
			int start = 1;

			// 获取movescore 的最大值，以及最大值的位置

			// int max_score = ; // 一次iter 中最大的move-score
			// for (int i = start; i < G.verNum + 1; i += step) {
			// for (int j = start; j < nbColor + 1; j += step) {
			// // 记录所有的分数，找出最大的分数
			// if ((moveScore[i][j] > maxMoveScore) || (moveScore[i][j] == maxMoveScore &&
			// randInt(10) > 5)) {
			// maxMoveScore = moveScore[i][j];
			// bestMove_v = i;
			// bestMoveColor_j = j;
			// flag = 1;
			// }
			// if ((moveScore[i][j] > maxMoveScore_nonTabu && tabuTable[i][j] < iter)
			// || (moveScore[i][j] == maxMoveScore_nonTabu && tabuTable[i][j] < iter &&
			// randInt(10) > 5)) {
			// maxMoveScore_nonTabu = moveScore[i][j];
			// bestMove_v_nonTabu = i;
			// bestMoveColor_j_nonTabu = j;
			// flag = 1;
			// }
			// }
			// }
			// // 并行加速 将二维矩阵四等分并行---------v
			// CompletableFuture<int[]> task_max1 = CompletableFuture
			// .supplyAsync(() -> findMax(iter, 1, 1, G.verNum / 2, nbColor / 2));
			// CompletableFuture<int[]> task_max2 = CompletableFuture
			// .supplyAsync(() -> findMax(iter, 1, nbColor / 2, G.verNum / 2, nbColor));
			// CompletableFuture<int[]> task_max3 = CompletableFuture
			// .supplyAsync(() -> findMax(iter, G.verNum / 2, 1, G.verNum, nbColor / 2));
			// CompletableFuture<int[]> task_max4 = CompletableFuture.supplyAsync(
			// () -> findMax(iter, G.verNum / 2, nbColor / 2, G.verNum, nbColor));
			// CompletableFuture<Void> allMaxTasks = CompletableFuture.allOf(task_max1,
			// task_max2, task_max3, task_max4);
			// allMaxTasks.join();

			// int[] dataLs1 = null;
			// int[] dataLs2 = null;
			// int[] dataLs3 = null;
			// int[] dataLs4 = null;
			// try {
			// // 获取最佳移动的返回值
			// dataLs1 = task_max1.get();
			// dataLs2 = task_max2.get();
			// dataLs3 = task_max3.get();
			// dataLs4 = task_max4.get();
			// } catch (Exception e) {
			// // System.out.println(e);
			// }

			// int[] dataLs1_2 = merge(dataLs1, dataLs2);
			// int[] dataLs3_4 = merge(dataLs3, dataLs4);
			// dataLs1_2 = merge(dataLs1_2, dataLs3_4);
			// -----------------^

			// 并行加速 将二维矩阵二等分并行 实验发现四线程和两线程的速度差别不大 相比单线程确有加速
			// 迭代次数过多，每次开启线程的消耗不小，如果允许，应当在search函数层面实现并行，这样每个线程存在的时间够长不会在创建线程上消耗过多资源
			// ------------v
			CompletableFuture<int[]> task_max1 = CompletableFuture
					.supplyAsync(() -> findMax(iter, 1, 1, G.verNum / 2, nbColor));
			CompletableFuture<int[]> task_max2 = CompletableFuture
					.supplyAsync(() -> findMax(iter, G.verNum / 2 + 1, 1, G.verNum, nbColor));
			CompletableFuture<Void> allMaxTasks = CompletableFuture.allOf(task_max1, task_max2);
			allMaxTasks.join();

			int[] dataLs1 = null;
			int[] dataLs2 = null;
			// int[] dataLs3 = null;
			// int[] dataLs4 = null;
			try {
				// 获取calConflict的返回值
				dataLs1 = task_max1.get();
				dataLs2 = task_max2.get();
				// dataLs3 = task_max3.get();
				// dataLs4 = task_max4.get();
			} catch (Exception e) {
				System.out.println(e);
			}

			int[] dataLs1_2 = merge(dataLs1, dataLs2);
			// ---------------^

			// ------------多线程使用，单线程应当注释
			maxMoveScore = dataLs1_2[0];
			bestMove_v = dataLs1_2[1];
			bestMoveColor_j = dataLs1_2[2];
			maxMoveScore_nonTabu = dataLs1_2[3];
			bestMove_v_nonTabu = dataLs1_2[4];
			bestMoveColor_j_nonTabu = dataLs1_2[5];
			flag = dataLs1_2[6];
			// -----------

			// // 单线程 -------v
			// // int max_score = ; // 一次iter 中最大的move-score
			// for (int i = start; i < G.verNum + 1; i += step) {
			// for (int j = start; j < nbColor + 1; j += step) {
			// // 记录所有的分数，找出最大的分数
			// if (moveScore[i][j] > maxMoveScore) {
			// maxMoveScore = moveScore[i][j];
			// bestMove_v = i;
			// bestMoveColor_j = j;
			// flag = 1;
			// } else if (moveScore[i][j] == maxMoveScore && randInt(10) > 5) {
			// // 遇到相同的分数，随机选择一个
			// maxMoveScore = moveScore[i][j];
			// bestMove_v = i;
			// bestMoveColor_j = j;
			// flag = 1;
			// }
			// if (moveScore[i][j] > maxMoveScore_nonTabu && tabuTable[i][j] < iter) {
			// maxMoveScore_nonTabu = moveScore[i][j];
			// bestMove_v_nonTabu = i;
			// bestMoveColor_j_nonTabu = j;
			// flag = 1;
			// } else if (moveScore[i][j] == maxMoveScore_nonTabu && tabuTable[i][j] < iter
			// && randInt(10) > 5) {
			// // 遇到相同的分数，随机选择一个
			// maxMoveScore_nonTabu = moveScore[i][j];
			// bestMove_v_nonTabu = i;
			// bestMoveColor_j_nonTabu = j;
			// flag = 1;
			// }
			// }
			// }
			// // -----------^

			// 非禁忌的移动中，找到一个最优的移动
			if (flag == 1) {
				int old_color = best_sol[bestMove_v];
				int old_color_nonTabu = best_sol[bestMove_v_nonTabu];
				// 最佳解不禁忌
				if (tabuTable[bestMove_v][bestMoveColor_j] < iter) {
					int conflictDelta = -moveScore[bestMove_v][bestMoveColor_j];
					System.out.println("bestMove: " + bestMove_v + " " + bestMoveColor_j + " " + conflictDelta);
					move(best_sol, bestMove_v, best_sol[bestMove_v], bestMoveColor_j);
					assert (best_sol[bestMove_v] == bestMoveColor_j);
					// System.out.print("conlict: ");
					// System.out.println(calConflict(sol));
					// 禁回头
					tabuTable[bestMove_v][old_color] = tt;
					// // 禁已经采取过的动作
					// tabuTable[bestMove_v][bestMoveColor_j] = tt2;

					assert (calConflict(best_sol) == conflict + conflictDelta);
					conflict += conflictDelta;
					// printTabu(tabuTable);
					// printMoveScore(moveScore);
					iter++;
				} else if (moveScore[bestMove_v][bestMoveColor_j] > 0) {
					// 最佳解禁忌但是move-score为正
					int conflictDelta = -moveScore[bestMove_v][bestMoveColor_j];
					System.out.println(
							"bestMove_moveScore>0: " + bestMove_v + " " + bestMoveColor_j + " " + conflictDelta);
					move(best_sol, bestMove_v, best_sol[bestMove_v], bestMoveColor_j);
					assert (best_sol[bestMove_v] == bestMoveColor_j);
					// System.out.print("conlict: ");
					// System.out.println(calConflict(sol));
					// 禁回头
					tabuTable[bestMove_v][old_color] = tt;
					// // 禁已经采取过的动作
					// tabuTable[bestMove_v][bestMoveColor_j] = tt2;

					assert (calConflict(best_sol) == conflict + conflictDelta);
					conflict += conflictDelta;
					// printTabu(tabuTable);
					// printMoveScore(moveScore);
					iter++;
				} else {
					// 最佳解禁忌且move-score为负，则从非禁忌的移动中选择一个最优的移动
					int conflictDelta = -maxMoveScore_nonTabu;
					System.out.println("bestMove_nonTabu: " + bestMove_v_nonTabu + " " + bestMoveColor_j_nonTabu + " "
							+ conflictDelta);
					move(best_sol, bestMove_v_nonTabu, best_sol[bestMove_v_nonTabu], bestMoveColor_j_nonTabu);
					assert (best_sol[bestMove_v_nonTabu] == bestMoveColor_j_nonTabu);
					// 禁回头
					tabuTable[bestMove_v_nonTabu][old_color_nonTabu] = tt;
					System.out.println("tabuTable: " + bestMove_v_nonTabu + " " + old_color_nonTabu + " " + tt);
					// // 禁已经采取过的动作
					// tabuTable[bestMove_v_nonTabu][bestMoveColor_j] = tt2;

					assert (calConflict(best_sol) == conflict + conflictDelta);
					conflict += conflictDelta;
					iter++;
				}
			} else {
				// 存在一种情况flag为0，maxMoveScore为moveScore[1][1]，但是这个移动是非禁忌的，而且不存在禁忌且比moveScore[1][1]更好的移动
				// 那么不做任何动作，直接进入下一次迭代
				iter++;
			}

			// if (temp_iter == iter) {
			// printMoveScore(moveScore);
			// printMoveScore(tabuTable);
			// }
		}
		// if (conflict == 1) {
		// printMoveScore(moveScore);
		// printMoveScore(tabuTable);
		// // 通过输入暂时阻塞在此处
		// Scanner sc = new Scanner(System.in);
		// sc.nextLine();
		// System.out.println("con: " + conflict);
		// }
		// System.out.println(iter);
		// System.out.println(calConflict(sol_best));
		// assert (calConflict(best_sol) == 0);
		System.out.println("con:" +
				calConflict(best_sol));
		System.out.println("iter-" + iter);
		// 结束记时并输出时间
		long endTime = System.currentTimeMillis();
		long runTime = endTime - startTime;
		System.out.println("runTime: " + runTime);
		return best_sol;
	}

	private void printTabu(int[][] tabuTable) {
		for (int i = 1; i < G.verNum + 1; i++) {
			for (int j = 1; j < nbColor + 1; j++) {
				System.out.print(tabuTable[i][j]);
				System.out.print(",");
			}
			System.out.println("");
		}
		System.out.println("---------------------");
	}

	private void printMoveScore(int[][] moveScore) {
		for (int i = 1; i < G.verNum + 1; i++) {
			for (int j = 1; j < nbColor + 1; j++) {
				System.out.print(moveScore[i][j]);
				System.out.print(",");
			}
			System.out.println();
		}
		System.out.println("---------------------");
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

	/**
	 * 计算移动评分表
	 * 
	 * @param sol
	 */
	public void calMoveScore(int[] sol, int[][] moveScore2) {
		int conflict_neighbor_origin = 0;
		int conflict_neighbor_new = 0;
		for (int i = 1; i < G.verNum + 1; i++) {
			for (int color = 1; color < nbColor + 1; color++) {
				conflict_neighbor_new = 0;
				conflict_neighbor_origin = 0;
				// 计算原有冲突数
				ArrayList<Integer> neighbors = G.getNeighbors(i);
				for (int v : neighbors) {
					if (sol[v] == sol[i]) {
						conflict_neighbor_origin++;
					}
					if (sol[v] == color) {
						conflict_neighbor_new++;
					}
				}
				moveScore2[i][color] = conflict_neighbor_origin - conflict_neighbor_new;
			}
		}
	}

	/**
	 * 移动顶点并更新moveScore,sol表
	 * 
	 * @param best_sol 当前解
	 * @param v        改变的顶点
	 * @param c_i      顶点原来的颜色
	 * @param c_j      顶点改变后的颜色
	 * @return 返回冲突数的变化
	 */
	public void move(int[] best_sol, int v, int c_i, int c_j) {
		// 更新范围：v和v的邻居
		// 更新邻居的moveScore
		best_sol[v] = c_j;

		// // 更新 adj_score
		ArrayList<Integer> neighbors = G.getNeighbors(v);
		for (int u : neighbors) {
			this.adj_score[u][c_i]--;
			this.adj_score[u][c_j]++;
		}
		// 更新moveScore
		for (int c = 1; c < nbColor + 1; c++) {
			moveScore[v][c] = adj_score[v][best_sol[v]] - adj_score[v][c];
		}
		for (int u : neighbors) {
			for (int color = 1; color < nbColor + 1; color++) {
				moveScore[u][color] = adj_score[u][best_sol[u]] - adj_score[u][color];
			}
		}

		// 这里内部是小循环使用并行反而会降低效率，这里不管是用并行还是普通stream foreach都无法实现加速
		// ArrayList<Integer> neighbors = G.getNeighbors(v);
		// neighbors.parallelStream().forEach(u -> {
		// synchronized (this) {
		// this.adj_score[u][c_i]--;
		// this.adj_score[u][c_j]++;
		// }
		// });
		// // 更新moveScore
		// IntStream.rangeClosed(1, nbColor)
		// .parallel()
		// .forEach(c -> moveScore[v][c] = adj_score[v][best_sol[v]] - adj_score[v][c]);
		// neighbors.parallelStream().forEach(u -> {
		// IntStream.rangeClosed(1, nbColor)
		// .parallel()
		// .forEach(color -> moveScore[u][color] = adj_score[u][best_sol[u]] -
		// adj_score[u][color]);
		// });

		// calMoveScore(best_sol, moveScore2); // 暴力法保证准确率，但是不如上面的方法快 TODO 上面的方法错误
		// assert (equal_array(moveScore, moveScore2));

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

		// int max_score = ; // 一次iter 中最大的move-score
		for (int i = left_up_r; i < right_down_r + 1; i += 1) {
			for (int j = left_up_c; j < right_down_c + 1; j += 1) {
				// 记录所有的分数，找出最大的分数
				if ((moveScore[i][j] > maxMoveScore) || (moveScore[i][j] == maxMoveScore && randInt(10) > 5)) {
					maxMoveScore = moveScore[i][j];
					bestMove_v = i;
					bestMoveColor_j = j;
					flag = 1;
				}
				if ((moveScore[i][j] > maxMoveScore_nonTabu && tabuTable[i][j] < iter)
						|| (moveScore[i][j] == maxMoveScore_nonTabu && tabuTable[i][j] < iter && randInt(10) > 5)) {
					maxMoveScore_nonTabu = moveScore[i][j];
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