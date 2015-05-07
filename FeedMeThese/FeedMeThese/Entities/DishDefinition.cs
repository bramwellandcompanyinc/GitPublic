using System;
using System.Collections.Generic;

using FeedMeThese.Interfaces;

namespace FeedMeThese.Entities
{
	public class DishDefinition
	{
		private uint _id = 0;
		private uint _index = 0;
		private string _name = "";
		private MealTimeEnum _meal = MealTimeEnum.NotSet;
		private MealCourseEnum _course = MealCourseEnum.NotSet;
		private bool _multiple = false;

		public DishDefinition() { }

		public DishDefinition(MealTimeEnum meal, uint index, string name, MealCourseEnum course, bool multiple, uint id)
		{
			_id = id;
			_index = index;
			_name = name;
			_meal = meal;
			_course = course;
			_multiple = multiple;
		}

		public uint Index
		{
			get	{ return _index; }
		}

		public string Name
		{
			get	{ return _name; }
		}

		public MealTimeEnum MealTime
		{
			get	{ return _meal; }
		}

		public MealCourseEnum Course
		{
			get	{ return _course; }
		}

		public uint Id
		{
			get	{ return _id; }
		}

		public bool PermitsMultiple
		{
			get { return _multiple; }
		}

		public IComparer<DishDefinition> Comparer
		{
			get	{ return new DishDefinitionComparer(); }
		}
	}
}
