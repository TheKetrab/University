using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using ShopMVC.Code;

namespace ShopMVC.Controllers
{
    public class ShoppingCartController : Controller
    {
        [HttpGet]
        public ActionResult Index()
        {
            return View();
        }

        [HttpGet]
        public ActionResult Add(int ID)
        {
            ShoppingCart.AddItem( ShopItems.Instance.GetModel(ID) );
            return Redirect("/Start/Index");
        }

        [HttpGet]
        public ActionResult Remove(int ID)
        {
            ShoppingCart.RemoveItem( ShopItems.Instance.GetModel(ID) );
            return Redirect("/ShoppingCart/Index");
        }
    }
}