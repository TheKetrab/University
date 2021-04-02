using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Logika interakcji dla klasy MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            CenterWindowOnScreen();
        }

        public void CenterWindowOnScreen()
        {
            double sw = SystemParameters.PrimaryScreenWidth;
            double sh = SystemParameters.PrimaryScreenHeight;
            double ww = this.Width;
            double wh = this.Height;

            this.Left = sw / 2 - ww / 2;
            this.Top = sh / 2 - wh / 2;
        }

        private void RunProgressBar(object sender, RoutedEventArgs e)
        {
            /*
            Task.Run(async () =>
            {
                for (pb.Percent = 0; pb.Percent < 100; pb.Percent++)
                    await Task.Delay(50);
            });
            */

            new Thread(() =>
            {
                for (pb.Percent = 0; pb.Percent < 100; pb.Percent++)
                    Thread.Sleep(50);

                Dispatcher.BeginInvoke((Action)(() =>
                {
                    wpfText.FontSize = 40;
                    wpfText.Text = "DONE";
                }));

            }).Start();


        }

        private void CommandBinding_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CommandBinding_Executed_Close(object sender, ExecutedRoutedEventArgs e)
        {
            SystemCommands.CloseWindow(this);
        }

        private void CommandBinding_Executed_Minimize(object sender, ExecutedRoutedEventArgs e)
        {
            SystemCommands.MinimizeWindow(this);
        }

        private void StackPanel_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }
    }
}
