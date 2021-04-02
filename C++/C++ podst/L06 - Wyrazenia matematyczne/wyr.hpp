#include <string>
#include <vector>
#ifndef DEF_WYR
#define DEF_WYR


/// * ----- * ----- * ----- * ----- *


class wyrazenie {

public:
    virtual double oblicz() const =0;
    virtual std::string opis() const =0;

};

class operator1arg : public wyrazenie {

protected:
    wyrazenie *left;

public:
    operator1arg( wyrazenie *l );

};

class operator2arg : public operator1arg {

protected:
    wyrazenie *right;

public:
    operator2arg( wyrazenie *l, wyrazenie *r );

};

/// * ----- * ----- * ----- * ----- *


class liczba : public wyrazenie {

protected:
    double x;

public:
    liczba( double x );
    double oblicz() const override;
    std::string opis() const override;

};

class stala : public wyrazenie {
protected:
    const double x;
public:
    stala(double x);
    double oblicz() const override;
    std::string opis() const override;
};

class pi : public stala {
public:
    pi();
    std::string opis() const override;
};

class e : public stala {
public:
    e();
    std::string opis() const override;
};

class fi : public stala {
public:
    fi();
    std::string opis() const override;
};


class zmienna : public wyrazenie {

private:
    static std::vector< std::pair <std::string,double>> vec;
protected:
    std::string s;
public:
    static double get_val(std::string x);
    static void set_val(std::string x, double n);
    static void add_val(std::string s, double val);

    zmienna(std::string x);
    double oblicz() const override;
    std::string opis() const override;

};


class sin : public operator1arg {

public:
    sin(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};

class cos : public operator1arg {

public:
    cos(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};

class exp : public operator1arg {

public:
    exp(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};

class ln : public operator1arg {

public:
    ln(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};


class bezwzgl : public operator1arg {

public:
    bezwzgl(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};

class przeciw : public operator1arg {

public:
    przeciw(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};

class odwrot : public operator1arg {

public:
    odwrot(wyrazenie *w);
    double oblicz() const override;
    std::string opis() const override;
};



class dodaj : public operator2arg {

public:
    dodaj(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};


class odejmij : public operator2arg {

public:
    odejmij(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};


class mnoz : public operator2arg {

public:
    mnoz(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};

class dziel : public operator2arg {

public:
    dziel(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};


class logarytm : public operator2arg {

public:
    logarytm(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};

class potega : public operator2arg {

public:
    potega(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};

class modulo : public operator2arg {

public:
    modulo(wyrazenie *w1, wyrazenie *w2);
    double oblicz() const override;
    std::string opis() const override;
};




#endif
