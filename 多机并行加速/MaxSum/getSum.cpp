#include"iostream"
#include"omp.h"
#include"ctime"
#include"cmath"
#include"xmmintrin.h"
#include <iomanip>
using namespace std;

#define MAX_THREADS 64
#define SUBDATANUM 1000000
#define DATANUM (SUBDATANUM * MAX_THREADS)   
float rawFloatData[DATANUM];

//不使用任何加速手段
float sum(const float data[],const int len)
{
    double res=0;
    for(int i=0;i<len;i++)
    {
        res+=sqrt(sqrt(data[i]));
        //res+=data[i];
    }
    return res;
} 

//128位SSE指令+四路循环展开
float sumSpeedUp(float data[],unsigned int len)
{
    float s=0;
    unsigned int nBlockWidth=4*4;
    unsigned int cntBlock=len/nBlockWidth;
    unsigned int cntRem=len%nBlockWidth;
    __m128 fSum0=_mm_setzero_ps();//求和变量，初值清零
    __m128 fSum1=_mm_setzero_ps();
    __m128 fSum2=_mm_setzero_ps();
    __m128 fSum3=_mm_setzero_ps();
    __m128 fLoad0,fLoad1,fLoad2,fLoad3;
    float *p=data;
        for (unsigned int i=0;i<cntBlock;i++)
        {            

            fLoad0=_mm_load_ps(p);//加载
            fLoad1=_mm_load_ps(p+4);
            fLoad2=_mm_load_ps(p+8);
            fLoad3=_mm_load_ps(p+12);

            fLoad0=_mm_sqrt_ps(fLoad0);//sqrt
            fLoad1=_mm_sqrt_ps(fLoad1);
            fLoad2=_mm_sqrt_ps(fLoad2);
            fLoad3=_mm_sqrt_ps(fLoad3);

            fLoad0=_mm_sqrt_ps(fLoad0);//sqrt
            fLoad1=_mm_sqrt_ps(fLoad1);
            fLoad2=_mm_sqrt_ps(fLoad2);
            fLoad3=_mm_sqrt_ps(fLoad3);

            fSum0=_mm_add_ps(fSum0,fLoad0);//求和
            fSum1=_mm_add_ps(fSum1,fLoad1);
            fSum2=_mm_add_ps(fSum2,fLoad2);
            fSum3=_mm_add_ps(fSum3,fLoad3);
            p+=nBlockWidth;
        }
    fSum0=_mm_add_ps(fSum0,fSum1);
    fSum2=_mm_add_ps(fSum2,fSum3);
    fSum0=_mm_add_ps(fSum0,fSum2);
    const float*q=(const float*)&fSum0;
    s=q[0]+q[1]+q[2]+q[3];                //合并
    for (unsigned int i=0;i<cntRem;i++)//处理尾部剩余数据
    {
        s+=sqrt(sqrt(p[i]));
    }
    return s;
}



int main()
{
    //数据初始化
    for(size_t i = 0; i < DATANUM; i++)
    {
        rawFloatData[i] = float(i+1);
    }

    //不使用任何加速
    clock_t start=clock();
    float slowSum=sum(rawFloatData,DATANUM);
    cout<<"Sum without speed up:";
    cout<<setprecision(5)<<slowSum<<endl;
    clock_t end=clock();
    double slowTime=(end-start);
    cout<<"no speed up:"<<(end-start)<<"ms"<<endl;

    // //使用SSE+openmp加速
    // float *fastSumList=new float[MAX_THREADS];
    // start=clock();
    // float fastSum=0;
    // #pragma omp parallel for reduction(+:fastSum)
    //     for(long i=0;i<MAX_THREADS;i++)
    //         fastSum=fastSum+sumSpeedUp(rawFloatData+i*SUBDATANUM,SUBDATANUM);
    // cout<<"Sum with SSE&openmp speed up:";
    // cout<<setprecision(5)<<fastSum<<endl;
    // end=clock();
    // double fastTime=(end-start);
    
    // //使用多线程
    start=clock();
    double he=0;
    #pragma omp parallel for reduction(+:he)
        for(long i=0;i<DATANUM;i++)
            he+=rawFloatData[i];
    cout<<"Sum with openmp speed up:"<<he<<endl;
    end=clock();
    double fastTime=(end-start);

    //使用sse
    // start=clock(); 
    // float fastSum=0;
    // fastSum+=sumSpeedUp(rawFloatData,DATANUM);
    // cout<<"Sum with sse speed up:"<<fastSum<<endl;
    // end=clock();
    // double fastTime=(end-start);

    //输出加速比
    cout<<"speed up:"<<fastTime<<"ms"<<endl;
    cout<<"speed up rate:"<<slowTime/fastTime<<endl;

    system("pause");
    return 0;
}