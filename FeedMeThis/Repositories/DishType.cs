using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.Repositories
{
	public class DishType : IDishType
	{
		private int _id = 0;
		private int _index = 0;
		private string _name = "";
		private MealTimeEnum _meal = MealTimeEnum.NotSet;
		private string _type = "";

		public DishType() { }

		public DishType(MealTimeEnum meal, int index, string name, string type, int id)
		{
			_id = id;
			_index = index;
			_name = name;
			_meal = meal;
			_type = type;
		}

		public int Index
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

		public string Type
		{
			get	{ return _type; }
		}

		public int Id
		{
			get	{ return _id; }
		}

		public string MealCourse
		{
			get	{ return _type; }
		}

		public IComparer<IDishType> Comparer
		{
			get	{ return new DishTypeComparer(); }
		}
	}
}
