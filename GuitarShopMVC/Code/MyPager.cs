using ShopMVC.Models.Start;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace ShopMVC.Code
{

    public class MyPagerInfo
    {
        public List<ShopItemModel> Items;
        public int PageNumber;
        public int ItemsPerPage;
        public int PagesCount;
    }

    public static partial class MyHtmlHelper
    {
        private static string linkToPage(int i, NameValueCollection qs)
        {
            var res = $"/Start/Index?Page={i}";
            foreach (string key in qs)
            {
                if (key == null) break;

                // if not PAGE then add
                if (!key.ToUpper().Equals("PAGE")) {
                    res += $"&{key}={qs[key]}";
                }
            }

            return res;
        }

        public static MvcHtmlString MyPager(this HtmlHelper htmlHelper, int PageNumber, int PagesCount, NameValueCollection qs)
        {
            
            var pagerHtml = "<div>";

            if (PagesCount > 5)
            {
                if (PageNumber >= 3 && PageNumber <= PagesCount-2)
                {
                    pagerHtml += $"<a href=\"{linkToPage(1, qs)}\"> << </a>";
                    pagerHtml += $"<a href=\"{linkToPage(PageNumber-1, qs)}\"> < </a>";
                    pagerHtml += "...";
                    for (int i=PageNumber-2; i<=PageNumber+2; i++)
                        pagerHtml += $"<a href=\"{linkToPage(i, qs)}\"> {i} </a>";

                    pagerHtml += "...";
                    pagerHtml += $"<a href=\"{linkToPage(PageNumber+1, qs)}\"> > </a>";
                    pagerHtml += $"<a href=\"{linkToPage(PagesCount, qs)}\"> >> </a>";

                }
                else if (PageNumber == 1 || PageNumber == 2) {

                    for (int i=1; i<=5; i++)
                        pagerHtml += $"<a href=\"{linkToPage(i, qs)}\"> {i} </a>";

                    pagerHtml += "...";
                    pagerHtml += $"<a href=\"{linkToPage(PageNumber+1, qs)}\"> > </a>";
                    pagerHtml += $"<a href=\"{linkToPage(PagesCount, qs)}\"> >> </a>";
                }
                else if (PageNumber == PagesCount-1 || PageNumber == PagesCount)
                {
                    pagerHtml += $"<a href=\"{linkToPage(1, qs)}\"> << </a>";
                    pagerHtml += $"<a href=\"{linkToPage(PageNumber-1, qs)}\"> < </a>";
                    pagerHtml += "...";

                    for (int i = 4; i >= 0; i--)
                        pagerHtml += $"<a href=\"{linkToPage(PagesCount - i, qs)}\"> {PagesCount-i} </a>";

                }
            }

            else
            {
                for (int i = 1; i <= PagesCount; i++)
                    pagerHtml += $"<a href=\"{linkToPage(i, qs)}\"> {i} </a>";

            }

            pagerHtml += "</div>";

            return MvcHtmlString.Create(pagerHtml);
        }
    }


}