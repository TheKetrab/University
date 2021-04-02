using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ShopMVC.Models.Start
{
    public class ShopItemModel
    {
        public int ID { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public double Value { get; set; }
        public string ImgSrc { get; set; }

        

    }
}