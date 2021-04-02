using Dapper;
using PagedList;
using ShopMVC.Models.Start;
using System;
using System.Collections.Generic;
using System.Data.Linq;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Web;
using System.Linq.Dynamic;
using System.Web.Configuration;

namespace ShopMVC.Code
{

    public class ShopItems
    {

        private readonly string logPath = WebConfigurationManager.AppSettings["logPath"].ToString();
        private readonly string connectionString = WebConfigurationManager.AppSettings["connectionString"].ToString();

        private SqlConnection connection;

        private static ShopItems _instance;
        private static object _lock = new object();

        public static ShopItems Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_lock)
                    {
                        if (_instance == null)
                        {
                            _instance = new ShopItems();
                        }
                    }
                }

                return _instance;
            }
        }

        public MyPagerInfo Retrieve(IndexModel model, int PageNumber, int PageSize)
        {
            if (string.IsNullOrEmpty(model.ValueFrom)) model.ValueFrom = "0";
            if (string.IsNullOrEmpty(model.ValueTo)) model.ValueTo = "0";


            if (model.OrderBy == null) model.OrderBy = "ID";
            if (model.SearchString == null) model.SearchString = "";

            if (model.OrderBy != "Name" && model.OrderBy != "Name DESC" && model.OrderBy != "Value" && model.OrderBy != "Value DESC")
            {
                model.OrderBy = "ID";
            }

            // LOG
            using (var writer = new StreamWriter(logPath, true, Encoding.UTF8))
            {
                writer.WriteLine("({0}) Get Items | OrderBy: {1} | SearchString: {2}", DateTime.Now, model.OrderBy, model.SearchString);
            }

            // MODEL
            /*
            return items    // DbContext, w nim Instruments context.Instruments (Iqueryable)
                    .Where(item =>
                        item.Name.ToLower().Contains(model.SearchString.ToLower())
                        && (item.Value >= model.ValueFrom || model.ValueFrom <= 0)
                        && (item.Value <= model.ValueTo || model.ValueTo <= 0))
                    .OrderBy(model.OrderBy)
                    .Skip( 50 )
                    .Take(50)
                    .ToList()
                    .ToPagedList(PageNumber, PageSize);
            */

            var paramsOfQuery = new
            {
                ValueFrom = int.Parse(model.ValueFrom),
                ValueTo = int.Parse(model.ValueTo),
                Skip = (PageNumber - 1) * PageSize,
                Take = PageSize
            };


            var querySpecifiers = "";
            querySpecifiers += "WHERE Upper(Name) LIKE '%" + model.SearchString.ToString().ToUpper() + "%' ";
            querySpecifiers += (int.Parse(model.ValueFrom) <= 0) ? "" : "AND Value > @ValueFrom ";
            querySpecifiers += (int.Parse(model.ValueTo) <= 0) ? "" : "AND Value < @ValueTo ";

            int total = (int)connection.ExecuteScalar("SELECT COUNT(*) FROM ShopItems " + querySpecifiers, paramsOfQuery);
            int pagesCount = (int) Math.Ceiling( (decimal)((float)total / (float)PageSize ) );

            var querySort = "ORDER BY " + model.OrderBy + " OFFSET @Skip ROWS FETCH NEXT @Take ROWS ONLY ";

            var list = connection.Query<ShopItemModel>("SELECT * FROM ShopItems " + querySpecifiers + querySort, paramsOfQuery).ToList();

            return new MyPagerInfo
            {
                Items = list,
                ItemsPerPage = PageSize,
                PageNumber = PageNumber,
                PagesCount = pagesCount
            };

        }

        private string GenerateDescription(char c, int n)
        {
            string res = "";
            for (int i=0; i<n; i++)
            {
                res += c;
                res += ' ';
            }

            return res;
        }

        public void GenerateDatabase(int n)
        {
            // CLEAR OLD DATABASE
            connection.Execute("DELETE FROM ShopItems");
            Random rnd = new Random();

            for (var i=1; i<=n; i++)
            {
                var query =
                    "INSERT INTO ShopItems (ID, Name, Description, Value, ImgSrc) " +
                    "VALUES (@ID, @Name, @Description, @Value, @ImgSrc)";


                connection.Execute(query, new {
                    ID = i,
                    Name = string.Format("Gitara {0}", i),
                    Description = GenerateDescription((char)rnd.Next(97, 123), rnd.Next(20, 100)),
                    Value = rnd.Next(1000, 50000) / 10,
                    ImgSrc = string.Format("../Img/G{0}.jpg", rnd.Next(0, 10) + 1)
                });
            }


            // LOG
            using (var writer = new StreamWriter(logPath, true, Encoding.UTF8))
            {
                writer.WriteLine("({0}) Database regenerated", DateTime.Now);
            }
        }

        
        public ShopItemModel GetModel(int ID)
        {
            var query =
                    "SELECT * FROM ShopItems " +
                    "WHERE ID = @ID";

            var res = connection.Query<ShopItemModel>(query, new { ID });
            return res.First();
        }


        private ShopItems()
        {
            connection = new SqlConnection(connectionString);
            connection.Open();

            // LOG
            using (var writer = new StreamWriter(logPath,true,Encoding.UTF8))
            {
                writer.WriteLine("({0}) DATABASE IMPORTED", DateTime.Now);
            }
        }


        public void UpdateItem(ShopItemModel item)
        {

            // DATABASE
            var query = 
                "UPDATE ShopItems " +
                "SET Name = @Name, Description = @Description, Value = @Value, ImgSrc = @ImgSrc " +
                "WHERE ID = @ID";

            connection.Execute(query, new { item.ID, item.Name, item.Description, item.Value, item.ImgSrc });
            

            // LOG
            using (var writer = new StreamWriter(logPath, true, Encoding.UTF8))
            {
                writer.WriteLine("({0}) Item updated [{1}]", DateTime.Now, item.ID);
            }
        }

        public int GetFreeID()
        {

            int freeID = (int)connection.ExecuteScalar("SELECT COUNT(*) FROM ShopItems");
            bool found = false;

            while(!found)
            {
                found = true;
                var res = connection.Query<ShopItemModel>("SELECT * FROM ShopItems WHERE ID = @ID", new { ID = freeID });
                if (res.Count() != 0)
                {
                    found = false;
                    freeID++;
                }
            }

            return freeID;

        }

        public void CreateItem(ShopItemModel item)
        {
            // DATABASE
            
            var query =
                "INSERT INTO ShopItems (ID, Name, Description, Value, ImgSrc) " +
                "VALUES (@ID, @Name, @Description, @Value, @ImgSrc)";

            connection.Execute(query, new { item.ID, item.Name, item.Description, item.Value, item.ImgSrc });
            

            // LOG
            using (var writer = new StreamWriter(logPath, true, Encoding.UTF8))
            {
                writer.WriteLine("({0}) Item added [{1}]", DateTime.Now, item.ID);
            }

        }

        public void DeleteItem(int ID)
        {

            // DATABASE
            var query =
                "DELETE FROM ShopItems " +
                "WHERE ID = @ID";

            connection.Execute(query, new { ID });
            
            // LOG
            using (var writer = new StreamWriter(logPath, true, Encoding.UTF8))
            {
                writer.WriteLine("({0}) Item removed [{1}]", DateTime.Now, ID);
            }
        }
    }
}