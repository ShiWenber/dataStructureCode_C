package ynu.ls.coloring;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Random;


public class TabuColoring {

    int Size;
    int colorNum;
    ArrayList<int[]> allList = new ArrayList<int[]>();
    Random r = new Random();


    // 调用g.getNeighbors(i);方法，获得边eage和allList
    public TabuColoring(Graph g, int nbColor) {
        this.Size = g.verNum;
        this.colorNum = nbColor;
        ArrayList<Integer> adjList = new ArrayList<>();
        int[][] edge = new int[Size][Size];
        for (int i = 0; i <= Size; i++) {
            adjList = g.getNeighbors(i);
            int[] array = new int[adjList.size()];
            boolean result = adjList.isEmpty();
            if (!result && i != 0) {
                for (int integer = 0; integer < adjList.size(); integer++) {
                    edge[i - 1][adjList.get(integer) - 1] = 1;
                    array[integer] = adjList.get(integer) - 1;
                }
                allList.add(array);
            }
        }
    }
    // 初始化vex，获得一个初始解
    public int[] initiaWork(int nbColor) {
        int[] vex = new int[Size];
        for (int i = 0; i < Size; i++) {
            vex[i] = r.nextInt(nbColor);
        }
        return vex;

    }
    // 初始化冲突值
    public int calculateConflict(List<int[]> adjList, int[] graphColor) {
        int conflict = 0;
        for (int i = 0; i < adjList.size(); i++) {
            for (int j = 0; j < adjList.get(i).length; j++) {
                if (graphColor[i] == graphColor[adjList.get(i)[j]]) {
                    conflict++;
                }
            }
        }
        return conflict / 2;

    }
    // 计算每一个点相邻的点的颜色数量，构成二阶矩阵vertexColor，size代表节点数量，colornum代表颜色数量
    public int[][] getVertexColor(List<int[]> adjList, int[] graphColor) {
        int[][] vertexColor = new int[Size][colorNum];
        for (int i = 0; i < adjList.size(); i++) {
            for (int j = 0; j < adjList.get(i).length; j++) {
                int color = graphColor[adjList.get(i)[j]];
                vertexColor[i][color]++;
            }
        }
        return vertexColor;
    }
    //使用hashmap提高运行效率
    public HashMap<Integer, ArrayList<Integer>> getColorVertex(int[] graphColor) {

        HashMap<Integer, ArrayList<Integer>> colorVertex = new HashMap<Integer, ArrayList<Integer>>();
        for (int i = 0; i < graphColor.length; i++) {
            if (colorVertex.containsKey(graphColor[i])) {
                colorVertex.get(graphColor[i]).add(i);
            } else {
                colorVertex.put(graphColor[i], new ArrayList<Integer>());
                colorVertex.get(graphColor[i]).add(i);
            }
        }
        return colorVertex;
    }

    public int[] search() {

        int iter = 1;
        // 最大迭代次数
        int maxIter = 100000000;
        // 获得初始解
        int[] graphColor = initiaWork(colorNum);

        int[] bestSolve = graphColor.clone();

        int[][] vertexNeighbourColor = getVertexColor(allList, graphColor);

        int[][] tabuList = new int[Size][colorNum];

        int conflictNum = calculateConflict(allList, graphColor);

        int bestConflictNum = conflictNum;

        int vertexNum = 0;
        int colorNum = 0;

        int maxScore = -9999;

        List<Integer> bestPoint = new ArrayList<Integer>();

        List<Integer> bestColor = new ArrayList<Integer>();

        List<Integer> bestvetrx = new ArrayList<Integer>();

        do {
            bestPoint.clear();
            bestColor.clear();
            vertexNum = 0;
            colorNum = 0;
            maxScore = -999;

            for (int i = 0; i < vertexNeighbourColor.length; i++) {
                // 若当前点已经无冲突
                if (vertexNeighbourColor[i][graphColor[i]] == 0) {
                    continue;
                }

                for (int i1 = 0; i1 < vertexNeighbourColor[i].length; i1++) {

                    if (graphColor[i] == i1) {
                        continue;
                    } else {
                        int currentScore = vertexNeighbourColor[i][graphColor[i]] - vertexNeighbourColor[i][i1];

                        // 维护分数表，并寻找最高分数，并将最高分数的点bestPoint.add(i);

                        if (currentScore == maxScore && tabuList[i][i1] < iter) {
                            bestPoint.add(i);
                            bestColor.add(i1);
                        }

                        if (currentScore > maxScore) {

                            double p=r.nextDouble();
                            if (conflictNum - currentScore < bestConflictNum) {
                                tabuList[vertexNum][colorNum] = iter;
                                maxScore = currentScore;

                                bestPoint.clear();
                                bestColor.clear();

                                bestPoint.add(i);
                                bestColor.add(i1);

                            }

                            else if (conflictNum - currentScore >= bestConflictNum && tabuList[i][i1] < iter) {
                                maxScore = currentScore;

                                bestPoint.clear();
                                bestColor.clear();

                                bestPoint.add(i);
                                bestColor.add(i1);
                            }


                        }
                    }
                }
            }

            // 将顶点对应的颜色更新
            conflictNum = conflictNum - maxScore;

            int idx = r.nextInt(bestPoint.size());

            vertexNum = bestPoint.get(idx);

            colorNum = bestColor.get(idx);

            double p=r.nextDouble();

            if (conflictNum < bestConflictNum) {
                tabuList[vertexNum][colorNum] = iter + conflictNum + r.nextInt(87);
                bestConflictNum = conflictNum;
                bestSolve = graphColor.clone();
            } else {
                tabuList[vertexNum][colorNum] = iter + conflictNum + r.nextInt(87);
            }

            for (int i = 0; i < allList.get(vertexNum).length; i++) {
                // 邻居的顶点颜色数量表中，被选中顶点的旧的颜色的值-1，新的颜色的值+1
                vertexNeighbourColor[allList.get(vertexNum)[i]][graphColor[vertexNum]]--;
                vertexNeighbourColor[allList.get(vertexNum)[i]][colorNum]++;
            }
            // 更新顶点的颜色
            graphColor[vertexNum] = colorNum;

            iter++;
            System.out.println("iter:" + iter+ "，" + "conflict:" + conflictNum);

//            if (iter % 1000000 == 0) {
//                System.out.println("iter:"+iter+",conflict:"+conflictNum+","+"best:"+bestConflictNum);
//            }

            if (conflictNum == 0) {
//                System.out.println("iter:" + iter+ "，" + "conflict:" + conflictNum);
                break;
            }

        } while ( iter <= maxIter && conflictNum > 0);

        int[] vex = new int[graphColor.length + 1];
        System.arraycopy(graphColor, 0, vex, 1, graphColor.length);
        return vex;

    }
}
