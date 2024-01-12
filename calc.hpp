#ifndef CALC
#define CALC
//#include "qapplication.h"
//#include <QDebug>
#include "mainwindow.h"
#include <QString>
#include <QThread>
#include <string>
#include <random>
#include <vector>
#include <iostream>
#include <string.h>
//OO();
#include <windows.h>
#define MINI_STRING 5
using namespace std;
int const STRSIZE=623;
int const S=STRSIZE;
int const G=623-MINI_STRING;//每次遍历的起始位置。最后5个字符对应的计算似乎还可以通过存档减少运算。我先不做了。
int const M=624;//std::max(624,STRSIZE+1);

using uint = unsigned int;
using int8 = char;



/*
static bool const shallcontain[56]=
    //2  6  7
    {0,0, 0,0, 0,0, //31
        0,0, 0,0, 0,0, //35
        0,0, 1,1, 1,1, //39
        0,0, 0,0, 0,0, //41
        0,0, 0,0, 1,1,//45
        1,1, 1,1, 1,1,//49
        0,0, 0,0, 0,0, //81
        0,0, 1,1, 1,1,//85
        1,1, 1,1, 1,1, //89
        1,1,//any
};*/
static bool const shallnotcontain[64]=
    {1,1,1,1,1,1,
     1,1,1,1,1,1,
     1,1,0,0,0,0,
     1,1,1,1,1,1,
     1,1,1,1,0,0,
     0,0,0,0,0,0,
     1,1,1,1,1,1,
     1,1,0,0,0,0,
     0,0,0,0,0,0,
     0,0,true,true,true,true,true,true,true,true};//越界防范


inline uint Xor27(uint x){return x ^(x>>27);}
inline void f(uint*&Ak, uint*&A306k, uint*& A317k, uint &Rx4, int k)
{
        {
            auto Rx3=1566083941*Xor27(*Ak+*A306k+Rx4);

            *(A306k++)^=Rx3;
            *(A317k++)^=(*(Ak++)=Rx4=Rx3-(k));
        }
//        *(A317k++)^=Rx4;
}

class Calc :public QThread
{
    Q_OBJECT
public:

signals:
    void Update_Progress(QString s);
    void New_Discover(int t);
public:
    char Input[12]="!!!!!!!!!!";
    Calc(){}
    Calc(std::string s)
    {for(int i=0;i<10;i++)
            Input[i]=s[i];}

    //只找408

    uint A[2*624];//mt的本体。实际上没有设置twist。如果极极极极小的几率抽了几百抽都没抽出结果，最终会越界。
    uint A_init[624];//存档。每个种子开始先从这里读取前G步的结果。
    uint *const Agminus1=&A[G-1];
    uint *const A306 =&A[306];
    uint *const A317 =&A[317];
    uint *const A623 =&A[623];
    uint *const A7   =&A[7];
    uint *const A8   =&A[8];
    uint *const A404 =&A[404];
    uint *const A27  =&A[27 ];
    uint *const A28  =&A[28 ];
    uint *const A424 =&A[424];
    uint *const Ag = &A[G];
    uint *const A306g = &A[G-318];
    uint *const A317g =&A[G-307];//一堆后面用到的位置
//    uint *const A397 =&A[397];
//    uint *const A1 = &A[1];

    static_assert(624>=STRSIZE+1,"");
    //当string长度超过623时，mt的生成算法会变。
    //事实上，是第一阶段会执行到种子信息用完。然后原地开始第二阶段。
    //这会导致mt生成的最后步骤不在497~623的位置，会进入到前28次调用需要使用的位置，于是只好全部算完。


    bool current_shuffle[64];//洗牌后的每个位置的块在不在408里

    bool *const cr15=&current_shuffle[15];
    bool *const cr35=&current_shuffle[35];
//    bool *const cr18=&current_shuffle[18];
    bool *const cr20=&current_shuffle[20];
//    bool *const cr38=&current_shuffle[38];//洗牌后的一些常用位置。


