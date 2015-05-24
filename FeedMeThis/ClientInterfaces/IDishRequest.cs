using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{
	public interface IDishRequest
	{
		uint Index { get; }
		MealTimeEnum Meal { get; }
		bool IsInError { get; }
		IComparer<IDishRequest> Comparer { get; }
	}

}
