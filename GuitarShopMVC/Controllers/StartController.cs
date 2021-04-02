using ShopMVC.Code;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Linq.Dynamic;
using ShopMVC.Models.Start;

namespace ShopMVC.Controllers
{

    public class StartController : Controller
    {

        [HttpGet]
        public ActionResult Index(IndexModel model, int page=1)
        {
            if (Int32.TryParse(model.ValueFrom, out int resFrom))
            {
                if (resFrom <= 0) model.ValueFrom = "";
            }
            else
            {
                model.ValueFrom = "";
            }

            if (Int32.TryParse(model.ValueTo, out int resTo))
            {
                if (resTo <= 0) model.ValueTo = "";
            }
            else
            {
                model.ValueTo = "";
            }


            MyPagerInfo info = ShopItems.Instance.Retrieve(model, page, 5);
            ViewBag.Pager = info;
            
            return View(model);
        }

        [Authorize(Roles = "admin")]
        [HttpGet]
        public ActionResult Edit(ShopItemModel model, int id)
        {
            model = ShopItems.Instance.GetModel(id);
            return View(model);
        }

        [Authorize(Roles = "admin")]
        [HttpGet]
        public ActionResult Regenerate()
        {
            ShopItems.Instance.GenerateDatabase(1000);
            return Redirect("/Start/Index");
        }


        [Authorize(Roles = "admin")]
        [HttpPost]
        public ActionResult Edit(ShopItemModel model)
        {
            var valid = true;

            if (valid)
            {
                ShopItems.Instance.UpdateItem(model);
                return Redirect("/Start/Index");
            }

            ViewBag.ErrorMessage = "Popraw błędy.";
            return View(model);
        }

        [Authorize(Roles = "admin")]
        [HttpGet]
        public ActionResult Delete(ShopItemModel model, int id)
        {
            model = ShopItems.Instance.GetModel(id);
            return View(model);
        }

        [Authorize(Roles = "admin")]
        [HttpPost]
        public ActionResult Delete(int ID)
        {
            ShopItems.Instance.DeleteItem(ID);
            return Redirect("/Start/Index");
        }

        [Authorize(Roles = "admin")]
        [HttpGet]
        public ActionResult Create()
        {
            var model = new ShopItemModel();
            return View(model);
        }

        [Authorize(Roles = "admin")]
        [HttpPost]
        public ActionResult Create(ShopItemModel model)
        {
            var valid = true;
            if (model.Name == null || model.Value < 0)
                valid = false;

            if (valid)
            {
                model.ID = ShopItems.Instance.GetFreeID();
                ShopItems.Instance.CreateItem(model);
                return Redirect("/Start/Index");
            }

            ViewBag.ErrorMessage = "Popraw błędy!";
            return View(model);

        }

    }
}