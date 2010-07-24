﻿using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace datatool
{
    static class Program
    {
        public static DataProvider dp;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string datapath;

            // The datapath is the current directory,
            // unless if we are running out of the Debug directory for the datatool.
            // In that case, we assume a chaotic rage install is a couple directories above.
            datapath = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            datapath = datapath.Replace("\\datatool\\bin\\Debug", "");
            datapath += "\\data\\cr";

            dp = new DataProvider();
            dp.load(datapath);

            string file = System.IO.File.ReadAllText(datapath + "\\particletypes\\particletypes.conf");

            ConfuseReader read = new ConfuseReader();
            ConfuseSection sect = read.Parse(file);
            if (sect == null) {
                MessageBox.Show("Parse error");
            }

            System.Diagnostics.Debug.WriteLine(sect);

            //Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Main());
        }
    }
}
