﻿using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace datatool
{
    [Serializable]
    public class unitclass_settings : base_item
    {
        private string name;
        private modifier_item modifier;
        private int lin_speed;
        private int lin_accel;
        private int turn_move;
        private int turn_aim;

        [DescriptionAttribute("Internal use only")]
        public string Name
        {
            get { return this.name; }
            set { this.name = value; }
        }

        [DescriptionAttribute("The modifier this settings group applies for")]
        [Editor(typeof(modifier_ui_editor), typeof(System.Drawing.Design.UITypeEditor))]
        public modifier_item Modifier
        {
            get { return this.modifier; }
            set { this.modifier = value; }
        }

        [DescriptionAttribute("The maximum speed attainable, pixels/second")]
        public int LinearSpeed
        {
            get { return this.lin_speed; }
            set { this.lin_speed = value; }
        }

        [DescriptionAttribute("The rate of acceleration, pixels/second/second")]
        public int LinearAcceleration
        {
            get { return this.lin_accel; }
            set { this.lin_accel = value; }
        }

        [DescriptionAttribute("The speed of player turns")]
        public int MoveTurnSpeed
        {
            get { return this.turn_move; }
            set { this.turn_move = value; }
        }

        [DescriptionAttribute("The speed of aiming turns")]
        public int AimTurnSpeed
        {
            get { return this.turn_aim; }
            set { this.turn_aim = value; }
        }
        
        override public string getName() { return this.name; }
    }


    public enum unitclass_state_types
    {
        WALK = 0,
        STATIC = 1,
        RUNNING = 2,
        FIRING = 3,
    }

    [Serializable]
    public class unitclass_state : base_item
    {
        private string name;
        private unitclass_state_types type;
        private string image;

        public unitclass_state() { }
        public unitclass_state(unitclass_state_types type) { this.name = type.ToString().ToLower();  this.type = type; }

        [DescriptionAttribute("Internal use only")]
        public string Name
        {
            get { return this.name; }
            set { this.name = value; }
        }

        [DescriptionAttribute("The unit state to use this animation for. If multiple animations are specified for a specific type, one is chosen at random")]
        public unitclass_state_types Type
        {
            get { return this.type; }
            set { this.type = value; }
        }

        [DescriptionAttribute("The name of the image to use for this animation")]
        [Editor(typeof(image_ui_editor), typeof(System.Drawing.Design.UITypeEditor))]
        public string Image
        {
            get { return this.image; }
            set { this.image = value; }
        }

        override public string getName() { return this.name; }
    }


    [Serializable]
    public class unitclass_item : actions_item 
    {
        private string name;
        private List<unitclass_settings> settings;
        private List<unitclass_state> states;

        public unitclass_item(string name)
        {
            this.name = name;

            this.settings = new List<unitclass_settings>();
            this.settings.Add(new unitclass_settings());

            this.states = new List<unitclass_state>();
            this.states.Add(new unitclass_state());
        }

        override public string getName()
        {
            return this.name;
        }



        [DescriptionAttribute("The name of this unitclass, internal use only")]
        public string Name
        {
            get { return this.name; }
            set { this.name = value; }
        }

        [DescriptionAttribute("The available settings")]
        public List<unitclass_settings> Settings
        {
            get { return this.settings; }
        }

        [DescriptionAttribute("The available states")]
        public List<unitclass_state> States
        {
            get { return this.states; }
        }

    }
}