    uint reps[56]; //常算的参数。此处索引可能有点费时间。是4294967295/nchoice
    uint *const reps54 = &reps[54];
    uint nchoice[56];//第2k个是(2k+1)(2k+2)。
   int userd;


    uint8_t *const dataGth = &fullstring[G];
    uint8_t *const data613th = fullstring + 613;//处理末尾信息的
    uint8_t *const data622th=&fullstring[622];

    uint8_t fullstring [623];//存储种子。因为。这里改成int可能能快。我先这样存，下次一定试试。


    void run() override
    {
        {

            for(int i=0;i<56;i++)
            {nchoice[i]=(i+1)*(i+2);        //设置nchoice
                reps[i]=uint(-1)/nchoice[i];}//设置rep：maxuint/nchoice。mt()除以他得到randint(0,nchoice-1)。如果是nchoice就重抽。这是LGTBot的std::shuffle的方法。


            for(int i=0;i<G;i++)
                fullstring[i]='?';
            //uint data[STRSIZE-1];
            for(int i=0;i<MINI_STRING;i++)dataGth[i]=int('!');

            {
                string str="Li2CO3_600?s_";
                for(int z=0;z<str.length();z++) fullstring[z]=str[z];
            }

            {std::string str = Input;
                for(int i=0;i<10;i++)
                    fullstring[613+i]=str[i];

            }
            fullstring[STRSIZE-1]=32;//空格。有效字符前一个字符。
            //设置初始种子。我这里定义的是Li2CO3_600?s_??????????????????? ... ??，然后613号位开始是十个自定义字符。
        }




        uint Rx2,Rx4;
        int swap1,swap2;


        uint *Ak,*A306k,*A317k;//第一第二阶段用的
//确定随机数时需要用的
        //由于是再循环。设置初始值后一直++。

        //前两阶段的所有下标都需要%624。此时后624都是未定义的。
        //第二阶段有一个参数是k%624。
        memset(A,0x8b8b8b8b,sizeof(A_init)) ;
        Rx2=A[uint(-1)%624];
        for(auto k=0;k<G;k++)
        {
            auto TempPlus306=A[(k+306)%624];
            Rx2 = 1664525 * Xor27(A[k%624] ^ (TempPlus306) ^Rx2);
            A[(k+306)%624]=TempPlus306+Rx2;
            uint Off;
            static_assert(S>=G-1,"保证第三种情况不出现");
            if(k==0){Off=S;}        //感觉这一步直接阻止了无限步逆推。
            else {Off=fullstring[k-1]+k%624;}
            //else if (k-1>S) Off=
            Rx2 = Rx2 +Off;

            A[(k+317)%624]=A[(k+317)%624]+Rx2;
            A[k%624]=Rx2;

        }
        memcpy(A_init,A,sizeof(A_init));




        uint8_t shuffleseedfirststeps[8];


        while(true)
        {//while true
            {
            (*data622th)++;
            int next=MINI_STRING-1;
            while(dataGth[next]==127)
            {dataGth[next]=33;next--;dataGth[next]++;
                if(next<0){
                    //DWORD endt=GetTickCount();
                    //qdebug()<<"\nruntime="<<(endt-startt)<<"ms"<<'\n';
                    data613th[4]+=(NTHREAD-1);
                    if(data613th[4]>126)
                        for(int t=4;t<10;t++)data613th[t]=126;
                    emit Update_Progress(QString("")
                                         +char(data613th[0])+char(data613th[1])+char(data613th[2])+char(data613th[3])+char(data613th[4])
                                         +char(data613th[5])+char(data613th[6])+char(data613th[7])+char(data613th[8])+char(data613th[9])
                                         );
                    connect(this,&QThread::finished,this,&QThread::deleteLater);

                    return;}
            }
            if(next<=MINI_STRING-4)
            {
                //'[';//***
                emit Update_Progress(QString("")
                                     +char(data613th[0])+char(data613th[1])+char(data613th[2])+char(data613th[3])+char(data613th[4])
                                     +char(data613th[5])+char(data613th[6])+char(data613th[7])+char(data613th[8])+char(data613th[9])
                                     );
                //qdebug()<<']';


            }
            }
/*
        std::seed_seq seed(fullstring,data2);//别的注释都可以取消 这个千万别 这个最慢了！
  */          //mt=std::mt19937(seed);
//#define use_mt

#ifdef use_mt
#define REPORT_EVERY_94_TO_X_STEPS 2
#else
#define REPORT_EVERY_94_TO_X_STEPS 3
#endif


#ifdef use_mt
            seed_seq sq(fullstring,&fullstring[623]);
            std::mt19937 mt(sq);
#else

            {
                //memset(A,0x8b8b8b8b,sizeof(A_init)) ;
                memcpy(A,A_init,sizeof(A_init));
                //for(i=0;i<624;i++)///A[i]=0x8b8b8b8b;
                Rx2= *Agminus1;


                Ak=Ag;
                A306k=A306g;
                A317k=A317g;

                for(short k=G;k!=M;k++)
                {
                    static_assert(G>=624-306,"保证所有的轮转都要-624");
                    static_assert(M==624,"A[k]的k要%624");
                    auto Rx1=*Ak ^ *A306k ^Rx2;
                    Rx2 = 1664525 * Xor27(Rx1);
                    *A306k=*A306k+Rx2;


                    static_assert(S>=M-1 && G>0,"保证第三种情况不出现");


                    //Off=data[k-(G+1)]+k;//k%624;
                    //if k==0; else if k<=S.len; else
                    Rx2 = Rx2 +dataGth[k-(G+1)]+k;//Off;
                    *Ak=Rx2;  //A[mod624[k]]=Rx2;
                    *A317k=*A317k+Rx2;
                    Ak++;A306k++;A317k++;
                }


                Rx4=*A623;

                Ak=A;
                A306k=A306;
                A317k=A317;
                for(short k=0;k!=307;k++)//不留余量。额外需要的抽卡次数超过2的只有3e-6概率 感觉为了它加代码都是亏了
                {
                    f(Ak, A306k, A317k, Rx4,k);

                }
                A317k=A;
                for(short k=307;k!=318;k++)//不留余量。额外需要的抽卡次数超过2的只有3e-6概率 感觉为了它加代码都是亏了
                {
                    f(Ak, A306k, A317k, Rx4,k);

                }
                A306k=A;
                for(short k=318;k!=425;k++)//不留余量。额外需要的抽卡次数超过2的只有3e-6概率 感觉为了它加代码都是亏了
                {
                    f(Ak, A306k, A317k, Rx4,k);
                }


                {                uint* A397w;
                    uint *Aw;
                    uint *A1w;
                    uint *repsw=reps54;
                    Aw=A27;
                    A1w=A28;
                    A397w=A424;
                    uint8_t tg=27;
                    bool HAS_LAIZI;
                    for(;tg!=23;tg--)
                {

                    auto g  = (*(Aw--) & 0x80000000U) | (*(A1w--) & 0x7fffffffU);
                    auto Res = (g >> 1) ^ (g & 1 ? 0x9908b0dfU : 0) ^ *(A397w--);


                    Res ^= (Res>>11);
                    Res ^=(Res<<7)&(0x9d2c5680U);

                    Res ^=(Res<<15)&(0xefc60000U);
                    Res ^= (Res)>>18;
                    Res/=*repsw;
                    repsw-=2;

                    short a=Res/((tg+1)<<1);
                    short b=Res%((tg+1)<<1);
                    if(a>=40 || b>=40)break;
                    if(tg==27){HAS_LAIZI= (b<20);
                        if((a<20) != HAS_LAIZI)break;
                        }

                    if(tg!=27 && ((b<20!=HAS_LAIZI)||(a<20!=HAS_LAIZI)))break;
                    int i=((tg&3)+1)<<1;
                    if(a==b)break;
                    for(;i!=8;i++)
                    {if(a==shuffleseedfirststeps[i] || b==shuffleseedfirststeps[i])break;}
                        if(i!=8)break;
                    shuffleseedfirststeps[(tg&3)<<1]=a;
                    shuffleseedfirststeps[((tg&3)<<1) +1]=b;
                }
                if(tg!=23) continue;
                }


            }

#endif

//删了一段。真0了是2^-19937的几率。

//用到:0-29,397-424





            //int8 current_shuffle[56];
            //uint reps[28];    set
            //uint nchoice[28]; set
            //int target;int userd;
            {
            uint* A397w;
            uint *Aw;
            uint *A1w;

            Aw=A7;
            A1w=A8;
            A397w =A404;
            memset(current_shuffle,1,14*sizeof(bool));

            int target=14;
            for(;target!=56;)
            {

                auto g  = (*(Aw++) & 0x80000000U) | (*(A1w++) & 0x7fffffffU);
                auto Res = (g >> 1) ^ (g & 1 ? 0x9908b0dfU : 0) ^ *(A397w++);


                Res ^= (Res>>11);
                Res ^=(Res<<7)&(0x9d2c5680U);

                Res ^=(Res<<15)&(0xefc60000U);
                Res ^= (Res)>>18;

                auto k= Res/reps[target];//k是(target+1)*(target+2)范围内对应的数字
                //if(k== ((target+1)*(target+2)) )break;
                swap1=k/(target+2);
                swap2=k%(target+2);
                current_shuffle[target]=current_shuffle[swap1];
                current_shuffle[swap1]=shallnotcontain[target++];
                current_shuffle[target]=current_shuffle[swap2];
                current_shuffle[swap2]=shallnotcontain[target++];
            }

            if(target!=56)continue;

            }
            bool HAS_LAIZI =swap1<20;

            if(HAS_LAIZI)
            {
                bool *boolpt;
                for(boolpt=current_shuffle; boolpt !=cr15;boolpt++)
                    if(*boolpt)break;

                if(boolpt!=cr15)continue;//如果前18块有408不要的，while种子循环
                // if((*((unsigned int*)(current_shuffle))) & 0xffffc000U) continue;//这行是错的。没试出来像这样的实现方式
            }
            else
            {
                bool *boolpt;
                for(boolpt=cr20; boolpt !=cr35;boolpt++)
                    if(*boolpt)break;


                if(boolpt!=cr35)continue;//如果前18块有408不要的，回到while种子循环
            }
            //
            bool *b=HAS_LAIZI?cr15:cr35;
            char buf[50];
            int totb=b[0]+b[1]+b[2]+b[3]+b[4];

            if(totb<3)
            {
            //uint *x = &data[623-G];
            sprintf(buf,"%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_%d.txt",data613th[0],data613th[1],data613th[2],data613th[3],data613th[4],
                    data613th[5],data613th[6],data613th[7],data613th[8],data613th[9],totb);
            //00 表示最后两块都是好块。
            //
            FILE *f =fopen( buf,"w");
            for(int i=0;i!=623;i++)fprintf(f,"%c",fullstring[i]);
            fclose(f);
            }
            emit New_Discover(totb);

        }

    }





};


