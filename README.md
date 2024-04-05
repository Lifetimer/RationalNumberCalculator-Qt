# 基于Qt实现的简易有理数计算器

这个项目是作者课程“软件设计综合实验”的作业之一，使用C++初步实现了基本计算器有理数计算的相关功能，通过C++实现了高精度计算，理论上支持极高的运算精度。可以通过分数或小数的方式进行输入，运算结果会以分数和小数的形式输出，能够自定义修改小数点后保留的位数。

## 功能演示

### 整数加法
![整数加法](/pictures/IntAdd.png)

### 整数减法
![整数减法](/pictures/IntSub.png)

### 整数乘法
![整数乘法](/pictures/IntMulti.png)

### 整数除法
![整数除法](/pictures/IntDiv.png)

### 分数加法
![分数加法](/pictures/DoubleAdd.png)

### 分数减法
![分数减法](/pictures/DoubleSub.png)

### 分数乘法
![分数乘法](/pictures/DoubleMulti.png)

### 分数除法
![分数除法](/pictures/DoubleDiv.png)

### 小数分数混合输入
![小数分数混合输入](/pictures/MixInput.png)

### 高精度计算

高精度计算，结果过长时会新打开一个窗口显示计算结果

![高精度计算](/pictures/HighPrecisionCalc.png)

### 整数的幂次运算

![整数的幂次运算](/pictures/PowerCalc.png)

## 源代码结构
源代码位于(./src)目录下

```
src
│  CalcMainWindow.cpp   // 计算器主界面源
│  CalcMainWindow.h     // 计算器主界面头文件
│  CalcMainWindow.ui    // Qt界面UI文件
│  main.cpp             // 主函数文件
│  RationalNumberCalcController.cpp // 控制器，接受视图要求的指令，并调用RationalCalcEngine进行计算
│  RationalNumberCalcController.h   // 控制器对应头文件
│
└─RationalNumberCalcEngine
    │  RationalCalcEngine.cpp   // 负责调用BigInt和Fraction提供的接口进行计算
    │  RationalCalcEngine.h     // 计算引擎头文件
    │
    └─RationalCalculationLib
            BigInt.cpp          // 高精度的整型库
            BigInt.h
            Fraction.cpp        // 分数计算
            Fraction.h
```



### 后续维护想法
因作者本人能力有限，这份代码的可读性不高，底层实现的高精度计算的效率和安全性也不高，因此该项目并无长期维护的想法。这个项目是作为一个例子，为刚接触Qt界面设计的同学提供一个参考。

作者正在学习数字电子技术，后续会考虑建立新的仓库，制作支持“进制转换”“真值表穷举”“二、三、四变量卡诺图显示”等功能的新计算器。

若时间和精力足够，会考虑使用java重写并迁移至Android平台。