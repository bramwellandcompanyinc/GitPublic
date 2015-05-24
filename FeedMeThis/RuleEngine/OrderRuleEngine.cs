using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

using FeedMeThis.Framework;
using FeedMeThis.Interfaces.Client;
using FeedMeThis.Interfaces.Server;
using FeedMeThis.Repositories;
using FeedMeThis.Rules;

namespace FeedMeThis.Rules
{
	//This class is not thread safe and can only be used instance-per-thread

	public class OrderRuleEngine : IOrderRuleEngine
	{
		private List<IDishRequest> _drRequests = null;
		private List<IDishDisplay> _ddOutput = null;
		private IRepository<IDishType> _repo = null;
		private IOrderRule[] _rules = null;

		public OrderRuleEngine()
		{
			//Ready the menu item repository

			Factory.RegisterType<IRepository<IDishType>, StaticRepository>();	//Would be configured via Unity given more time, not referenced
			_repo = Factory.Instantiate<IRepository<IDishType>>();

			//Ready the rules

			Factory.RegisterType<IRuleManager, RuleManager>();	//Would be configured via Unity given more time, not referenced
			IRuleManager rm = Factory.Instantiate<IRuleManager>();
			_rules = rm.GetRules();
		}


		//Execute the business rules against the received list of requests.
		//The first rule that fails will cause the creation of a dummy dish display
		//at the currently parsed request item, that will contain the stop directive
		//for producing output at the client.

		public List<IDishDisplay> Process(List<IDishRequest> drRequests)
		{
			Debug.Assert(_drRequests.Count > 0);

			_drRequests = drRequests;
			_ddOutput = new List<IDishDisplay>();

			Sort();
			bool blnPreRuleError = ConstructDistinctRequestList();

			//Iterate rules. Each rule can operate on the request list as a whole, and this is necessary
			//because some of the rules span all the items in the list and cannot operate on just one request item.

			bool blnContinueRules = true;
			int intRuleIndex = 0;
			while (blnContinueRules)
				blnContinueRules = _rules[intRuleIndex++].Apply(_repo, ref _ddOutput);

			return _ddOutput;
		}


		//Use the dish type comparer to order the requests ascending numerically by dish type index.

		private void Sort()
		{
			IComparer<IDishRequest> comp = _drRequests[0].Comparer;
			_drRequests.Sort(comp);
		}


		//Construct the unprocessed list of output dish display items.  This will create an entry for
		//each distinct meal type in the request list.  As the rules are executed on the request list
		//they will apply error flags to the list (that will ultimately affect rendering of the list)

		//THIS NEEDS SOME SERIOUS ERROR HANDLING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		private bool ConstructDistinctRequestList()
		{
			bool blnUnerrored = true;
			MealTimeEnum mtMeal = _drRequests[0].Meal;
			int intPreviousIndex = 0;	//The previous index value is used to detect multiple instances of the same item,
										//which will work because the list is sorted and successive occurences of
										//the same index will be evidnt

			int intCurrentOut = 0;		//The index of the current distinct menu item, incremented as we find a new one

			foreach (IDishRequest item in _drRequests)
			{
				//Firstly try to identify the item in the repository to see if it exists, but only
				//if this item doesn't have the same index as the last one (in which case it's a repeat order).

				if (item.Index != intPreviousIndex)
				{
					DishDisplay dd = new DishDisplay();	//Don't need to use the factory here as this assembly is responsible for
														//the instantiation of this concrete class.

					IDishType lookup = _repo.GetBy2DSearch(item.Meal, item.Index);

					if (lookup != null)
					{
						dd.TypeRef = lookup;
						//dd.DisplayAs = lookup.Name;
					}
					else
					{
						//We're going to create a placeholder item with error flags.

						blnUnerrored = false;
						dd.IsLastItem = true;	//There could be potentially more than one dish in error that has this flag but
												//that's ok because ultimately the output generation will stop at the first occurence
												//of this flag anyway.
						dd.IsInError = true;
						dd.TypeRef = null;
					}

					//Add the dish display to the output list.  Each of these will correspond to exactly one
					//item in the comma delimited rendered output in the case of a text writer, or as an XML element
					//in the case of an XML writer.

					_ddOutput.Add(dd);

					//Move the output index up one.  This only ever gets incremented if
					//the current item didn't have the same index as the previous item.

					intCurrentOut++;					
				}
				else
					_ddOutput[intCurrentOut].Count++;	//Just increment the instance count for the same dish type as is currently
														//at the head of the output list.
			}

			//Make sure to set the "is last item" flag on the last output item.  This is just a nicety
			//that the output generator may want to take advantage of down the road.

			_ddOutput[--intCurrentOut].IsLastItem = true;

			return blnUnerrored;
		}
	}
}
