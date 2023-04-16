package ynu.ls.coloring;

import ynu.ls.coloring.Graph;
import java.util.ArrayList;
import java.util.Vector;

import javax.management.relation.RelationSupportMBean;
import javax.print.event.PrintEvent;

public class TabuColoring {
	private Graph G;
	// 颜色数
	private int nbColor;
	// 二维表作为禁忌表，第0列表顶点，第1列-第k列表示颜色，值为禁忌次数
	private int[][] tabuTable;
	// 邻接状态表
	private int[][] adjStatus;
	private int[][] moveScore;

	public TabuColoring(Graph G, int nbColor) {
		this.G = G;
		this.nbColor = nbColor;
		this.tabuTable = new int[G.verNum+1][nbColor];
		this.adjStatus = new int[G.verNum+1][nbColor];
		this.moveScore = new int[G.verNum+1][nbColor]; // moveScore[i][j] 表示顶点i的颜色变为j时的分数，分数越大越好
	}

	/**
	 * 执行禁忌搜索，返回解 sol[i] 表示第 i 个顶点的颜色
	 * 
	 * @return
	 */
	public int[] search() {
		// 初始化随机解
		int sol[] = new int[G.verNum+1];
		for (int i = 0; i < G.verNum+1; i++) {
			sol[i] = (int) (Math.random() * nbColor);
		}

		// 初始化最优解，这样记录最优解的时候方便跟禁忌中找到的局部最优解比较，todo:但是比较耗时
		int sol_best[] = sol.clone();

		// 初始化禁忌表和移动评分表
		for (int i = 0; i < G.verNum+1; i++) {
			for (int j = 0; j < nbColor; j++) {
				tabuTable[i][j] = 0;
				moveScore[i][j] = 0;
			}
		}

		// // 初始化邻接状态表
		// for (int i = 0; i < G.verNum+1; i++) {
		// 	for (int j = 0; j < nbColor; j++) {
		// 		adjStatus[i][j] = 0;
		// 	}
		// }
		// for (int i = 0; i < G.verNum+1; i++) {
		// 	ArrayList<Integer> neighbors = G.getNeighbors(i);
		// 	// todo
		// 	// neighbors.stream().forEach(v -> adjStatus[i][sol[v]]++);
		// 	for (int v : neighbors) {
		// 		adjStatus[i][sol[v]]++;
		// 	}
		// }

		// 初始化移动评分表
		calMoveScore(sol);

		// 解评分，仅计算初始化的一次，后面只做增量式更新
		int conflict = calConflict(sol);

		int iter = 1; // 迭代次数
		int max_iter = 1000; // 最大迭代次数
		// 注意一个iter才更新一次sol，且只有比sol_best好的才更新sol_best
		while (conflict > 0 && iter < max_iter) {
			//  choose a best move (v, c_i, c_j) from non-tabu moves
			//  有非禁忌移动就从非禁忌移动中选择一个最好的，否则从禁忌移动中选择第一个move-score为正的
			int flag = 0; // 一次iter 中是否存在非禁忌的移动
			int break_to_while = 0; // 是否跳出for循环
			// 记录不在禁忌期的最优移动
			int maxMoveScore = moveScore[0][0]; // 一次iter 中最大的move-score
			int bestMove_v = 0;
			int bestMoveColor_j = 0;
			if (tabuTable[0][0] < iter) {
				flag = 1;
			}

			// int max_score = ; // 一次iter 中最大的move-score
			for (int i = 0; i < G.verNum+1; i++) {
				for (int j = 0; j < nbColor; j++) {
					// 如果非禁忌的移动没有score为正的，就从禁忌的移动中选择最先找到的一个能score为正的移动
					if (tabuTable[i][j] >= iter && moveScore[i][j] > 0) {
						move(sol, i, sol[i], j);
						tabuTable[i][j] = iter + conflict + randInt(7);
						if (calConflict(sol) < calConflict(sol_best)) {
							sol_best = sol.clone();
						}
						iter++;
						break_to_while = 1;	
						flag = 0;
						break; // 跳出到while循环
					} else {
						// 如果是没有禁忌的移动或者是禁忌的移动但是不能使解更好，那么选择一个最佳的移动
						// 记录所有的分数，找出最大的分数
						if (moveScore[i][j] > maxMoveScore) {
							maxMoveScore = moveScore[i][j];
							bestMove_v = i;
							bestMoveColor_j = j;
							flag = 1;
						}
					}
				}
				if (break_to_while == 1) {
					break;
				}
			}
			// 不存在非禁忌的移动，找到一个最优的移动
			if (flag == 1) {
				move(sol, bestMove_v, sol[bestMove_v], bestMoveColor_j);
				tabuTable[bestMove_v][bestMoveColor_j] = iter + conflict + randInt(7);
				if (calConflict(sol) < calConflict(sol_best)) {
					sol_best = sol.clone();
				}
				iter++;
			}
		}
		return sol_best;
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
		for (int i = 0; i < G.verNum+1; i++) {
			ArrayList<Integer> neighbors = G.getNeighbors(i);
			// todo
			for (int v : neighbors) {
				if (sol[v] == sol[i]) {
					res++;
				}
			}
		}
		return res;
	}

	/**
	 * 计算移动评分表
	 * 
	 * @param sol
	 */
	public void calMoveScore(int[] sol) {
		for (int i = 0; i < G.verNum+1; i++) {
			for (int color = 0; color < nbColor; color++) {
				moveScore[i][color] = 0;
				ArrayList<Integer> neighbors = G.getNeighbors(i);
				// neighbors.stream().forEach(v -> {
				// if (sol[v] == color) {
				// moveScore[i][color]--;
				// }
				// });
				for (int v : neighbors) {
					if (sol[v] == color) {
                       
						moveScore[i][color]--;
					}
				}
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
	 */
	public void move(int[] sol, int v, int c_i, int c_j) {
		sol[v] = c_j;
		ArrayList<Integer> neighbors = G.getNeighbors(v);
		neighbors.stream().forEach(u -> {
			if (sol[u] == c_i) {
				moveScore[u][c_i]--;
				moveScore[u][c_j]++;
			}
		});
	}
}
