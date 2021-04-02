using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.Mvc;
using System.Web.Security;
using System.Web.Mvc.Html;

namespace ShopMVC.Code
{
    public static partial class MyHtmlHelper
    {
        public static MvcHtmlString BuildBreadcrumbs(this HtmlHelper htmlHelper)
        {
            var route = HttpContext.Current.Request.RequestContext.RouteData;
            var breadcrumbs = new StringBuilder();
            breadcrumbs.Append($"<a href=\"/Start/Index\">ShopMVC</a>");
            var url = "";

            foreach (var pair in route.Values)
            {
                url += "/" + pair.Value;
                breadcrumbs.Append($" > <a href=\"{url}\">{pair.Value}</a>");
            }

            return MvcHtmlString.Create(breadcrumbs.ToString());
        }

        public static MvcHtmlString LinkButton(this HtmlHelper htmlHelper, string text, string url)
        {
            var button = $"<input type=\"button\" onclick=\"location.href='{url}';\" value='{text}' />";

            return MvcHtmlString.Create(button);
        }

        public static MvcHtmlString UserInfo(this HtmlHelper htmlHelper)
        {
            string res = "<div style=\"position: absolute; right: 10px; top: 10px;\" >";

            if (HttpContext.Current.User.Identity.IsAuthenticated)
            {
                res += $"<div>Zalogowano jako: {HttpContext.Current.User.Identity.Name}</div>";
                res += "<div>" + LinkExtensions.ActionLink(htmlHelper,"Wyloguj", "Logout", "Account") + "</div>";
            }

            else
            {
                res += "<div>" + LinkExtensions.ActionLink(htmlHelper, "Zaloguj", "Login", "Account") + "</div>";
            }

            res += "</div>";

            return MvcHtmlString.Create(res);
        }


    }
}