#endif



#ifdef THIS_IS_IN_THE_STD_FILE

using mt19937 = mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 0x9908b0df, 11, 0xffffffff, 7,
                                        0x9d2c5680, 15, 0xefc60000, 18, 1812433253>;


_EXPORT_STD template <class _Ty, size_t _Wx, size_t _Nx, size_t _Mx, size_t _Rx, _Ty _Px, size_t _Ux, _Ty _Dx,
                     size_t _Sx, _Ty _Bx, size_t _Tx, _Ty _Cx, size_t _Lx, _Ty _Fx>
explicit mersenne_twister_engine(_Seed_seq& _Seq) : _Mybase(default_seed, _Dx, _Fx) {//这个Mybase就是一个2*624的uint数组。DxFx是初始化，后续会被覆盖。
    seed(_Seq);
}



void seed(_Seed_seq& _Seq) { // reset sequence from seed sequence
    constexpr int _Kx = (_Wx + 31) / 32;        //使用uint的时候，这个就是1
    unsigned long _Arr[_Kx * _Nx];              //这是临时624数组
    _Seq.generate(&_Arr[0], &_Arr[_Kx * _Nx]);  //给这个数组生成。参数都在。

    int _Idx0 = 0;
    _Ty _Sum  = 0;
    for (size_t _Ix = 0; _Ix < _Nx; ++_Ix, _Idx0 += _Kx) { // pack _Kx words    //这段的情况是，如果生成的东西不是uint而是更长的东西才会有效。
        this->_Ax[_Ix] = static_cast<_Ty>(_Arr[_Idx0]);                         //Ax是uint，Ty也是uint。所以其实就是把Arr抄到Ax这里。
        for (int _Jx = 1; _Jx < _Kx; ++_Jx) {                                   //Kx=1，这一段也没了
            this->_Ax[_Ix] |= static_cast<_Ty>(_Arr[_Idx0 + _Jx]) << (32 * _Jx);
        }

        this->_Ax[_Ix] &= this->_WMSK;  //WMSK是全1，这一段没了        static constexpr _Ty _WMSK = ~((~_Ty{0} << (_Wx - 1)) << 1);

        if (_Ix == 0) {                 //这段是说，如果所有所有所有623个uint和第一个的第一位全都是0，这是2^-19937的几率。//623*32+1=19937
            _Sum = this->_Ax[_Ix] >> _Rx;   //Rx=31,只留最高位
        } else {
            _Sum |= this->_Ax[_Ix];     //后面的全留
        }
    }

    if (_Sum == 0) {        //把第一个数改了。我不知道这是什么意义。
        this->_Ax[0] = _Ty{1} << (_Wx - 1);
    }

    this->_Idx = _Nx;       //初始位置是624。
}

