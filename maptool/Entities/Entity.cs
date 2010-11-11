﻿using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace Maptool
{
    abstract class Entity
    {
        private int x;
        private int y;
        private int angle;


        [Description("The X co-ordinate of the entity")]
        public int X
        {
            get { return x; }
            set { x = value; }
        }

        [Description("The Y co-ordinate of the entity")]
        public int Y
        {
            get { return y; }
            set { y = value; }
        }

        [Description("The angle of the entity, in degrees")]
        public int Angle
        {
            get { return angle; }
            set { angle = value; }
        }
    }
}
