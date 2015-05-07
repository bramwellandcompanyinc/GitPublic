using System.Collections.Generic;
using System.Linq;
using FeedMeThese.Interfaces;
using FeedMeThese.Entities;

namespace FeedMeThese.Implementations
{
	//In a real world this would be a configured list of items and not hard-coded, as here.

	public class StaticRepository : IRepository<DishDefinition>
	{
		//In real life this information would be stored in a configuration store such as SQL or an XML resource.

		private List<DishDefinition> _records = new List<DishDefinition>
		{
			new DishDefinition(MealTimeEnum.Morning,	1,		"eggs",		MealCourseEnum.Entre,	false,	1),
			new DishDefinition(MealTimeEnum.Night,		1,		"steak",	MealCourseEnum.Entre,	false,	2),

			new DishDefinition(MealTimeEnum.Morning,	2,		"toast",	MealCourseEnum.Side,	false,	3),
			new DishDefinition(MealTimeEnum.Night,		2,		"potato",	MealCourseEnum.Side,	true,	4),

			new DishDefinition(MealTimeEnum.Morning,	3,		"coffee",	MealCourseEnum.Drink,	true,	5),
			new DishDefinition(MealTimeEnum.Night,		3,		"wine",		MealCourseEnum.Drink,	false,	6),

			new DishDefinition(MealTimeEnum.Night,		4,		"cake",		MealCourseEnum.Desert,	false,	7)
		};

		public IEnumerable<DishDefinition> GetAll()
		{
			return _records;
		}


		//Retreive a single item in the list using its unique identifier

		public DishDefinition GetById(int id)
		{
			IEnumerable<DishDefinition> item = from record in _records
					   where record.Id == id
					   select record;

			return item.First();
		}


		//Given the two dimensions - meal time and index, lookup the single record
		//THIS COULD DO WITH SOME GOOD BOLSTERING UP REF ERROR MANAGEMENT but we don't
		//really have the time for it for the sake of this exercise.

		public DishDefinition GetBy2DSearch<MealTimeEnum, UInt32>(MealTimeEnum meal, UInt32 index)
		{
			IEnumerable<DishDefinition> item = from record in _records
				where record.Index.Equals(index) && record.MealTime.Equals(meal)
				select record;

			if (item.Any())
				return item.First();
			else
				return null;
		}


		//Given a unique dish name, retrieve its record

		public DishDefinition GetByName(string name)
		{
			IEnumerable<DishDefinition> item = from record in _records
					   where record.Name.ToLower() == name.ToLower()
					   select record;

			if (item.Any())
				return item.First();
			else
				return null;
		}
	}
}
