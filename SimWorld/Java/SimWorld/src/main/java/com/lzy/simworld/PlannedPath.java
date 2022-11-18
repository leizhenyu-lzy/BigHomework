package com.lzy.simworld;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class PlannedPath
{
    private Coordinate origin;
    private Coordinate destination;
    private ArrayList<Coordinate> path = new ArrayList<Coordinate>();  // 经过多少个点只需要计算ArrayList的长度
    private int pathCost = 0;  // 总路径长度（不是经过点的个数）
    private int pathPointer = 0;  // 记录走到路径的哪里了

    // 构造函数，由需提供两个Coordinate作为起始点和终点
    public PlannedPath(Coordinate outerOrigin, Coordinate outerDestination)
    {
        // 对于给定两个Coordinate创建的path，是用于寻路的，所以不用添加起点到path中
        this.origin = outerOrigin;
        this.destination = outerDestination;
    }

    public PlannedPath(Coordinate originCoord)
    {
        this.origin = originCoord;
        path.add(this.origin);
    }

    // 无参构造函数
    public PlannedPath()
    {

    }

    // A*路径搜索算法，如果没有找到，返回-1
    public int pathPlanningAStar(final WorldMap outerWorldMap, final int directionNums, final int distCalculateChoice)
    {
        this.pathCost = -1;
        // 可以选择8、4方向搜索（默认8方向）
        int[][] DIRECTIONS;
        int[] DIRECTIONS_COST;
        if(directionNums == Consts.EIGHT_DIRECTIONS_NUM)
        {
            DIRECTIONS = Consts.EIGHT_DIRECTIONS;
            DIRECTIONS_COST = Consts.EIGHT_DIRECTIONS_COST;
        }
        else
        {
            DIRECTIONS = Consts.FOUR_DIRECTIONS;
            DIRECTIONS_COST = Consts.FOUR_DIRECTIONS_COST;
        }

        // 记录距离起点的路径代价
        int[][] pathCostsFromOriginMat = new int[outerWorldMap.getMapRows()][outerWorldMap.getMapCols()];
        // 记录已经被收录的点
        int [][]closedMat = new int[outerWorldMap.getMapRows()][outerWorldMap.getMapCols()];
        // 记录上一个点，用于最后从终点反推路径
        Coordinate [][]formerCoordinateMat = new Coordinate[outerWorldMap.getMapRows()][outerWorldMap.getMapCols()];
        // 记录将会被处理的点
        ArrayList<Coordinate> openCoordinateList = new ArrayList<Coordinate>();
        // 记录路径总代价（使用arrayList的原因是方便选出代价最低的）
        ArrayList<Integer> openCoordinateTotalPathCostList = new ArrayList<Integer>();

        // 初始化三个数组
        for(int i=0;i< outerWorldMap.getMapRows();i++)
        {
            for(int j=0;j< outerWorldMap.getMapCols();j++)
            {
                // 将障碍物设置为收录，其他点设为未被收录
                if(outerWorldMap.coordinateIsObstacle(i,j))
                    closedMat[i][j] = Consts.IN_CLOSE_LIST;
                // else
                //     closeMat[i][j] = Consts.IN_UNKNOWN_LIST;
            }
        }
        closedMat[this.origin.row][this.origin.col] = Consts.IN_CLOSE_LIST;  // 将起点进行收录

        boolean missionSuccessFlag = false;
        Coordinate newlyEnclosedCoordinate = new Coordinate(this.origin.row,this.origin.col);

        while (true)
        {
            // System.out.println("newlyEnclosedCoordinate : " + newlyEnclosedCoordinate.getCoordinateInfoString());
            // 添加邻接节点，并更新代价
            for(int dirIdx=0;dirIdx<directionNums;dirIdx++)
            {
                // 当前计算点的横纵坐标
                Coordinate tempCoordinate = new Coordinate(0,0);
                tempCoordinate.row = newlyEnclosedCoordinate.row + DIRECTIONS[dirIdx][0];
                tempCoordinate.col = newlyEnclosedCoordinate.col + DIRECTIONS[dirIdx][1];

                // 如果点已经被收录（包括障碍物），则跳过
                if(closedMat[tempCoordinate.row][tempCoordinate.col] == Consts.IN_CLOSE_LIST)
                    continue;
                else
                {
                    int coordinateIndex = openCoordinateList.indexOf(tempCoordinate);  // 当前coordinate在openCoordinateList中的位置（如果在其中）
                    // 计算距离终点的距离
                    int pathCostToDestination = ToolFunctions.calculateDistance(distCalculateChoice, tempCoordinate, newlyEnclosedCoordinate);
                    pathCostToDestination = pathCostToDestination*Consts.A_STAR_AIM_WEIGHT;  // 距离终点的距离乘以相应的权重
                    // 计算当前点通过上一个收录点距离起点的距离
                    int pathCostToOrigin = pathCostsFromOriginMat[newlyEnclosedCoordinate.row][newlyEnclosedCoordinate.col] + DIRECTIONS_COST[dirIdx];
                    // 计算当前点通过上一个收录点的路径的总距离
                    int totalPathCost = pathCostToOrigin + pathCostToDestination;

                    // 当前点不在openCoordinateList中，则添加
                    if(coordinateIndex == -1)
                    {
                        openCoordinateList.add(tempCoordinate);  // 每次for循环都会添加一个新的
                        openCoordinateTotalPathCostList.add(totalPathCost);
                        formerCoordinateMat[tempCoordinate.row][tempCoordinate.col] = newlyEnclosedCoordinate;
                        pathCostsFromOriginMat[tempCoordinate.row][tempCoordinate.col] = pathCostToOrigin;
                    }
                    // 当前处理的点在openCoordinateList中，检查是否要更新
                    else if(coordinateIndex !=-1)
                    {
                        if(totalPathCost<openCoordinateTotalPathCostList.get(coordinateIndex))
                        {
                            openCoordinateTotalPathCostList.set(coordinateIndex, totalPathCost);
                            formerCoordinateMat[tempCoordinate.row][tempCoordinate.col] = newlyEnclosedCoordinate;
                            pathCostsFromOriginMat[tempCoordinate.row][tempCoordinate.col] = pathCostToOrigin;
                        }
                    }
                }
            }

            // 如果openCoordinateList为空，说明没有待处理的点，所有点都试过了，没有找到路径，则表明任务失败
            if(openCoordinateList.isEmpty())
            {
                missionSuccessFlag = false;
                // System.out.println("Path isn't Found!!! (Using A* Algorithm)");
                break;
            }
            // openCoordinateList不为空，找到openCoordinateList中的totalPathCost最小的点
            else
            {
                int minPathCostCoordinateValue = Collections.min(openCoordinateTotalPathCostList);  // 找到最小值
                int minPathCostCoordinateIndex = openCoordinateTotalPathCostList.indexOf(minPathCostCoordinateValue);  // 找到最小值对应的idx
                newlyEnclosedCoordinate = openCoordinateList.get(minPathCostCoordinateIndex);
                closedMat[newlyEnclosedCoordinate.row][newlyEnclosedCoordinate.col] = Consts.IN_CLOSE_LIST;

                openCoordinateList.remove(minPathCostCoordinateIndex);
                openCoordinateTotalPathCostList.remove(minPathCostCoordinateIndex);
            }

            // 如果终点已经被收录，设置任务成功标志位，生成路径，并退出
            if(closedMat[this.destination.row][this.destination.col] == Consts.IN_CLOSE_LIST)
            {
                missionSuccessFlag = true;
                // System.out.println("Path is Found!!! (Using A* Algorithm)");
                // 生成路径
                this.path.clear();
                Coordinate tempGeneratePathCoordinate = new Coordinate(this.destination);
                while(!tempGeneratePathCoordinate.equals(this.origin))
                {
                    this.path.add(tempGeneratePathCoordinate);
                    tempGeneratePathCoordinate = formerCoordinateMat[tempGeneratePathCoordinate.row][tempGeneratePathCoordinate.col];
                }
                this.path.add(this.origin);
                Collections.reverse(this.path);  // 倒序则转为正向路线（包括起点和终点）
                // System.out.println("Path is Generated. (Using A* Algorithm)");
                // 计算路径长度（不只是经过的点）
                this.pathCost = pathCostsFromOriginMat[this.destination.row][this.destination.col]; // 显然对于终点，只需计算其距离起点的距离即可
                // System.out.println("PathCost : " + this.pathCost);
                break;
            }
        }
        return this.pathCost;
    }


    // 返回一共经历了几个点
    public int printPlannedPath()
    {
        if(this.path.size()==0)
        {
            System.out.println("Can Find a Path.");
            return -1;
        }
        else
        {
            int counter = 0;
            for(Coordinate coordinate:this.path)
            {
                System.out.print(coordinate.getCoordinateInfoString());
                ++counter;
                if(counter%5==0)
                    System.out.println();
            }
            return this.path.size();
        }
    }

    // 将给定路径添加进当前路径(返回合并后路径总长度)
    public int pathMerge(final PlannedPath anotherPath)
    {
        System.out.print("原始路径长度 : "+this.path.size()+"  ||  "+"新增路径长度 : "+anotherPath.path.size()+"  ||  ");
        this.path.addAll(anotherPath.path);
        int pathSize = this.path.size();
        System.out.println("合计路径长度 : "+pathSize);
        return pathSize;
    }
    // 将给定任务所执行的路径添加进当前路径(返回合并后路径总长度)
    public int pathMerge(final Task anotherTask)
    {
        return pathMerge(anotherTask.getTaskPath());
    }

    public void appendOneCoord(final Coordinate coord)
    {
        this.path.add(coord);
    }



    // Getters&Setters
    public Coordinate getOrigin()
    { return this.origin; }
    public Coordinate getDestination()
    {
        return this.destination;
    }
    public ArrayList<Coordinate> getPath()
    {
        return this.path;
    }
    public int getPathCost()
    {
        return this.pathCost;
    }
    public int getPathPointer()
    {
        return this.pathPointer;
    }
    public int getPathLength()
    {
        return this.path.size();
    }
    public void setOrigin(final Coordinate origin) { this.origin = origin; }
    public void setDestination(final Coordinate destination) { this.destination = destination; }
    public Coordinate retrieveCoordinateByPointer(final int retrievePointer)
    {
        if(retrievePointer>=this.getPathLength())
        {
            return null;
        }
        else
        {
            return this.path.get(retrievePointer);
        }
    }


    // 更新pathPointer，返回位置有没有变动boolean类型
    public boolean updatePathPointer(final int addNum)
    {
        // 理论上pointer是不会超过getPath.size的，最多等于，所以取路径的时候需要"左闭右开"
        if(this.pathPointer == this.getPath().size())
        {
            return false;
        }
        else
        {
            if(this.pathPointer+addNum>=this.getPathLength())
            {
                this.pathPointer = this.getPathLength();
            }
            else
            {
                this.pathPointer = this.pathPointer+addNum;
            }
            return true;
        }
    }

    public PlannedPath getSubPathByPointers(final int startPointer, final int endPointer)
    {
        PlannedPath subPath = new PlannedPath();
        if(endPointer<=startPointer)  // 指针大小有问题，返回空路径
        {
            return new PlannedPath();
        }
        else
        {
            for(int i = startPointer;i<endPointer;i++)  // endPointer是不会碰到的
            {
                subPath.path.add(this.path.get(i));
            }
            // subPath.path = (ArrayList<Coordinate>) this.path.subList(startPointer,endPointer);
            subPath.origin = this.path.get(startPointer);
            subPath.origin = this.path.get(endPointer-1);
            return subPath;
        }
    }


    // public String getPathTrimmedJsonInfo()
    // {
    //     StringBuffer tempBuffer = new StringBuffer();
    //
    //     int[] rows = null;
    //     int[] cols = null;
    //     int length = 0;
    //
    //     synchronized (this)
    //     {
    //         length = this.getPathLength();
    //         rows = new int[length];
    //         cols = new int[length];
    //
    //         for(int i=0;i<this.getPathLength();i++)
    //         {
    //             rows[i] = this.path.get(i).row;
    //             cols[i] = this.path.get(i).col;
    //         }
    //     }
    //
    //
    //     tempBuffer.append("\"pathLength\":");
    //     tempBuffer.append(length);
    //     tempBuffer.append(",\"path\":");
    //     tempBuffer.append("{");
    //     // 添加rows
    //     tempBuffer.append("\"rows\":{");
    //     tempBuffer.append(Arrays.toString(rows));
    //     tempBuffer.append("}");
    //     // 添加cols
    //     tempBuffer.append(",\"cols\":{");
    //     tempBuffer.append(Arrays.toString(cols));
    //     tempBuffer.append("}");
    //     // 添加结尾
    //     tempBuffer.append("}");
    //
    //     return String.valueOf(tempBuffer);
    // }

    // public static void main(String[] args)
    // {
    //     PlannedPath path = new PlannedPath(new Coordinate(1,2), new Coordinate(3,4));
    //     System.out.println(path.getPathTrimmedJsonInfo());
    //
    // }
}

