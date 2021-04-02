using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using ShopMVC.Models.Start;

namespace ShopMVC.Code
{
    public class ShoppingCart
    {
        public static readonly string CartName = "SC_";

        public static bool AddItem(ShopItemModel item)
        {
            var arg = CartName + item.ID;
            if (HttpContext.Current.Session[arg] == null)
            {
                HttpContext.Current.Session[arg] = item;
                return true;
            }

            return false;
        }

        public static bool RemoveItem(ShopItemModel item)
        {
            var arg = CartName + item.ID;
            if (HttpContext.Current.Session[arg] != null)
            {
                HttpContext.Current.Session[arg] = null;
                return true;
            }

            return false;
        }

        public static List<ShopItemModel> GetContent()
        {
            var list = new List<ShopItemModel>();

            foreach (string key in HttpContext.Current.Session.Keys)
            {
                if (key.StartsWith(CartName))
                {
                    list.Add((ShopItemModel)HttpContext.Current.Session[key]);
                }
            }

            return list;
        }


    }
}