using DIEngine;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class ServiceLocatorTests
    {
        [TestMethod]
        public void ServiceLocatorUsageTest()
        {
            // main
            SimpleContainer c = new SimpleContainer();
            c.RegisterInstance<int>(1);
            ServiceLocator.SetContainerProvider(() => c);

            // another context
            int res = DoSth();
            Assert.AreEqual(1, res);
        }

        int DoSth()
        {
            // używamy lokatora
            return ServiceLocator.Current.GetInstance<int>();
        }

        [TestMethod]
        public void SwitchServiceLocatorTest()
        {
            // main
            SimpleContainer c1 = new SimpleContainer();
            c1.RegisterInstance<int>(1);

            ServiceLocator.SetContainerProvider(() => c1);
            int res = DoSthAndSwitch();
            Assert.AreEqual(2, res);

            res = ServiceLocator.Current.GetInstance<int>();
            Assert.AreEqual(2, res);

            res = DoSth();
            Assert.AreEqual(2, res);

            SimpleContainer c3 = new SimpleContainer();
            c3.RegisterInstance<int>(3);
            ServiceLocator.SetContainerProvider(() => c3);

            res = DoSth();
            Assert.AreEqual(3, res);

            res = DoSthAndSwitch();
            Assert.AreEqual(2, res);

        }

        int DoSthAndSwitch()
        {
            // set another container
            SimpleContainer c2 = new SimpleContainer();
            c2.RegisterInstance<int>(2);
            ServiceLocator.SetContainerProvider(() => c2);
            
            // return int from new container
            return ServiceLocator.Current.GetInstance<int>();
        }

        [TestMethod]
        public void GetSimpleContainerTest()
        {
            SimpleContainer c = new SimpleContainer();
            ServiceLocator.SetContainerProvider(() => c);

            var o = ServiceLocator.Current.GetInstance<SimpleContainer>();
            Assert.AreEqual(c, o);
        }
    }
}
