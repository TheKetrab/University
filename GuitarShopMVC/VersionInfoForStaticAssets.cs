using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ShopMVC
{
    public static class VersionInfoForStaticAssets
    {
        public static string GetVersionInfoUrl( this string baseUrl )
        {
            var version = typeof(VersionInfoForStaticAssets).Assembly.GetName().Version.ToString();

            return baseUrl + "?_=" + version;
        }
    }
}