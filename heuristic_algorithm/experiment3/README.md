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