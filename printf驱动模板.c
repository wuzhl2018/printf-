/*可变参数是指某一个函数被调用的时候，并不知道具体传递进来的参数类型和参数的数目，
例如大家熟知的函数printf（）。C语言是通过软件堆栈的方式进行参数传递的，对于下面的函
数，从左到右依次压入栈中的变量为：a、b、c，如果存在更多的参数，只要在函数真正被调用
前按照同样的顺序依次压入栈中就可以完成任意数量的参数传递，这就是可变参数传递的原理。
在函数声明时，在函数列表最右边加入一个省略号“...”作为参数就可以将一个函数声明为可变
参数传递，例如：*/


void printf(char *pString, ...);    /*一个使用可变参数的例子*/

/*可变参数实际上具有参数类型 va_list.在参数内部必须要首先声明一个可变参数变量，以便
依次取出所有传入的数据，例如：*/

va_list    Example;        /*定义一个可变参数列表*/

/*va_list 可以像普通变量一样充当函数的参数和返回值，例如*/


void FuncExample(va_list *pva);   /*定义一个函数，需要上级函数传递一个va_list型变量的指针*/


/*我们可以通过宏va_start()告知函数准备从堆栈中取数据。其中，使用va_start()需要传递
两个参数，分别是va_list变量以及函数参数列表中“...”左边的第一个形参的名称，例如：*/

va_start(Example,pString);        /*告知函数准备从可变参数列表Example中取数据*/

/*与va_start()对应，我们可以通过宏va_end()告知函数不再继续进行参数的提取，例如；*/

va_end(Example);        /*结束参数提取*/

/*在va_start()和va_end()所划定的范围内，我们可以通过va_art()依次提取所需要的参数，
其中提取参数的顺序和调用时传递的参数顺序相同，例如：*/

uinsigned int A = va_arg(Example,unsigned int);     /*提取一个unsigned int 型的数据*/

/*也可以通过va_copy作为当前的参数列表做一个备份（备份当前的参数读取位置），例如：*/
/*保存当前的参数栈*/
va_list ExampleB;                       /*定义一个新的可变参数列表*/
va_copy(ExampleB,Example);    /*复制当前的参数栈信息到ExampleB*/


/*综合演示*/

/*该范例用于实现向指定的设备输出的可变数量的字符串。我们首先需要利用函数指针构造一个
输出设备驱动函数表，将所有的输出设备以数组的形式组织在一起： */

/*定义输出设备的驱动函数原型*/
typedef void OUTPUT_DRV(unsigned char *pstr,va_list *pArg); 
/*注意这里不是定义函数指针，而是定义了一个函数原型*/

OUTPUT_DRV LCD_Drv;     /*定义了一个函数LCD_Drv()*/
OUTPUT_DRV PRN_Drv;     /*定义了一个函数PRN_Drv()*/

/*定义指向OUTPUT_DRV类型函数的函数指针*/
typedef OUTPUT_DRV *P_DRV;

/*使用函数指针构造一个驱动函数表*/
P_DRV OutputDrivers[] = {&LCD_Drv,&PRN_Drv};


/*接下来我们将通过可变参数实现一个向指定设备输出类似printf格式字符串的函数，具体的设备
需要用户通过字符串的形式给出，例如“LCD”或者“PRN”。该函数将根据用户输入的字符串决定输出
的设备和字符串：*/

#include <stdarg.h>
#include <string.h>

/*定义输出设备驱动函数的原型*/
int Print(unsigned char *DrvNAME,...)
{
    unsigned char *pstr = NULL;
    P_DRV fnDrv = NULL;
    va_list Arg;        /*定义可变参数列表*/
    
    if(NULL == DrvNAME) {
        return -1;
    }
    
    /*确定使用哪个设备进行输出*/
    if(strcmp(DrvNAME,"LCD") == 0) {
        fnDrv = OutputDrivers[0];   /*使用LCD驱动*/
    } else if(strcmp(DrvNAME,"PRN") == 0) {
        fnDrv = OutputDrivers[1];   /*使用打印机驱动*/
    } else {
        return -1;
    }
    
    va_start(Arg,DrvNAME);              /*开始取参数*/
    pstr = va_arg(Arg,unsigned char *)  /*获取一个字符串*/
    fnDrv(pstr,&Arg);                   /*调用指定的设备驱动*/
    va_end(Arg);                        /*结束取参数*/
}


/*驱动函数实体*/
void LCD_Drv(unsigned char *pstr,va_list *pArg)
{
    ...
    /*  在函数中可以通过 va_arg(*pArg,类型)来依次提取参数，不需要
        通过va_end(*pArg)来标注取参数结束，如果通过va_copy生成了一
        个新的va_list变量，则需要在取出参数后通过va_end()将该变量
        关闭 **/
    ...
}

/**Print()的操作范例*/
unsigned char Day = 3;
Print("LCD","Is's the %dth day of this week.\n",Day);

