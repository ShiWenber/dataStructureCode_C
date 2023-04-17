package ynu.ls.coloring;

import ynu.ls.coloring.Graph;

import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.SimpleFileVisitor;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
import java.util.Vector;

import javax.management.relation.RelationSupportMBean;
import javax.print.event.PrintEvent;

public class TabuColoring {
	private Graph G;
	// 颜色数
	private int nbColor;
	// 二维表作为禁忌表，第0列表顶点，第1列-第k列表示颜色，值为禁忌次数
	private int[][] tabuTable;
	private int[][] moveScore;

	public TabuColoring(Graph G, int nbColor) {
		this.G = G;
		this.nbColor = nbColor;
		this.tabuTable = new int[G.verNum + 1][nbColor + 1];
		this.moveScore = new int[G.verNum + 1][nbColor + 1]; // moveScore[i][j] 表示顶点i的颜色变为j时的分数，分数越大越好
	}

	/**
	 * 执行禁忌搜索，返回解 sol[i] 表示第 i 个顶点的颜色
	 * 
	 * @return
	 */
	public int[] search() {
		// 初始化随机解
		int sol[] = new int[G.verNum + 1];
		for (int i = 1; i < G.verNum + 1; i++) {
			// 产生从1~nbColor的随机数
			sol[i] = (int) (Math.random() * nbColor) + 1;
		}

		// 初始化最优解，这样记录最优解的时候方便跟禁忌中找到的局部最优解比较，todo:但是比较耗时
		int sol_best[] = sol.clone();

		// 初始化禁忌表和移动评分表
		for (int i = 1; i < G.verNum + 1; i++) {
			for (int j = 1; j < nbColor + 1; j++) {
				tabuTable[i][j] = 0;
				moveScore[i][j] = 0;
			}
		}

		// 初始化移动评分表
		calMoveScore(sol);

		// 解评分，仅计算初始化的一次，后面只做增量式更新
		int conflict = calConflict(sol_best);

		int iter = 1; // 迭代次数
		int max_iter = 10000; // 最大迭代次数
		int randIdx = 1000; // 禁忌表中随机选择的禁忌次数
		// 注意一个iter才更新一次sol，且只有比sol_best好的才更新sol_best
		while (conflict > 0 && iter < max_iter) {
			System.out.println("conflict: " + conflict);
			// // 输出轮数
			System.out.print("---------iter: ");
			System.out.println(iter);
			// choose a best move (v, c_i, c_j) from non-tabu moves
			// 有非禁忌移动就从非禁忌移动中选择一个最好的，否则从禁忌移动中选择第一个move-score为正的
			int flag = 0; // 一次iter 中是否存在非禁忌的移动
			int break_to_while = 0; // 是否跳出for循环
			// 记录不在禁忌期的最优移动
			int maxMoveScore = moveScore[1][1]; // 一次iter 中最大的move-score
			int bestMove_v = 1;
			int bestMoveColor_j = 1;
			if (tabuTable[1][1] < iter) {
				flag = 1;
			}

			// int max_score = ; // 一次iter 中最大的move-score
			for (int i = 1; i < G.verNum + 1; i++) {
				for (int j = 1; j < nbColor + 1; j++) {
					// 如果非禁忌的移动没有score为正的，就从禁忌的移动中选择最先找到的一个能score为正的移动
					if (tabuTable[i][j] >= iter && moveScore[i][j] > 0) {

						int conflictDelta = -moveScore[i][j];
						assert (sol[i] == j);
						// System.out.print("conflict: ");
						// System.out.println(calConflict(sol));
						tabuTable[i][j] = iter + conflict + randInt(randIdx);
						if (conflictDelta < 0) {
							move(sol, i, sol[i], j);
							sol_best = sol.clone();
							assert (calConflict(sol) == conflict + conflictDelta);
							conflict += conflictDelta;
						}
						// printTabu(tabuTable);
						// printMoveScore(moveScore);
						iter++;
						break_to_while = 1;
						flag = 0;
						break; // 跳出到while循环
					} else {
						// 如果是没有禁忌的移动或者是禁忌的移动但是不能使解更好，那么选择一个最佳的移动
						// 记录所有的分数，找出最大的分数
						if (moveScore[i][j] > maxMoveScore && tabuTable[i][j] < iter) {
							maxMoveScore = moveScore[i][j];
							bestMove_v = i;
							bestMoveColor_j = j;
							flag = 1;
						}
						// 存在一种情况flag为0，maxMoveScore为moveScore[1][1]，但是这个移动是禁忌的，而且不存在非禁忌且比moveScore[1][1]更好的移动
					}
				}
				if (break_to_while == 1) {
					break;
				}
			}
			// 不存在非禁忌的移动，找到一个最优的移动
			if (flag == 1) {
				int conflictDelta = - moveScore[bestMove_v][bestMoveColor_j];
				move(sol, bestMove_v, sol[bestMove_v], bestMoveColor_j);
				assert (sol[bestMove_v] == bestMoveColor_j);
				// System.out.print("conlict: ");
				// System.out.println(calConflict(sol));
				tabuTable[bestMove_v][bestMoveColor_j] = iter + conflict + randInt(randIdx);
				sol_best = sol.clone();
				assert (calConflict(sol) == conflict + conflictDelta);
				conflict += conflictDelta;
				// printTabu(tabuTable);
				// printMoveScore(moveScore);
				iter++;
			} else {
				// 存在一种情况flag为0，maxMoveScore为moveScore[1][1]，但是这个移动是禁忌的，而且不存在非禁忌且比moveScore[1][1]更好的移动
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
		// System.out.println("conflict: " + conflict);
		// }
		// System.out.println(iter);
		// System.out.println(calConflict(sol_best));
		assert (calConflict(sol_best) == 0);
		return sol_best;
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

	/**
	 * 计算移动评分表
	 * 
	 * @param sol
	 */
	public void calMoveScore(int[] sol) {
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
				moveScore[i][color] = conflict_neighbor_origin - conflict_neighbor_new;
			}
		}
	}

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

		// int conflict_neighbor_new = 0;
		// int conflict_neighbor_origin = 0;

		// ArrayList<Integer> neighbors = G.getNeighbors(v);
		// for (int u : neighbors) {
		// 	for (int c = 1; c < nbColor + 1; c++) {
		// 		conflict_neighbor_new = 0;
		// 		conflict_neighbor_origin = 0;
		// 		if (c == sol[u]) {
		// 			moveScore[u][c] = 0;
		// 			continue;
		// 		}
		// 		for (int w : neighbors) {
		// 			// 现在对邻居的颜色冲突数
		// 			if (sol[u] == sol[w]) {
		// 				conflict_neighbor_origin++;
		// 			}
		// 			// 如果变化为c，对邻居的颜色冲突数
		// 			if (c == sol[w]) {
		// 				conflict_neighbor_new++;
		// 			}
		// 		}
		// 		moveScore[u][c] = conflict_neighbor_origin - conflict_neighbor_new;
		// 	}
		// }
		// // 更新v的moveScore
		// for (int c = 1; c < nbColor + 1; c++) {
		// 	conflict_neighbor_new = 0;
		// 	conflict_neighbor_origin = 0;
		// 	if (c == c_j) {
		// 		moveScore[v][c] = 0;
		// 		continue;
		// 	}
		// 	for (int u : neighbors) {
		// 		// 现在对邻居的颜色冲突数
		// 		if (c_j == sol[u]) {
		// 			conflict_neighbor_origin++;
		// 		}
		// 		// 如果变化为c，对邻居的颜色冲突数
		// 		if (c == sol[u]) {
		// 			conflict_neighbor_new++;
		// 		}
		// 	}
		// 	moveScore[v][c] = conflict_neighbor_origin - conflict_neighbor_new;
		// }

		calMoveScore(sol); // 暴力法保证准确率，但是不如上面的方法快 TODO 上面的方法错误
	}
}
