using System;
using FeedMeThese.Interfaces;
using FeedMeThese.Entities;

namespace FeedMeThese.Implementations
{
	public class InputParser : IInputParser
	{
		private IParsedOutput _output = null;
		private IRepository<DishDefinition> _repo = null;

		public InputParser(IParsedOutput output, IRepository<DishDefinition> repo)
		{
			_output = output;
			_repo = repo;
		}

		//Given the input string from the user, parse this into usable information
		//and construct and output structure containing the valid menu selections and
		//if appropriate, the error state.

		public IParsedOutput ParseInput(string input)
		{
			//The end game here is a collection of valid selections sorted in order of
			//entre, side, drink, desert (if applicable), and an error flag set if errors
			//were encountered

			MealTimeEnum mealTime = MealTimeEnum.NotSet;
			_output.Clear();

			string[] items = input.Split(',');

			//The behaviour is not specified in requirements if meal time is not specified, so I'm making a design choice to return "error".

			bool result = ParseMealTime(items, out mealTime);
			if (result)
				result = ParseSelections(items, mealTime);

			if (!result)
				_output.SetErrorState();

			return _output;
		}

		//Given the list of inputs, we expect the first one to conform to the enumeration
		//definition set forth in MealTimeEnum.  We're going to parse the first item provided
		//in the list and make sure it does.

		private bool ParseMealTime(string[] items, out MealTimeEnum mealTime)
		{
			bool result = true;

			if (items.Length > 0)
			{
				string mealTimeCandidate = items[0].Trim().ToLower();
				Int32 discard = 0;
				
				//We test for the first value being numeric because the enum.TryParse can actually resolve to a valid
				//item if the number is in the defined range of the enum.

				if (!MealTimeEnum.TryParse(mealTimeCandidate, true, out mealTime) || Int32.TryParse(mealTimeCandidate, out discard))
					result = false;
			}
			else
				mealTime = MealTimeEnum.NotSet;

			return result;
		}


		//Given the list of selections in the list, individually identify each one, check that it valid
		//for the given time of day, and that it conforms to rules on multiple items.  Construct an output
		//structure that will drive the written output.

		private bool ParseSelections(string[] items, MealTimeEnum mealTime)
		{
			bool error = false;

			if (items.Length > 1)
			{
				for (int i = 1; i < items.Length; i++)
				{
					string index = items[i];
					uint request = 0;

					if (UInt32.TryParse(index, out request))
					{
						//Query the menu repo for the item using mealtime and index

						DishDefinition dish = _repo.GetBy2DSearch<MealTimeEnum, uint>(mealTime, request);
						if (dish != null)
						{
							//We have an identified dish, now lets see if its addition
							//is compliant with business rules

							uint existingCount = _output.CountOf(dish);
							if (existingCount == 0)
								_output.Add(dish);	//It doesn't exist in our ouput list yet, add it now
							else if (existingCount > 0 && dish.PermitsMultiple)
								_output.Add(dish);	//We're allowed to add more than one of this item
							else
								error = true;	//Not allowed to add more than one item unless PermitsMultiple is configured in the DishDefinition
						}
						else
							error = true;	//There was no dish defined for the provided index (index out of bounds)
					}
					else
						error = true;	//The number did not parse to an unsighed integer

					if (error)
						break;	//Any error has to cause us to stop parsing
				}
			}
			else
			{
				//We're going to return a zero-item list, essentially.
			}

			return !error;
		}


	}
}
