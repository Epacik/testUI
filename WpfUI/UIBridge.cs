using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using net.r_eg.Conari.Types;

//using DllExportLib; /// This depends on your using library
using System.Windows.Interop;
using System.Windows;

namespace WpfUI
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

        private static string msg;
        private static GCHandle MsgHandle;

        [DllExport]
        public static void AllocMsg()
        {
            msg = "Zażółć gęślą jaźń 😨";
            MsgHandle = GCHandle.Alloc(msg);
        }

        [DllExport]
        public static void FreeMsg()
        {
            MsgHandle.Free();
        }

        [DllExport]
        public static IntPtr GetMsg()
        {
            return Marshal.StringToHGlobalUni(msg);
        }
    }
}