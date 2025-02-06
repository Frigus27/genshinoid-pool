# Genshinoid-Pool：一个模拟原神抽卡卡池的C++类库

当前版本：ver 0.0.1a

已实现：常驻池、角色UP池

未来：武器池（定轨系统等）、自选池、捕获明光等

在```g++ -std=c++17```下编译通过
# 使用方法
```c++
#include "pool.hpp"
using namespace Genshinoid;
using namespace Pool;
```
## 基本对象（用于抽取的）
使用Item结构体。其结构如下：
```c++
struct Item {
    string name;
    string description;
    ItemLevel level;
    Item(string Name, string Description, ItemLevel Level);
};
```
## 定义一个卡池
卡池分为常驻池、UP池及（将来实现的）武器池。
概率参考：[原神抽卡概率工具表](https://www.bilibili.com/read/cv12616453)
### 常驻池
使用如下构造函数定义：
```c++
ConstantPool(ConstantPool::ItemList newItems);
```
其中
```c++
using ConstantPool::ItemList = array<vector<Item>, int(ItemLevel::Size)>;
// 数组第一项为三星池，第二项为四星池，第三项为五星池
enum class ItemLevel {
    Third = 0,
    Fourth = 1,
    Fifth = 2,
    Size = 3
};
```

### UP池
通过如下构造函数定义：
```c++
UpPool(ItemList newItems, ConstantPool constantPool);
```
其中```ItemList```同上，```ConstantPool constantPool```表示该UP池链接到的常驻池（用来出三星、四星及五星歪的角色）
## 使用示例：《抽 申 鹤》
见```example/shenhe.cpp```。后者是一个完整示例。
