#include "tritset.h"

Tritset::Tritset(size_t size) {
    std::vector<uint> tmv(size/4/sizeof(uint)+1, 0);
    for(int i = 0; i < size/4/sizeof(uint)+1; i++) tmv[i] = 0;
    trits = tmv;
    capacity = ((size-1)/4/sizeof(uint)+1)* sizeof(uint) *4;
    this->size = size;
}

Tritset::Reference Tritset::operator[](size_t pos) {
    Tritset::Reference ref(this, pos);
    return ref;
}

Trit Tritset::operator[](size_t pos) const {
    uint cur = trits[pos/4/sizeof(uint)];
    char posu = 4*sizeof(uint) - pos % (4*sizeof(uint));
    cur = (cur & ((uint)3 << posu*2)) >> posu*2;
    if(cur == 0) return Unknown;
    if(cur == 1) return False;
    if(cur == 2) return True;
    return Unknown;
}

Tritset& Tritset::operator&=(const Tritset &tset) {
    for(size_t i = 0; i < this->size; i++) (*this)[i] &= tset[i];
    return *this;
}

Tritset operator& (const Tritset &left, const Tritset &tset) {
    Tritset res((left.Size() > tset.Size()) ? left : tset);
    return res&=((left.Size() < tset.Size()) ? left : tset);
}

Tritset& Tritset::operator|=(const Tritset &tset) {
    for(size_t i = 0; i < this->size; i++) (*this)[i] |= tset[i];
    return *this;
}

Tritset operator| (const Tritset &left, const Tritset &tset) {
    Tritset res((left.Size() > tset.Size()) ? left : tset);
    return res|=((left.Size() < tset.Size()) ? left : tset);
}

Tritset Tritset::operator~() {
    for(size_t i = 0; i < capacity; i++) (*this)[i] = ~(*this)[i];
    return *this;
}

size_t Tritset::Capacity() const {
    return capacity;
}

size_t Tritset::Length() const{
    size_t res = 0;
    for(size_t i = 0; i < capacity; i++) {
        if((*this)[i] != Unknown) res = i;
    }
    if(res > 0) res++;
    return res;
}

size_t Tritset::Size() const {
    return size;
}

size_t Tritset::Cardinality(const Trit value) const {
    size_t res = 0;
    for(size_t i = 0; i < Length(); i++) if((*this)[i] == value) res++;
    return res;
}


void Tritset::Print() const {
    for(size_t i = 0; i < capacity; i++) {
        if((*this)[i] == False) std::cout << "F";
        if((*this)[i] == Unknown) std::cout << "U";
        if((*this)[i] == True) std::cout << "T";
    }
}

void Tritset::Print(size_t pos) const {
    if((*this)[pos] == False) std::cout << "F";
    if((*this)[pos] == Unknown) std::cout << "U";
    if((*this)[pos] == True) std::cout << "T";
}

void Tritset::Resize(size_t nsize) {
    trits.resize((nsize-1)/4/sizeof(uint)+1, 0);
    size = nsize;
    capacity = ((nsize-1)/4/sizeof(uint)+1)* sizeof(uint) *4;
}

void Tritset::Shrink() {
    Resize(Length());
}

bool Tritset::Reference::operator==(Trit t) const {
    return this->Get() == t;
}


bool Tritset::Reference::operator!=(Trit t) const {
    return this->Get() != t;
}


Tritset::Reference::Reference(Tritset *sett, size_t pos) {
    set = sett;
    trit_pos = pos;
}

Trit Tritset::Reference::Get() const{
    uint cur = set->trits[trit_pos/4/sizeof(uint)];
    char posu = 4*sizeof(uint) - trit_pos % (4*sizeof(uint));
    cur = cur >> posu*2;
    return (Trit)(cur % 4);
}

Tritset::Reference Tritset::begin(){
    Tritset::Reference ref(this, 0);
    return ref;
}
Tritset::Reference Tritset::end(){
    Tritset::Reference ref(this, Length());
    return ref;
}

Tritset::Reference& Tritset::Reference::operator=(Trit x) {
    if(x != Unknown) {
        set->trits.resize(((trit_pos) / 4 / sizeof(uint) + 1), 0);
        if (trit_pos >= set->capacity) set->capacity = ((trit_pos) / 4 / sizeof(uint) + 1) * 4 * sizeof(uint);
    }
    uint cur = set->trits[trit_pos/4/sizeof(uint)];
    char posu = 4*sizeof(uint) - trit_pos % (4*sizeof(uint));
    cur = cur & ~((uint)3 << posu*2) | ((uint)0 | ((uint)x << (posu*2)));
    set->trits[trit_pos/4/sizeof(uint)] = cur;
    return *this;
}


Tritset::Reference& Tritset::Reference::operator=(const Tritset::Reference &x) {
    *this = (Trit)x;
    return *this;
}

Tritset::Reference& Tritset::Reference::operator&=(Trit t) {
    Trit res = Unknown;
    if(Get() == False) res = False;
    if(Get() == Unknown) {
        if(t != False) res = Unknown;
        else res = False;
    }
    if(Get() == True) res = t;
    uint cur = set->trits[trit_pos/4/sizeof(uint)];
    char posu = 4 * sizeof(uint) - trit_pos % (4*sizeof(uint));
    cur = ( cur & ~((uint)3 << posu*2) ) | ((uint)0 | ((uint)res << (posu*2)));
    set->trits[trit_pos/4/sizeof(uint)] = cur;
    return *this;
}


Tritset::Reference& Tritset::Reference::operator|=(Trit t) {
    Trit res = Unknown;
    if(Get() == True) res = True;
    if(Get() == Unknown) {
        if(t != True) res = Unknown;
        else res = True;
    }
    if(Get() == False) res = t;
    uint cur = set->trits[trit_pos/4/sizeof(uint)];
    char posu = 4*sizeof(uint) - trit_pos % (4*sizeof(uint));
    cur = cur & ~((uint)3 << posu*2) | ((uint)0 | ((uint)res << (posu*2)));
    set->trits[trit_pos/4/sizeof(uint)] = cur;
    return *this;
}

Tritset::Reference::operator Trit() const {
    return Get();
}

Trit operator| (Trit l, Trit r) {
    if(l == True || r == True) return True;
    if(l == False && r == False) return False;
    return Unknown;
}

Trit operator& (Trit l, Trit r) {
    if(l == False || r == False) return False;
    if(l == True && r == True) return True;
    return Unknown;
}

Trit operator~ (Trit t) {
    if(t == True) return False;
    if(t == False) return True;
    return Unknown;
}