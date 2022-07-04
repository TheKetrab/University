using DIEngine;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class DepMethodsPropsTests
    {
        [TestMethod]
        public void PropsAndMethodsTest()
        {
            // tworzy nową instancję A z B wstrzykniętym przez konstruktor,
            // C wstrzykniętym przez właściwość i D wstrzykniętym przez metodę.

            SimpleContainer c = new SimpleContainer();
            var singletonC = new C();
            var singletonD = new D();
            c.RegisterType<A>(false);
            c.RegisterType<B>(false);
            c.RegisterInstance<C>(singletonC);
            c.RegisterInstance<D>(singletonD);

            A a = c.Resolve<A>();
            Assert.IsTrue(a.TheC is C);
            Assert.IsTrue(a.getD() is D);
            Assert.AreEqual(singletonC, a.TheC);
            Assert.AreEqual(singletonD, a.getD());
        }

        [TestMethod]
        public void BuildUpTest()
        {
            SimpleContainer c = new SimpleContainer();
            var singletonC = new C();
            var singletonD = new D();
            c.RegisterType<A>(false);
            c.RegisterType<B>(false);
            c.RegisterInstance<C>(singletonC);
            c.RegisterInstance<D>(singletonD);

            // obiekt theA SKĄDŚ pochodzi, ale nie z kontenera
            A theA = new A();
            theA.TheC = new C(); // ma jakieś C
            theA.setD(new D()); // ma jakieś D

            Assert.AreNotEqual(singletonC, theA.TheC);
            Assert.AreNotEqual(singletonD, theA.getD());

            // wstrzykuje do theA zależności przez właściwości i metody
            c.BuildUp(theA);
            Assert.AreEqual(singletonC, theA.TheC);
            Assert.AreEqual(singletonD, theA.getD());

        }
    }
}
