using System;
using System.Collections.Generic;
using System.Diagnostics;

using FeedMeThis.Interfaces.Client;
using FeedMeThis.Interfaces.Server;

namespace FeedMeThis.Rules
{
	public class MorningRule : IOrderRule
	{
		private const string c_strCoffee = "coffee";

		public bool Apply(IRepository<IDishType> repo, /*List<IDishRequest> drRequests,*/ ref List<IDishDisplay> ddOutput)
		{
			//Establish the mealtime for the first item in the request list.  As we know that all items
			//are either morning or all items are night, if the first item is a night meal then this rule does not apply.

			if (ddOutput[0].TypeRef.MealTime != MealTimeEnum.Morning)
				return true;

			bool result = true;		//We're going to set this to false as soon as we have a failure condition

			//We know this is a morning meal now, let's apply the logic as defined:
			//"In the morning you can order multiple cups of coffee but every other item may be ordered only once"

			//Now, because we're the rules engine it doesn't mean we can assume to know coffee's id, so we're going to
			//pull it from the repository by name so that we can retrieve its id.
			//We'll keep an eye out for that as we iterate the list of item requests.

			IDishType coffee = repo.GetByName(c_strCoffee);
			Debug.Assert(coffee != null);	//If this is null then there's a real problem. Getting null here isn't what I'd
											//test with a normal isnull check.

			foreach (IDishDisplay item in ddOutput)
			{
				if (item.TypeRef.Id != coffee.Id)
					if (item.Count > 1)
					{
						result = false;
						item.IsInError = true;
						item.IsLastItem = true;
						break;
					}
			}

			return result;
		}
	}
}
