using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.RequestParsers
{
	public class TextInputDishRequestParser : IDishTypeInputParser
	{
		public TextInputDishRequestParser() { }

		public bool Parse(object strTextInput, out List<IDishRequest> items, out string strValidationFaultMessage)
		{
			bool result = false;
			items = null;
			strValidationFaultMessage = "";

			string input = null;
			if (NotNullTestTrimAndLCase(strTextInput, out input))
			{
				//Split the input string into an array

				string[] strParamArray = input.Split(',');
				result = ParseParameters(strParamArray, out items, out strValidationFaultMessage);
			}
			else
				strValidationFaultMessage = "Input string was blank; no output will be produced and you're likely going hungry.";

			return result;
		}


		//Perform gross error not-null and spacey string tests.
		//Code should never be passing a null string.

		private bool NotNullTestTrimAndLCase(object strTextInput, out string strNotNullInput)
		{
			bool result = false;

			Debug.Assert(strTextInput != null);

			string input = (string)strTextInput;
			strNotNullInput = input.Trim().ToLower();

			if (input.Length > 0)
				result = true;

			return result;
		}


		//Identify the meal time and initiate the parsing of the (hopefully) numeric selections

		private bool ParseParameters(string[] strParamArray, out List<IDishRequest> items, out string strValidationFaultMessage)
		{
			bool result = false;
			items = null;
			strValidationFaultMessage = "";


			//Firstly, we know that in order to have a valid list of parameters we have to have at least two parameters provided.
			//Namely, this is either "morning" or "night", and at least a single positive integer value.

			if (strParamArray.Length >= 2)
			{
				//Now establish which mealtime this is using the first parameter.  It must match up to one of the valid enumeration items in MealTimeEnum.

				MealTimeEnum meal = MealTimeEnum.NotSet;
				if (DetermineMealTime(strParamArray[0], out meal))
					result = ParseNumericSelections(strParamArray, meal, out items, out strValidationFaultMessage);
				else
					strValidationFaultMessage = "The meal time requested does not match any of the expected values (morning, night).";
			}
			else
				strValidationFaultMessage = "Provided list of parameters did not contain enough information to select a menu item.";

			return result;
		}

		
		//Match the provided meal time against the expected range of values in the enumeration

		private bool DetermineMealTime(string strMealTimeRequest, out MealTimeEnum meal)
		{
			bool result = true;

			if (strMealTimeRequest == "morning")
				meal = MealTimeEnum.Morning;
			else if (strMealTimeRequest == "night")
				meal = MealTimeEnum.Night;
			else
			{
				meal = MealTimeEnum.NotSet;
				result = false;
			}

			return result;
		}


		//Knowing the meal type, continue parsing remaining parameters, testing for numeric values.
		//In order for this portion of the parsing to work, all remaining values must be numeric, otherwise
		//i
		private bool ParseNumericSelections(string[] strParamArray, MealTimeEnum meal, out List<IDishRequest> items, out string strValidationFaultMessage)
		{
			//We know the first item in the array is the meal time, so the rest should be numeric, therefore we'll skip over the first one.
			//Therefore we're going to create a list of items that is as long as the list of numeric selections.

			items = new List<IDishRequest>();
			bool error = false;
			strValidationFaultMessage = "";

			for (int i = 1; i < strParamArray.Length; i++)
			{
				string strSelection = strParamArray[i];
				uint uintSelection = 0;
				if (!UInt32.TryParse(strSelection, out uintSelection))
					error = true;

				//If the selection was not numeric the error flag will be set on the request.  This will
				//be used later.

				IDishRequest request = new DishRequest(meal, uintSelection, error);
				items.Add(request);
			}

			if (error)
				strValidationFaultMessage = "At least one item selection parameter was not numeric (expected only unsigned integer values).";

			return !error;
		}
	}
}
