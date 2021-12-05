#ifndef FRIGUS27_GENSHINOID_POOL_HPP
#define FRIGUS27_GENSHINOID_POOL_HPP

#include <string>
#include <array>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>

namespace Genshinoid {
    using std::string;
    using std::array;
    using std::vector;
    using std::mt19937;
    using std::uniform_int_distribution;
    using std::random_device;
    using std::max;

    enum class ItemLevel {
        Third = 0,
        Fourth = 1,
        Fifth = 2,
        Size = 3
    };

    struct Item {
        string name;
        string description;
        ItemLevel level;
        Item(string Name, string Description, ItemLevel Level) : name(Name), description(Description), level(Level) {}
    };

    namespace Pool {
        class RNG {
            private:
            uniform_int_distribution<int> disbool;
            mt19937 gen;

            public:
            RNG() {
                disbool = uniform_int_distribution<int>(1, 1000);
                gen = mt19937(random_device()());
            }

            bool operator()(float possibilityPercentage) {
                int freq = int(possibilityPercentage * 10);
                return disbool(gen) <= freq;
            }

            int operator()(int minimum, int maximum) {
                return uniform_int_distribution<int>(minimum, maximum)(gen);
            }
        };

        class BasePool {
            public:
            using ItemList = array<vector<Item>, 3>;
            using PossibilityList = array<float, 3>;
            using LastPickInterval = array<int, 3>;

            protected:
            ItemList items;
            PossibilityList possibilities;
            static RNG random;

            public:
            virtual PossibilityList getPossibilities() {
                return possibilities;
            }

            virtual float getPossibility(ItemLevel level) {
                return possibilities[int(level)];
            }

            virtual void setPossibilities(PossibilityList newPossibilities) {
                possibilities = newPossibilities;
            }

            virtual void setPossibility(ItemLevel level, float newPossibility) {
                possibilities[int(level)] = newPossibility;
            }

            public:
            virtual Item pickCard(ItemLevel level) {
                return items[int(level)][random(0, items[int(level)].size() - 1)];
            }

            virtual ItemLevel pickLevel() {
                if (random(getPossibility(ItemLevel::Fifth))) {
                    return ItemLevel::Fifth;
                }
                else if (random(getPossibility(ItemLevel::Fourth))) {
                    return ItemLevel::Fourth;
                }
                else {
                    return ItemLevel::Third;
                }
            }

            virtual Item pick() {
                return pickCard(pickLevel());
            }

            public:
            BasePool() : items(ItemList()) {}
            BasePool(ItemList newItems) : items(newItems) {}
        };

        RNG BasePool::random = RNG();

        class ConstantPool : public BasePool {
            protected:
            static LastPickInterval lastPickInterval;

            public:
            void calculatePossibility() {
                possibilities[int(ItemLevel::Fifth)] = max(0.6, 0.6 + 6 * (lastPickInterval[int(ItemLevel::Fifth)] - 73));
                possibilities[int(ItemLevel::Fourth)] = max(5.1, 5.1 + 50 * (lastPickInterval[int(ItemLevel::Fourth)] - 8));
                possibilities[int(ItemLevel::Third)] = 1 - possibilities[int(ItemLevel::Fourth)] - possibilities[int(ItemLevel::Fifth)];
            }

            ItemLevel pickLevel() {
                if (random(getPossibility(ItemLevel::Fifth))) {
                    lastPickInterval[int(ItemLevel::Fifth)] = 1;
                    lastPickInterval[int(ItemLevel::Fourth)] ++;
                    calculatePossibility();
                    return ItemLevel::Fifth;
                }
                else if (random(getPossibility(ItemLevel::Fourth))) {
                    lastPickInterval[int(ItemLevel::Fifth)] ++;
                    lastPickInterval[int(ItemLevel::Fourth)] = 1;
                    calculatePossibility();
                    return ItemLevel::Fourth;
                }
                else {
                    lastPickInterval[int(ItemLevel::Fifth)] ++;
                    lastPickInterval[int(ItemLevel::Fourth)] ++;
                    calculatePossibility();
                    return ItemLevel::Third;
                }
            }

            public:
            ConstantPool(ItemList newItems) {
                items = newItems;
                calculatePossibility();
            }

            ConstantPool() {
                items = ItemList();
                calculatePossibility();
            }

            Item pick() {
                return pickCard(pickLevel());
            }
        };

        class UpPool : public BasePool {
            protected:
            static LastPickInterval lastPickInterval;
            static bool isNecessaryGuarantee;
            ConstantPool linkedConstantPool;

            public:
            void calculatePossibility() {
                possibilities[int(ItemLevel::Fifth)] = max(0.6, 0.6 + 6 * (lastPickInterval[int(ItemLevel::Fifth)] - 73));
                possibilities[int(ItemLevel::Fourth)] = max(5.1, 5.1 + 50 * (lastPickInterval[int(ItemLevel::Fourth)] - 8));
                possibilities[int(ItemLevel::Third)] = 1 - possibilities[int(ItemLevel::Fourth)] - possibilities[int(ItemLevel::Fifth)];
            }

            ItemLevel pickLevel() {
                if (random(getPossibility(ItemLevel::Fifth))) {
                    lastPickInterval[int(ItemLevel::Fifth)] = 1;
                    lastPickInterval[int(ItemLevel::Fourth)] ++;
                    calculatePossibility();
                    return ItemLevel::Fifth;
                }
                else if (random(getPossibility(ItemLevel::Fourth))) {
                    lastPickInterval[int(ItemLevel::Fifth)] ++;
                    lastPickInterval[int(ItemLevel::Fourth)] = 1;
                    calculatePossibility();
                    return ItemLevel::Fourth;
                }
                else {
                    lastPickInterval[int(ItemLevel::Fifth)] ++;
                    lastPickInterval[int(ItemLevel::Fourth)] ++;
                    calculatePossibility();
                    return ItemLevel::Third;
                }
            }

            public:
            UpPool(ItemList newItems, ConstantPool constantPool) {
                items = newItems;
                linkedConstantPool = constantPool;
                calculatePossibility();
            }

            virtual Item pick() {
                ItemLevel levelResult = pickLevel();
                if (levelResult == ItemLevel::Fifth) {
                    if (!isNecessaryGuarantee) {
                        if (!random(50)) {          // 小保底歪了
                            isNecessaryGuarantee = true;
                            return linkedConstantPool.pickCard(ItemLevel::Fifth);
                        }
                        else {
                            return pickCard(ItemLevel::Fifth);
                        }
                    }
                    else {
                        isNecessaryGuarantee = false;
                        return pickCard(ItemLevel::Fifth);
                    }
                }
                else if (levelResult == ItemLevel::Fourth) {
                    if (!random(50)) {          // 四星在常驻
                        return linkedConstantPool.pickCard(ItemLevel::Fourth);
                    }
                    else {
                        return pickCard(ItemLevel::Fourth);
                    }
                }
                else {
                    return linkedConstantPool.pickCard(ItemLevel::Third);
                }
            }
        };

        BasePool::LastPickInterval ConstantPool::lastPickInterval = {1, 1, 1};
        BasePool::LastPickInterval UpPool::lastPickInterval = {1, 1, 1};
        bool UpPool::isNecessaryGuarantee = false;

        class ArmourPool: public BasePool {

        };

        

    }
}

#endif