void generate(_RanIt _First, _RanIt _Last) const { // generate randomized interval from seeds
    _Adl_verify_range(_First, _Last);
    auto _UFirst   = _Get_unwrapped(_First);                                //安全性验证
    const auto _Nx = static_cast<size_t>(_Get_unwrapped(_Last) - _UFirst);  //安全性验证

    if (0 < _Nx) { // finite interval, fill it
        const size_t _Sx = _Myvec.size();
        const size_t _Tx = 623 <= _Nx ? 11 : 68 <= _Nx ? 7 : 39 <= _Nx ? 5 : 7 <= _Nx ? 3 : (_Nx - 1) / 2;
        const size_t _Px = (_Nx - _Tx) / 2;
        const size_t _Qx = _Px + _Tx;
        const size_t _Mx = _Nx <= _Sx ? _Sx + 1 : _Nx;
        size_t _Kx;                         //这些是参数。Sx是数据长度。但是构造vector和析构vector似乎是主要费时操作。
            //Tx=11是从内部数据到随机结果转换中的参数。参考operator()()处。
            //Px=(624-11)/2=306。 Qx=Px+11=317
            //Mx=初始化第一阶段长度。至少为N。如果S太长就是S+1把种子信息用完。
            //Kx是临时参数。

        _Iter_value_t<_RanIt> _Mask = _Iter_value_t<_RanIt>(1) << 31;
        _Mask <<= 1; // build 32-bit mask safely
        _Mask -= 1;                         //是一个32位1.使用非unsigned int的情况下需要。我的代码全都略屌了。

        for (_Kx = 0; _Kx < _Nx; ++_Kx) {   //真正的初始化。后续会将Arr的值覆盖到MyBase上。
            _UFirst[_Kx] = 0x8b8b8b8b;
        }

        for (_Kx = 0; _Kx < _Mx; ++_Kx) { // scramble and add any vector contributions  //第一阶段
            result_type _Rx1 =
                1664525 * _Xor27(_UFirst[_Kx % _Nx] ^ _UFirst[(_Kx + _Px) % _Nx] ^ _UFirst[(_Kx - 1) % _Nx]);

            // K%N (K+306)%N (k+317)%N
            size_t _Off;                     //根据种子生成的Offset。
            if (_Kx == 0) {
                _Off = _Sx;                 //第一个是种子长度
            } else if (_Kx <= _Sx) {
                _Off = _Kx % _Nx + _Myvec[_Kx - 1]; //第二个。注意如果超长了后面的K也要%N。
            } else {
                _Off = _Kx % _Nx;           //种子短，会有这个情况。
            }
            result_type _Rx2 = static_cast<result_type>((_Rx1 + _Off) & _Mask);

            _UFirst[(_Kx + _Px) % _Nx] = (_UFirst[(_Kx + _Px) % _Nx] + _Rx1) & _Mask;   //+P是306，+Q是317
            _UFirst[(_Kx + _Qx) % _Nx] = (_UFirst[(_Kx + _Qx) % _Nx] + _Rx2) & _Mask;
            _UFirst[_Kx % _Nx]         = _Rx2;
        }
        for (; _Kx < _Mx + _Nx; ++_Kx) { // rescramble              //第二阶段。每一步改K，K+P，K+Q。于是最后用到的是316往前、305往前和623往前。
            result_type _Rx3 =
                1566083941 * _Xor27(_UFirst[_Kx % _Nx] + _UFirst[(_Kx + _Px) % _Nx] + _UFirst[(_Kx - 1) % _Nx]);
            result_type _Rx4 = static_cast<result_type>((_Rx3 - _Kx % _Nx) & _Mask);

            _UFirst[(_Kx + _Px) % _Nx] = (_UFirst[(_Kx + _Px) % _Nx] ^ _Rx3) & _Mask;
            _UFirst[(_Kx + _Qx) % _Nx] = (_UFirst[(_Kx + _Qx) % _Nx] ^ _Rx4) & _Mask;
            _UFirst[_Kx % _Nx]         = _Rx4;
        }
    }
}

