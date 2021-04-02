using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;
using PagedList;
using ShopMVC.Code;

namespace ShopMVC.Models.Start
{
    public class IndexModel
    {
        public string ValueFrom { get; set; }
        public string ValueTo { get; set; }
        public string SearchString { get; set; }
        public string OrderBy { get; set; }

    }
}