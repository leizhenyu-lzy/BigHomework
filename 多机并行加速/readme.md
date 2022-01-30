文件内容使用说明：
1. MaxSum 单机多线程执行求和以及求最大值（使用Visual Studio完成）
    1. getMax.cpp 使用gcc进行编译： g++ getMax.cpp -fopenmp -o getMax
    2. getSum.cpp 使用gcc进行编译： g++ getSum.cpp -fopenmp -o getSum
    3. 也可以直接运行里面的已经编译好的.exe文件
2. MySortSingle 单机多线程排序代码（使用Visual Studio完成）
   1. 使用visual studio进行编写，所以直接点击里面的.sln文件即可。
   2. 记得在Visual Studio的项目配置中打开OpenMP支持。
3. MySortMulti 多机并行排序代码（使用Visual Studio Code完成）
   1. client.cpp 使用gcc进行编译： g++ client.cpp -lwsock32 -fopenmp -o client
   2. server.cpp 使用gcc进行编译： g++ server.cpp -lwsock32 -o server
   3. MySort.h 是客户端程序和服务器程序公用的头文件
   4. server和client中的ip地址已经改为"127.0.0.1"，所以通过编译后可以直接双击.exe进行测试
   5. 注意要先开启server后打开client
4. Pics：存放各种测试结果的截图，可以随意查看