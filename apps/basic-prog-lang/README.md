# BASIC

大学期间（2009年）的作品，学院老师筹划写“C语言教程”时，需要一个例子作为综合实验，因此我帮忙用纯ANSI C写一个 BASIC 解释器。设计思路当年也整理成了博客文章：[《BASIC解释器》](http://zzp.me/archive/basic解释器.html)。

因为文章里和后续出版的书中都没有完整的代码（按照老师的说法，不希望同学们直接Ctrl-C Ctrl-V），因此有不少同学发邮件向我询问完整的代码，所以干脆就发布到Github上供大家自由下载。

# 下载编译

```
git clone https://github.com/redraiment/basic.git
cd basic
cc -o basic src/*.c
```

`scripts`目录下有一些简单的实例，可供参考。

# BASIC语法

本BASIC解释器支持的语法主要参考高中时玩的步步高里的GVBASIC。

## 行号

与GVBASIC相同，每一行都必须有一个“整数行号”+一行“代码”组成，例如：

```
0001 PRINT "HELLO WORLD"
```

为了方便对齐，行号可以有任意多个前导0；行号之后必须加上一个空格。

**注意**：本解释器最多支持10000行代码（可自行修改代码以支持更多行数）。

## 变量

与GVBASIC相同，本BASIC解释器也只支持固定26个变量，且变量名必须为单个大写字母（`A`到`Z`）。

变量为动态、弱类型。

**变量赋值**

```
[LET] <VAR> = <EXPRESSION>
```

计算等式右侧的表达式，并将结果赋值到左侧变量中。其中`LET`关键字可选。

## 输入输出

**输入**

```
INPUT <VAR>[, VAR ...]
```

从标准输入（通常是键盘）中输入任意多个数据到变量中。

**输出**

```
PRINT <EXPRESSION>[, EXPRESSION ...]
```

计算表达式`<EXPRESSION>`，并将结果输出到标准输出（通常是屏幕）。

## 表达式运算

**算数运算符**

* `+`：加法
* `-`：减法
* `*`：乘法
* `/`：除法
* `%`：求余
* `()`：小括号提高优先级

**比较运算符**

* `=`：相等
* `<>`：不等
* `<`：小于
* `>`：大于
* `<=`：不大于
* `>=`：不小于

**逻辑运算符**

* `AND`：与
* `OR`：或
* `NOT`：非

## 控制语句

**条件语句**

```
IF <EXPRESSION> THEN
  <SENTENCE>
  ...
[ELSE
  <SENTENCE>
  ...]
END IF
```

**FOR循环语句**

```
FOR <VAR> = <EXPRESSION> TO <EXPRESSION> [STEP <EXPRESSION>]
  [SENTENCE]
  ...
NEXT
```

* 表达式1为起点
* 表达式2为终点
* 步长表达式可选

**WHILE循环语句**

```
WHILE <EXPRESSION>
  [SENTENCE]
  ...
WEND
```

执行循环代码块，直到表达式的结果为FALSE。

**跳转语句**

```
GOTO <LINE-NUMBER>
```

无条件跳转到指定的行号。
