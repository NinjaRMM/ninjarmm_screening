#include <cstdio>
#include <memory>

//--------------------------------------
class Base {
public:
    int getA() const { return a; };
    int a {1};

protected:
    int b {2};
    int getB() const { return b; }

private:
    int c {3};
    int getC() const { return c;  }
};

//--------------------------------------
class PublicDerived : public Base {
public:
    PublicDerived() {
        printf("PublicDerived:\n");
        printf(" - Can access to public Base: %d (and remains public)\n", a + getA());
        printf(" - Can access to protected Base: %d (and remains protected)\n", b + getB());
        //printf(" - Cannot access to private Base: %d (because remains private)\n", c + getC());    // Error
    }
};

//--------------------------------------
class ProtectedDerived : protected Base {
public:
    ProtectedDerived() {
        printf("ProtectedDerived:\n");
        printf(" - Can access to public Base: %d (but becomes protected)\n", a + getA());
        printf(" - Can access to protected Base: %d (and remains protected)\n", b + getB());
        //printf(" - Cannot access to private Base: %d (because remains private)\n", c + getC());    // Error
    }
};

//--------------------------------------
class PrivateDerived : private Base {
public:
    PrivateDerived() {
        printf("PrivateDerived:\n");
        printf(" - Can access to public Base: %d (but becomes private)\n", a + getA());
        printf(" - Can access to protected Base: %d (but becomes private)\n", b + getB());
        //printf(" - Cannot access to private Base: %d (because remains private)\n", c + getC());    // Error
    }
};

//--------------------------------------
class Composition {
public:
    const Base &getBase() const { return base;  }

protected:
    Base base;
};

//--------------------------------------
class Aggregation {
public:
    void                        setBase(std::shared_ptr<Base> base) { mBase = base; }
    const std::shared_ptr<Base> getBase() const                     { return mBase; }

protected:
    std::shared_ptr<Base> mBase;    // Something else will give us a Base instance
};

//--------------------------------------
int
main() {
    PublicDerived           pub;
    ProtectedDerived        pro;
    PrivateDerived          pri;
    Composition             com;
    Aggregation             agg;
    std::shared_ptr<Base>   base = std::make_shared<Base>();


    agg.setBase(base);

    int trash{};

    trash = pub.a + pub.getA(); // Can Access to public data and methods 
    //trash += pub.b + pub.getB(); // Cannot access protected data and methods 
    //trash += pub.c + pub.getC(); // Cannot access private data and methods 

    // Nothing can be accessed from the Base class because the data and members are now protected.
    //trash += pro.a + pro.getA();
    //trash += pro.b + pro.getB();
    //trash += pro.c + pro.getC();

    // Nothing can be accessed from the Base class because the data and members are now private.
    //trash += pri.a + pri.getA();
    //trash += pri.b + pri.getB();
    //trash += pri.c + pri.getC();

    trash += com.getBase().a + com.getBase().getA();
    if (agg.getBase()) {
        trash += agg.getBase()->a + agg.getBase()->getA();
    }

    return trash;

}