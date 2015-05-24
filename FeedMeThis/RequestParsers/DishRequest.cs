using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.RequestParsers
{
	public class DishRequest : IDishRequest
	{
		private uint _index = 0;
		private MealTimeEnum _meal = MealTimeEnum.NotSet;
		private bool _error = false;

		public DishRequest() { }

		public DishRequest(MealTimeEnum meal, uint index, bool error)
		{
			_index = index;
			_meal = meal;
			_error = error;
		}

		public uint Index
		{
			get	{ return _index; }
		}

		public MealTimeEnum Meal
		{
			get	{ return _meal; }
		}

		public bool IsInError
		{
			get { return _error; }
		}

		public IComparer<IDishRequest> Comparer
		{
			get	{ return new DishRequestComparer(); }
		}
	}
}
