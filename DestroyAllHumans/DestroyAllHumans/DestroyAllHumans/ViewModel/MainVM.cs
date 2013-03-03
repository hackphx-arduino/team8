using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace DestroyAllHumans.ViewModel
{
    class MainVM : DependencyObject
    {
        // Dependency Property DebugInfo
        public static readonly DependencyProperty DebugInfoProperty =
             DependencyProperty.Register("DebugInfo", typeof(String), typeof(MainVM),
             new PropertyMetadata(null, DebugInfoChangedCallback));

        private static void DebugInfoChangedCallback(
            DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            Console.WriteLine(e.OldValue + " " + e.NewValue);
        }
        
        public String DebugInfo
        {
            get { return (String)GetValue(DebugInfoProperty); }
            set { SetValue(DebugInfoProperty, value); }
        }
        // Dependency Property DebugInfo

        public MainVM()
        {
        }
    }
}
