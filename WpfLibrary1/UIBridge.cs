using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

//using DllExportLib; /// This depends on your using library
using System.Windows.Interop;

namespace WpfLibrary1
{
    internal static class UIBridge
    {
        public static MainWindow mainview_ui;

        [DllExport]
        static public IntPtr CreateUserInterface()
        {
            mainview_ui = new MainWindow()
            {
                Opacity = 0,
                Width = 0,
                Height = 0
            };
            mainview_ui.Show();
            return new WindowInteropHelper(mainview_ui).Handle;
        }

        [DllExport]
        static public void DisplayUserInterface()
        {
            mainview_ui.Opacity = 1;
        }

        [DllExport]
        static public void DestroyUserInterface()
        {
            mainview_ui.Close();
        }

        [DllExport]
        static public string GetPath()
        {
            return "test";
        }
    }
}