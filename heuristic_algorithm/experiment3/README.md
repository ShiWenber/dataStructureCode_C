## Getting Started

Welcome to the VS Code Java world. Here is a guideline to help you get started to write Java code in Visual Studio Code.

## Folder Structure

The workspace contains two folders by default, where:

- `src`: the folder to maintain sources
- `lib`: the folder to maintain dependencies

Meanwhile, the compiled output files will be generated in the `bin` folder by default.

> If you want to customize the folder structure, open `.vscode/settings.json` and update the related settings there.

## Dependency Management

The `JAVA PROJECTS` view allows you to manage your dependencies. More details can be found [here](https://github.com/microsoft/vscode-java-dependency#manage-dependencies).

## ACA 求解TSP算法

1. 初始化：随机防止m只蚂蚁的七点，为每只蚂蚁建立禁忌表 tabuk，将各蚂蚁的初始节点置入禁忌表中

为什么设置：m = n

如果蚂蚁数量m>n，那么就会更可能会有多个蚂蚁走到同一个节点，这样就会导致节点的信息素的更新不准确，从而影响到后面的蚂蚁的选择。

如果蚂蚁数量m<n，那么就会更可能会有一个节点没有被蚂蚁走到，这样就会导致节点的信息素的更新不准确，从而影响到后面的蚂蚁的选择。

计算信息素浓度 $\tau_{ij}$ ,表示在节点 i 和 j 之间的信息素浓度

$$
\begin{aligned}
\tau_{ij} = \rho\tau_{ij} + \Delta \tau_{ij} \\
\Delta \tau_{ij} = \sum_{k=1} \Delta \tau
\end{aligned}
$$

选择下一个城市使用轮盘赌选择法(抛出一个随机数，落在那个区间就选择那个)

蚂蚁 k 选择路径 i-j 的概率

$$
p^k(i,j) = \begin{cases}
    \tau
$$

1. 构建路径
2. 更新信息素

算法
```pseudo

```

java中数组是一个引用，因此可以对其做直接的修改，也能保证共享信息全局更新

opt: 2586.7696475631606