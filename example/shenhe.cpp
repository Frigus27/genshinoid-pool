#include "pool.hpp"
#include "color.hpp"
#include <iostream>
#include <windows.h>

using namespace std;
using namespace Genshinoid;
using namespace Pool;

int thecount = 1;
int tencount = 1;
int shenhecount = 0;

ConstantPool cp = ConstantPool(ConstantPool::ItemList({
    vector<Item>{
        Item("魔导绪论", "假书", ItemLevel::Third),
        Item("黑缨枪", "假枪", ItemLevel::Third),
        Item("以理服人", "假大剑", ItemLevel::Third),
        Item("沐浴龙血的剑", "假大剑", ItemLevel::Third),
        Item("旅行剑", "假单手剑", ItemLevel::Third),
    },
    vector<Item>{
        Item("砂糖", "究极减抗人", ItemLevel::Fourth),
        Item("诺艾尔", "高 达", ItemLevel::Fourth),
        Item("香菱", "璃月第一大厨", ItemLevel::Fourth),
        Item("芭芭拉", "冲芭芭拉呀", ItemLevel::Fourth),
        Item("钟剑", "最 好 的 大 剑", ItemLevel::Fourth),
        Item("笛剑", "好好好好好", ItemLevel::Fourth),
        Item("白影剑", "这种东西不知道有什么用", ItemLevel::Fourth),
        Item("匣里灭尘", "比护摩好的枪", ItemLevel::Fourth),
        Item("西风烈弓", "比终末好的弓", ItemLevel::Fourth),
    },
    vector<Item>{
        Item("刻晴", "璃月七星中的老婆", ItemLevel::Fifth),
        Item("七七", "七七人称小神里", ItemLevel::Fifth),
        Item("迪卢克", "高贵的火系大剑", ItemLevel::Fifth),
        Item("琴", "高贵的琴团长", ItemLevel::Fifth),
        Item("莫娜", "揭不开锅的占星术士", ItemLevel::Fifth)
    }
}));

UpPool upPool = UpPool(UpPool::ItemList({
    vector<Item> {},
    vector<Item>{
        Item("烟绯", "璃月罗翔", ItemLevel::Fourth),
        Item("云瑾", "戏腔yyds", ItemLevel::Fourth),
        Item("重云", "纯 阳 之 体", ItemLevel::Fourth)
    },
    vector<Item>{
        Item("申鹤", "留云借风真君的弟子", ItemLevel::Fifth)
    }
}), cp);

void pickACard(BasePool * pool) {
    cout << endl;
    setColor(ColorID::White);
    cout << "第" << thecount << "发" << endl;
    cout << "按回车键开始抽卡。" << endl;
    cin.get();
    Item card = pool->pick();
    if (card.level == ItemLevel::Third) {
        setColor(ColorID::Celeste);
        cout << "一道蓝光划过天边。" << endl;
        cout << "抽到了" << card.name << "。"<< endl << card.description << endl;
    }
    else if (card.level == ItemLevel::Fourth) {
        setColor(ColorID::Purple);
        cout << "一道紫光划过天边，让过路的人不禁为之驻足。" << endl;
        Sleep(500);
        cout << "抽到了" << card.name << "！"<< endl << card.description << endl;
    }
    if (card.level == ItemLevel::Fifth) {
        setColor(ColorID::Yellow);
        cout << "一道金光划过天边，光芒刺破了天空。" << endl;
        Sleep(2000);
        cout << "抽到了" << card.name << "！"<< endl << card.description << endl;
        //thecount = 0;
        if (card.name == "申鹤") {
            cout << "申鹤" << shenhecount << "命" << endl;
            shenhecount ++;
        }
    }
    thecount ++;
}

void pick10Cards(BasePool * pool) {
    cout << endl;
    setColor(ColorID::White);
    cout << "第" << tencount << "个十连" << endl;
    tencount ++;
    cout << "按回车键开始抽卡。" << endl;
    cin.get();
    bool hasGolden = false;
    bool hasshenhe = false;
    vector<Item> theitems = vector<Item>();
    for (int i = 0; i < 10; i ++) {
        auto item = pool -> pick();
        thecount ++;
        if (item.level == ItemLevel::Fifth) {
            hasGolden = true;
            if (item.name == "申鹤") {
                hasshenhe = true;
                shenhecount ++;
            }
        }
        theitems.push_back(item);
    }
    if (hasGolden) {
        setColor(ColorID::Yellow);
        cout << "一道金光划过天边，光芒刺破了天空。" << endl;
        Sleep(2000);
    }
    else {
        setColor(ColorID::Purple);
        cout << "一道紫光划过天边，让过路的人不禁为之驻足。" << endl;
        Sleep(500);
    }
    cout << "抽到了：";
    for (auto eachItem : theitems) {
        if (eachItem.level == ItemLevel::Fifth) {
            setColor(ColorID::Yellow);
        }
        else if (eachItem.level == ItemLevel::Fourth) {
            setColor(ColorID::Purple);
        }
        else if (eachItem.level == ItemLevel::Third) {
            setColor(ColorID::Celeste);
        }
        cout << eachItem.name << " ";
    }
    cout << endl;
    if (hasshenhe) {
        setColor(ColorID::Yellow);
        cout << "申鹤" << shenhecount - 1 << "命" << endl;
    }
}

int main() {
    cout << "抽 申 鹤" << endl;
    cout << "看你要多少发才能抽满命？" << endl << endl;
    while (shenhecount != 7) {
        pick10Cards(&upPool);
    }
    setColor(ColorID::White);
    cout << "共抽了" << thecount << "发，" << tencount << "个十连，共" << int(tencount / 4 + 0.99) << "个648。" << endl;
    if (thecount <= 100) {
        cout << "趁现在的日子，吃点好的吧。" << endl;
    }
    else if (thecount <= 300) {
        cout << "你是什么欧煌？" << endl;
    }
    else if (thecount <= 800) {
        cout << "中规中矩，还行" << endl;
    }
    else  {
        cout << "太非了8？" << endl;
    }
    cout << "按x键退出" << endl;
    while (cin.get() != 'x') {}
}