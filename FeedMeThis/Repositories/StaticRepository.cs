using System;
using System.Collections.Generic;
using System.Linq;

using FeedMeThis.Interfaces.Client;
using FeedMeThis.Interfaces.Server;

namespace FeedMeThis.Repositories
{
	//In a real world this would be a configured list of items and not hard-coded, as here.

	public class StaticRepository : IRepository<IDishType>
	{
		private IDishType[] _records = new IDishType[]
		{
			new DishType(MealTimeEnum.Morning,	1,		"eggs",		"entre",	1),
			new DishType(MealTimeEnum.Night,	1,		"steak",	"entre",	2),

			new DishType(MealTimeEnum.Morning,	2,		"toast",	"side",		3),
			new DishType(MealTimeEnum.Night,	2,		"potato",	"side",		4),

			new DishType(MealTimeEnum.Morning,	3,		"coffee",	"drink",	5),
			new DishType(MealTimeEnum.Night,	3,		"wine",		"drink",	6),

			new DishType(MealTimeEnum.Night,	4,		"cake",		"desert",	7)
		};

		public IEnumerable<IDishType> GetAll()
		{
			return _records;
		}

		public IDishType GetById(int id)
		{
			var item = from record in _records
					   where record.Id == id
					   select record;

			return (IDishType)item;
		}


		//Given the two dimensions - meal time and index, lookup the single record
		//MAKE SURE TO BOLSTER THIS UP WITH SOME GOOD ERROR MANAGEMENT

		public IDishType GetBy2DSearch(object mtMeal, object intIndex)
		{
			MealTimeEnum meal = (MealTimeEnum)mtMeal;
			int index = (int)intIndex;

			var item = from record in _records
					   where record.Index == index && record.MealTime == meal
					   select record;

			return (IDishType)item;
		}


		//Given a unique dish name, retrieve its record

		public IDishType GetByName(string name)
		{
			var item = from record in _records
					   where record.Name.ToLower() == name.ToLower()
					   select record;

			return (IDishType)item;
		}
	}
}
