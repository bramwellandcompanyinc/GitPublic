using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Server;

namespace FeedMeThis.Rules
{
	public class RuleManager : IRuleManager
	{
		//In a real world this would be a configured list of coded implementations around the rule interface or would
		//be based on the configuration of a more complex configurable rule engine with its own rule constructs.
		//This would be a perfect case for dynamically configured implementations, which this class would be unaware of at design-time.
		//For the sake of the exercise at hand that is probably a bit more than is needed.

		public IOrderRule[] GetRules()
		{
			return new IOrderRule[] { new NightRule(), new MorningRule() };
		}
	}
}
