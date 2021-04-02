using ShopMVC.Models.Account;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Security;

namespace ShopMVC.Controllers
{
    public class AccountController : Controller
    {

        [HttpGet]
        public ActionResult Login()
        {
            AccountModel model = new AccountModel();

            return View(model);
        }

        [HttpPost]
        public ActionResult Login(AccountModel model, string ReturnUrl)
        {
            if (this.ModelState.IsValid)
            {
                if (Membership.ValidateUser(model.UserName, model.Password))
                {
                    FormsAuthenticationTicket ticket = new FormsAuthenticationTicket(model.UserName, false, 20);

                    HttpCookie cookie = new HttpCookie(FormsAuthentication.FormsCookieName);
                    cookie.Value = FormsAuthentication.Encrypt(ticket);
                    this.Response.AppendCookie(cookie);

                    if (String.IsNullOrEmpty(ReturnUrl))
                    {
                        return RedirectToAction("Index","Start");
                    }
                    else
                    {
                        return Redirect(ReturnUrl);
                    }

                }
                else
                    ViewBag.ErrorMessage = "Niepoprawna nazwa użytkownika lub hasło.";
            }

            return View(model);
        }

        public ActionResult Logout()
        {
            FormsAuthentication.SignOut();
            return RedirectToAction("Index", "Start");
        }



    }
}