result_type _Xor27(result_type _Val) const { // shift and merge
    return _Val ^ (_Val >> 27);
}

_NODISCARD result_type operator()() {       //这里是抽取随机数
    if (this->_Idx == _Nx) {
        _Refill_upper();                //Idx在624到2*624之间增加。初始会refill_upper。此前upper完全没动过，是未初始化的值。
    } else if (2 * _Nx <= this->_Idx) {
        _Refill_lower();                //抽624次后会用到这个。我没看这是什么。
    }

    _Ty _Res = this->_Ax[this->_Idx++] & _WMSK; //从Idx=624开始。直接调用数字。因此我不计算后面624，只算前至多30个。
    _Res ^= (_Res >> _Ux) & _Dxval;     //Dxval是32个1，忽略。Ux=11
    _Res ^= (_Res << _Sx) & _Bx;        //Sx=7，Bx=0x9d2c5680
    _Res ^= (_Res << _Tx) & _Cx;        //Tx=15, Cx=0xefc6000
    _Res ^= (_Res & _WMSK) >> _Lx;      //Lx=18。这些都是mt19937的参数。
    return _Res;
}


void _Refill_upper() { // compute values for the upper half of the history array
    for (size_t _Ix = _Nx; _Ix < 2 * _Nx; ++_Ix) { // fill in values
        const _Ty _Tmp = (this->_Ax[_Ix - _Nx] & _HMSK) | (this->_Ax[_Ix - _Nx + _One_mod_n] & _LMSK);  //
        this->_Ax[_Ix] = (_Tmp >> 1) ^ (_Tmp & 1 ? _Px : 0) ^ this->_Ax[_Ix - _Nx + _M_mod_n];//Px是mt19937传过来的0x9908bb0df那个参数
    }
}

static constexpr _Ty _WMSK = ~((~_Ty{0} << (_Wx - 1)) << 1);    //32个1
static constexpr _Ty _HMSK = (_WMSK << _Rx) & _WMSK;            //1后面31个0
static constexpr _Ty _LMSK = ~_HMSK & _WMSK;                    //0后面31个1

static constexpr int _One_mod_n = 1 % _Nx; // either 0 or 1     //Nx=624,所以这是1.
static constexpr int _M_mod_n   = _Mx % _Nx;                    //这个M是mt19937传过来的397那个参数。
#endif
