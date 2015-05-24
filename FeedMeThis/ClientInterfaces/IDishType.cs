using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{
	public interface IDishType
	{
		int Id { get; }
		int Index { get; }
		string Name { get; }
		MealTimeEnum MealTime { get; }
		string MealCourse { get; }
		IComparer<IDishType> Comparer { get; }
	}
}
