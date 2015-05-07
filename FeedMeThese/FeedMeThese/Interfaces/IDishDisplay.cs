using FeedMeThese.Entities;
using System.Collections.Generic;

namespace FeedMeThese.Interfaces
{
	public interface IDishDisplay
	{
		DishDefinition DefinitionRef { get; set; }
		uint Count { get; set; }
		IComparer<IDishDisplay> Comparer { get; }
	}
}