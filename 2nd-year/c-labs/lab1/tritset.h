#ifndef LAB1_TRITSET_H
#define LAB1_TRITSET_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <bits/unordered_map.h>

enum Trit {False = 1, Unknown = 0, True = 2};

Trit operator| (Trit l, Trit r);
Trit operator& (Trit l, Trit r);
Trit operator~ (Trit t);

class Tritset {
private:
    std::vector<uint> trits;
    size_t capacity;
    size_t size;
public:
    class Reference {
    private:
        friend class Tritset;
        Reference(Tritset* sett, size_t pos);
        Tritset *set;
        size_t trit_pos;
        Trit Get() const;
    public:
        Reference& operator= (Trit x);
        Reference& operator= (const Reference& x);
        operator Trit() const;
        bool operator== (Trit t) const;
        bool operator!= (Trit t) const;

        Reference& operator&= (Trit t);

        Reference& operator|= (Trit t);

    };
    class Iterator {
    private:
        friend class Tritset;
        Tritset *set;
        size_t trit_pos;
        Iterator(Tritset *sett, size_t pos) {
            set = sett;
            trit_pos = pos;
        }
    public:
        bool operator== (const Iterator& x) const {
            return (trit_pos == x.trit_pos);
        }
        bool operator!= (const Iterator& x) const {
            return (trit_pos != x.trit_pos);
        }
        void operator++ () {
            trit_pos++;
        }
        Reference operator* () {
            Tritset::Reference ref(set, trit_pos);
            return ref;
        }
    };
    Tritset(size_t size);

    Reference operator[] (size_t pos);
    Trit operator[] (size_t pos) const;
    Tritset& operator&= (const Tritset& tset);
    Tritset& operator|= (const Tritset& tset);
    Tritset operator~ ();

    size_t Length() const;
    size_t Capacity() const;
    size_t Size() const;
    size_t Cardinality(Trit value) const;
    void Resize(size_t nsize);
    void Shrink();
    void Print() const;
    void Print(size_t pos) const;
    Reference begin();
    Reference end();
};

Tritset operator& (const Tritset& left, const Tritset& tset);
Tritset operator| (const Tritset& left, const Tritset& tset);

#endif //LAB1_TRITSET_H
