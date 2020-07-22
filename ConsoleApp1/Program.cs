using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using net.r_eg.Conari.Types;

namespace ConsoleApp1
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            WpfUI.UIBridge.AllocMsg();
            var us = WpfUI.UIBridge.GetMsg();
            WpfUI.UIBridge.FreeMsg();
        }
    }
}