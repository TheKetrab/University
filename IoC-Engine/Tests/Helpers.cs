using DIEngine;

namespace Tests
{

    class Foo3 : Foo { public Foo3() { } }

    class Foo2 : Foo { public Foo2() { } }

    class Foo { }

    class Bar { }

    interface IBar { void DoSth(); }

    class ImplementedBar : IBar
    {
        private string prop;
        public ImplementedBar() { prop = "PROP"; }
        public void DoSth() { }
    }


    class Qux
    {
        string str;
        public Qux(string str) { this.str = str; }
        public string GetStr() { return str; }

    }

    class Baz1 { public Baz1(Baz2 b1, string str) { } }

    class Baz2 { public Baz2(int a, int b, Baz3 b3) { } }

    class Baz3 { public Baz3(string x) { } }


    class T1 { public T1(T2 o) { } }
    class T2 { public T2(T3 o) { } }

    class T3 { public T3(T4 o) { } }

    class T4 { public T4(T5 o) { } }

    class T5 { public T5(T6 o) { } }

    class T6 { public T6(T1 o) { } }

    class ProperCon
    {
        protected string msg;
        public ProperCon() { msg = "1"; }
        public ProperCon(int x) { msg = "2"; }
        public ProperCon(string a) { msg = "3"; }
        public ProperCon(int a, int b, int c) { msg = "4"; }
        public string GetMsg() {  return msg; }
    }

    class ProperCon1 : ProperCon
    {
        [DependencyConstructor]
        public ProperCon1() : base() { }
    }

    class ProperCon2 : ProperCon
    {
        public ProperCon2(int a, int b, int c) { this.msg = "AmbiguityCons1"; }
        public ProperCon2(int a, string b, int c) { this.msg = "AmbiguityCons2"; }
    }

    class ProperCon3 : ProperCon
    {
        public ProperCon3(int a, int b, int c) { this.msg = "AmbiguityCons1"; }
        public ProperCon3(int a, string b, int c) { this.msg = "AmbiguityCons2"; }

        [DependencyConstructor]
        public ProperCon3(string a, string b, string c) { this.msg = "DependencyCons"; }
    }


    class A
    {
        private D d;
        public A() { }
        public A(B b) { }
        // wstrzykiwanie przez właściwość
        [DependencyProperty]
        public C TheC { get; set; }
        // wstrzykiwanie przez metodę
        [DependencyMethod]
        public void setD(D d) { this.d = d; }
        public D getD() { return d; }
    }
    class B { }
    class C { }

    class D { }

}
