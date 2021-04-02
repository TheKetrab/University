using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfSemApp
{
    /// <summary>
    /// Logika interakcji dla klasy MyProgressBar.xaml
    /// </summary>
    public partial class MyProgressBar : UserControl, INotifyPropertyChanged
    {

        private int _percent = 0;

        public event PropertyChangedEventHandler PropertyChanged;

        public int Percent
        {
            get { return _percent; }
            set
            {
                if (value > 100) _percent = 100;
                else if (value < 0) _percent = 0;
                else _percent = value;
                OnPropertyChanged();
            }
        }

        protected void OnPropertyChanged([CallerMemberName] string name=null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        public MyProgressBar()
        {
            InitializeComponent();
        }
    }
}
